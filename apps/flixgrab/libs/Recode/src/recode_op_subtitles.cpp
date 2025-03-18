#include "recode_op_subtitles.h"

#include <iostream>
#include <fstream>
#include <regex>

#include <utils/string.h>


using namespace recode;
using namespace std;

//расширения файлов, которые класс може тпреобразовать в SRT
#define FILE_FORMAT_SRT                 L".srt"
#define FILE_FORMAT_TTML                L".ttml"
#define FILE_FORMAT_VTT                 L".vtt"
#define FILE_FORMAT_XML                 L".xml"

#define VTT_MAP                         L"X-TIMESTAMP-MAP"


RecodeOpSubtitles::RecodeOpSubtitles(const std::string& stream){
    stream_ = utils::Utf8ToUtf16(stream.c_str());
}

RecodeOpSubtitles::~RecodeOpSubtitles() {}


Status RecodeOpSubtitles::Do(OperationListener* listener) {
    //lock_guard<mutex> lock(mutex_);
    Status status;
    cout << "RecodeOpSubtitles::Do() " << endl;

    if (is_cancelled()) {
        //case if we was cancelled when we was waiting by mutex
        cout << "RecodeOpSubtitles Cancelled " << endl;
        return status;
    }

    if (checkFormatIsSupported(stream_))
        status = clearVTT(stream_);
    else
        cout << "RecodeOpSubtitles unsupported format" << endl;

    return status;
}

void RecodeOpSubtitles::Cancel() {
    Operation::Cancel();
}

Status RecodeOpSubtitles::clearVTT(wstring source) {
    Status status;

    //Шапка
    wstring head_str = L"WEBVTT";
    bool head_found = false;

    //ТЭГ STYLE
    wstring tag_STYLE = L"STYLE";
    bool tag_STYLE_found = false;

    //если исходный файл не задан файл
    if (source.empty())
        return Status(error::Code::RECODE_SUBTITLES_ERROR, "No input file", RECODESUBTITLES_ERROR_NO_INPUT_FILE);

    //проверяем, можем ли мы работать с файлом данного типа
    if (!checkFormatIsSupported(source))
        return Status(error::Code::RECODE_SUBTITLES_ERROR, "File format not supported", RECODESUBTITLES_ERROR_FORMAT_NOT_SUPPORTED);

    //открываем исходный файл
    wifstream file_in;
    file_in.open(source);
    //если файл не открылся
    if (!file_in.is_open())
        return status; //Status(error::Code::RECODE_SUBTITLES_ERROR, "No input file is open", RECODESUBTITLES_ERROR_OPEN_INPUT_FILE);

    vector<wstring> vector_data;

    //считываем файл vtt до конца
    wstring read_line;
    while (getline(file_in, read_line)) {
        //игнорируем X-TIMESTAMP-MAP строки
        if (!read_line.empty() && read_line.find(VTT_MAP) != wstring::npos) // hbo case
            continue;

        //удаялем пробелы спереди
        while (!read_line.empty() && read_line[0] == L' ')
            read_line.erase(0, 1);

        //удаялем пробелы сзади
        while (!read_line.empty() && read_line[read_line.size() - 1] == L' ')
            read_line.erase(read_line.size() - 1, 1);

        //если нашли шапку вервый раз
        if (read_line == head_str) {
            //если первый раз, то оставляем для записи в файл
            if (!head_found) {
                head_found = true;
                //добавляем строку в вектор
                vector_data.push_back(read_line);
                continue;
            }
            //если уже находили шапку, то переходим к соедующей строке
            else {
                continue;
            }
        }

        //Если это STYLE
        if (read_line == tag_STYLE) {
            tag_STYLE_found = true;
            continue;
        }
        //Если STYLE уже найдет
        if (tag_STYLE_found) {
            //если конец тега
            if (read_line == L"}")
                tag_STYLE_found = false;

            continue;
        }

        //проверяем строку на то что это время
        //для проверки времени
        wregex rx_1(L"[0-9][0-9][:][0-9][0-9][:][0-9][0-9][,][0-9][0-9][0-9]");
        wregex rx_2(L"[0-9][0-9][:][0-9][0-9][:][0-9][0-9][.][0-9][0-9][0-9]");

        bool this_time = false;

        //берем 12 символов
        if (read_line.size() > 11) {
            //если строка 
            wstring tmp = read_line.substr(0, 12);

            //если это время
            if (regex_match(tmp, rx_1) || regex_match(tmp, rx_2)) {
                //вместо запятых устанавливаем точки
                for (unsigned int i = 0; i < read_line.size(); ++i) {
                    if (read_line[i] == L',') {
                        read_line.erase(i, 1);
                        read_line.insert(i, L".");
                    }
                }

                read_line.resize(29);
                vector_data.push_back(read_line);
                continue;
            }
        }

        //Удаляем теги
        removeTag(read_line);

        //если предыдущая строка пустая и эта пустая, то переходим к следующей
        if (!vector_data.empty()
            && vector_data.at(vector_data.size() - 1) == L""
            && read_line == L"") {
            continue;
        }

        //добавляем строку в вектор 
        vector_data.push_back(read_line);
    }

    //закрываем файл
    file_in.close();

    //открываем файл для записи (перезаписываем этот файл)
    wofstream file_out;
    file_out.open(source);
    //если файл не открылся
    if (!file_out.is_open())
        return Status(error::Code::RECODE_SUBTITLES_ERROR, "No input file is open", RECODESUBTITLES_ERROR_OPEN_INPUT_FILE_REWRITE);

    //записываем данные
    for (int i = 0; i < vector_data.size(); ++i) {
        file_out << vector_data.at(i);
        if (i < vector_data.size() - 1)
            file_out << L"\n";
    }

    //закрываем файл
    file_out.close();

    return status;
}

/* Найти последнее вхождение в строке */
int RecodeOpSubtitles::findLast(wstring& str, wstring find_str) {
    int res = -1;

    while (true) {
        int pos = str.find(find_str, res + 1);

        if (pos != wstring::npos)
            res = pos;
        else
            break;
    }

    return res;
}

/* Взять расширение (формат) файла */
wstring RecodeOpSubtitles::getFormatFile(wstring fileName) {
    wstring character = L".";
    int i = findLast(fileName, character);

    if (i != wstring::npos) {
        fileName = fileName.substr(i, fileName.size() - i);
        return fileName;
    }

    return L"";
}

bool RecodeOpSubtitles::checkFormatIsSupported(wstring fileName) {
    //Узнаем расширение (формат) файла
    fileName = getFormatFile(fileName);

    //если расширене подходящее, то возвращаем true
    if (fileName == FILE_FORMAT_VTT)
        return true;

    return false;
}

void RecodeOpSubtitles::removeTag(wstring& source) {
    wstring res;
    bool found = false;
    int index_found = -1;
    int index_start = -1;
    int index_stop = -1;
    for (int i = 0; i < source.size(); ++i) {
        if (index_found == -1 && source.at(i) == L'<') {
            index_found = i;
            continue;
        }

        if (index_found != -1 && source.at(i) == L'>') {
            index_found = -1;
            continue;
        }

        if (index_found == -1)
            res.push_back(source.at(i));
    }

    //если конца тега не нашли, значит это был не тег, и нужно дописать все что пропустили
    while (index_found != -1 && index_found < source.size())
        res.push_back(source.at(index_found));

    source = res;
}