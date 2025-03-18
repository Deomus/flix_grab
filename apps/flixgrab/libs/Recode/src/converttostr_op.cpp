#include "converttostr_op.h"

#include <tchar.H>
#include <utility>
#include <regex>
#include <filesystem>

#include <languages.h>
#include <utils/string.h>

#if NETFLIXPORTALENABLED == 1
	#pragma comment(lib, LEPTONICA_LIBRARY)
	#pragma comment(lib, TESSERACT_LIBRARY)

    #include <tesseract/baseapi.h> 
    #include <leptonica/allheaders.h>
#endif

#define _CRT_SECURE_NO_WARNINGS

//расширения файлов, которые класс може тпреобразовать в SRT
#define FILE_FORMAT_SRT                 L".srt"
#define FILE_FORMAT_TTML                L".ttml"
#define FILE_FORMAT_VTT                 L".vtt"
#define FILE_FORMAT_XML                 L".xml"

//для файлов ttml
#define TTML_START_LINE                 L"<p "
#define TTML_STOP_LINE                  L"</p>"
#define TTML_START_LINE2                L"<tt:p "
#define TTML_STOP_LINE2                 L"</tt:p>"

#define TTML_ENCODING                   L"encoding="
#define TTML_TTP_TICKRATE               L"ttp:tickRate="
#define TTML_XML_LANGUAGE               L":lang="
#define TTML_XML_MAX_NUM                L":lang="
#define TTML_BEGIN                      L"begin="
#define TTML_END                        L"end="
#define TTML_BR                         L"<br/>"
#define TTML_BR2                        L"<tt:br/>"
#define TTMP_XML_SRC                    L"src="

#define TMP_FILE                        "tmp"


using namespace recode;
using namespace std;


std::mutex   ConvertToStrOp::mutex_;

ConvertToStrOp::ConvertToStrOp(const std::string& tessdata_folder)
{
    tessdata_folder_ = tessdata_folder;
}

ConvertToStrOp::~ConvertToStrOp() 
{
}

void ConvertToStrOp::Add(const std::string& in_file, const std::wstring& out_file) {
    std::wstring in_file_w = utils::Utf8ToUtf16(in_file.c_str());

    if (SupportedFormat(in_file_w))
        streams_.push_back({ in_file_w, out_file });

    std::cout << "ConvertToStrOp::Add " << in_file.c_str() << " to out streams size:" << streams_.size() << std::endl;
}

bool ConvertToStrOp::SupportedFormat(wstring fileName) {
    //Узнаем расширение (формат) файла
    fileName = getFormatFile(fileName);

    //если расширене подходящее, то возвращаем true
    if (fileName == FILE_FORMAT_SRT
        || fileName == FILE_FORMAT_TTML
        || fileName == FILE_FORMAT_VTT
        || fileName == FILE_FORMAT_XML)
        return true;

    return false;
}

/* Взять название файла */
vector<wstring> ConvertToStrOp::getLanguageForSourceFile(wstring& in) {
    vector<wstring> res;

    if (in.empty())
        return res;
    convertPathFile(in);

    //проверяем, можем ли мы работать с файлом данного типа
    if (!SupportedFormat(in))
        return res;

    //открываем исходный файл
    wifstream fileSource;
    fileSource.open(in);
    //если файл не открылся
    if (!fileSource.is_open())
        return res;

    //идем по всему файлу и ищем язык
    wstring read_line;
    wstring lang = L"";
    while (getline(fileSource, read_line)) {

        int pos = findLast(read_line, TTML_XML_LANGUAGE);

        if (pos != wstring::npos) {
            wstring str = TTML_XML_LANGUAGE;
            pos += str.size();
            pos++;

            str = read_line.substr(pos, read_line.size());

            pos = str.find(L'"');
            str = str.substr(0, pos);

            lang = str;

            break;
        }
    }

    if (!lang.empty())
        transform(lang.begin(), lang.end(), lang.begin(), ::tolower);

    //закрываем файл
    fileSource.close();
    //берем язык для tesseract
    lang = utils::Utf8ToUtf16(tesseractLang(utils::Utf16ToUtf8(lang)));

    //их может быть несколького, поэтому заполняем вектор
    wstring str = L"";
    for (int i = 0; i < lang.size(); ++i)
    {
        if (lang.at(i) == L'+')
        {
            res.push_back(str);
            str.clear();
        }
        else
            str.push_back(lang.at(i));
    }
    res.push_back(str);

    return res;

    //vector<wstring> files;
    //std::wstring path = filesystem::current_path().wstring() + L"\\" + TESSERACTFOLDER;
    //for (const auto & entry : filesystem::directory_iterator(path)) {
    //    std::wstring file_name = entry.path().filename().wstring();
    //    if (file_name.find(tes_lang + L".") != std::wstring::npos
    //        || file_name == (tes_lang + L".traineddata"))
    //        files.push_back(file_name);
    //}

    //return getTesseractDataFiles(lang);
}

Status ConvertToStrOp::Do(OperationListener* listener) {
    //std::lock_guard<std::mutex> lock(mutex_);
    Status status;
    std::cout << "ConvertToStrOp::Do() " << std::endl;

    if (is_cancelled()) {
        //case if we was cancelled when we was waiting by mutex
        std::cout << "ConvertToStrOp Cancelled " << std::endl;
        return status;
    }

    for (auto& stream : streams_) {
        //open input file
        wstring in_path = stream.first;
        convertPathFile(in_path);

        wifstream fileSource;
        fileSource.open(in_path);
        //file  isn't opened
        if (!fileSource.is_open())
            return status;// Status(error::Code::CONV_SUBTITLES_ERROR, "Input file not open", CONV_ERROR_CODE_OPEN_INPUT_FILE);

        //open output file
        wstring out_path = stream.second;
        convertPathFile(out_path);

        wofstream fileSRT;
        fileSRT.open(out_path);
        //file  isn't opened
        if (!fileSRT.is_open()) {
            fileSource.close();
            return Status(error::Code::CONV_SUBTITLES_ERROR, "Output file not open", CONV_ERROR_CODE_OPEN_OUTPUT_FILE);
        }

        //узнаем расширение файла
        wstring fileFormat = getFormatFile(stream.first);

        //если srt
        if (fileFormat == FILE_FORMAT_SRT)
            handleSRT(fileSource, fileSRT);
        //если TTML
        else if (fileFormat == FILE_FORMAT_TTML)
            handleTTML(fileSource, fileSRT);
        //если VTT
        else if (fileFormat == FILE_FORMAT_VTT)
            handleVTT(fileSource, fileSRT);
        //если это XML
        else if (fileFormat == FILE_FORMAT_XML) {
#if NETFLIXPORTALENABLED == 1
            //берем необходимый язык
            vector<wstring> vec_lang = getLanguageForSourceFile(stream.first);
            if (vec_lang.empty())
                return Status(error::Code::CONV_SUBTITLES_ERROR, "Language not defined", CONV_ERROR_CODE_LANGUAGE_NOT_DEFINED);

            wstring lang = L"";
            for (int i = 0; i < vec_lang.size(); ++i) {
                lang += vec_lang.at(i);

                if (i < vec_lang.size() - 1)
                    lang += L"+";
            }

            //ПРЕОБРАЗУЕМ В STRING (без потери символов)
            //открываем для записи
            wofstream out_tmp;
            out_tmp.open(TMP_FILE);
            //если файл открылся
            if (out_tmp.is_open()) {
                out_tmp << lang;
                out_tmp.close();
            }
            //открываем для чтения
            string tmp_lang = "";
            ifstream in_tmp;
            in_tmp.open(TMP_FILE);
            //если файл открылся
            if (in_tmp.is_open()) {
                getline(in_tmp, tmp_lang);
                in_tmp.close();
                remove(TMP_FILE);
            }
            //если нет файла (файлов) с нужным языком (языками)
            tesseract::TessBaseAPI myOCR;
            if (myOCR.Init(tessdata_folder_.c_str(), tmp_lang.c_str()) == -1)
                return Status(error::Code::CONV_SUBTITLES_ERROR, "No data files", CONV_ERROR_CODE_NO_DATA_FILES);

            /* Считать файл XML (TTML) с картинками */
            wstring read_line;
            int cur_num_title = 0;//порядковый номер sutitles

            int max_num_title = 0;

            while (getline(fileSource, read_line))
                ++max_num_title;

            fileSource.clear();
            fileSource.seekg(0, ios::beg);

            max_num_title = max_num_title - 10; //10 starting and ending of xml file without needed data
            max_num_title = max_num_title / 3;

            while (getline(fileSource, read_line) 
                && !this->is_cancelled()) {

                //ищем кодировку исходного файла
                if (this->ttmlEncoding_not_find && getEncoding(read_line) != L"") {
                    /* Your CODE */
                }

                //ищем tickRate
                getTTP_tickRate(read_line);

                //ищем xml_language
                wstring language = getXML_Language(read_line);

                //проверяем строку
                if (checkLineXML_TTMLonSubtitles(read_line)) {
                    fileSRT << to_wstring(++cur_num_title) << L'\n' << getTIMEfromXML_TTML(read_line) << L"\n";

                    if (!getline(fileSource, read_line))
                        break;

                    //взять строку SRT из картинки (XML)

                    //узнаем имя картинки
                    int index = findLast(in_path, L"\\");

                    if (index > -1) {

                        if (listener)
                            listener->OnProgress(cur_num_title / (double)max_num_title);

                        int pos = findLast(read_line, TTMP_XML_SRC);

                        string pathFile_char = "";
                        if (pos != wstring::npos) {
                            wstring nameFile = in_path.substr(0, index + 1);

                            wstring str = TTMP_XML_SRC;
                            pos += str.size();
                            pos++;

                            str = read_line.substr(pos, read_line.size());

                            pos = str.find(L'"');
                            str = str.substr(0, pos);

                            nameFile += str;

                            //открываем для записи
                            wofstream out;
                            out.open(TMP_FILE);
                            //если файл открылся
                            if (out.is_open()) {
                                out << nameFile;
                                out.close();
                            }

                            //открываем для чтения
                            ifstream in;
                            in.open(TMP_FILE);
                            //если файл открылся
                            if (in.is_open()) {
                                getline(in, pathFile_char);
                                in.close();
                            }

                        }

                        Pix* pix = pixRead(pathFile_char.c_str()); // загружаем картинку с текстом с диска 

                        if (pix) {
                            myOCR.SetImage(pix);

                            // вывод результатов распознавания текста в файл
                            FILE* f = nullptr;
                            if (0 == fopen_s(&f, TMP_FILE, "w")) {
                                //fprintf(f, "OCR output:\n\n");
                                fprintf(f, myOCR.GetUTF8Text());
                                fclose(f);

                                wifstream in;
                                in.open(TMP_FILE);
                                //если файл открылся
                                if (in.is_open()) {
                                    wstring read_line;
                                    wstring res = L"";
                                    while (getline(in, read_line) && read_line != L"\n") {
                                        res += read_line;
                                        res += L"\n";
                                    }

                                    in.close();

                                    fileSRT << res << L"\n";
                                }
                            }
                            // else return 
                        }
                    }
                    else {
                        fileSRT << L"error" << L"\n";
                    }
                }
            }

            //удаляем временный файл
            remove(TMP_FILE);
#endif
        }
        //неизвестный формат
        else {
            //закрываем файлы
            fileSource.close();
            fileSRT.close();
            return Status(error::Code::CONV_SUBTITLES_ERROR, "File format not supported", CONV_ERROR_CODE_FORMAT_NO_SUPPORTED);
        }

        //закрываем файлы
        fileSource.close();
        fileSRT.close();

        if (fileFormat == FILE_FORMAT_XML)
            filesystem::remove_all(filesystem::path(in_path).parent_path());
        else if (save_original_track_)
            filesystem::rename(in_path, out_path.substr(0, out_path.find_last_of(L"\\")) + in_path.substr(in_path.find_last_of(L"\\"), in_path.size()));
        else
            filesystem::remove(in_path);
    }

    return status;
}

void ConvertToStrOp::Cancel() {
    Operation::Cancel();
}

/* Найти последнее вхождение в строке */
int ConvertToStrOp::findLast(wstring &str, wstring find_str) {
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
wstring ConvertToStrOp::getFormatFile(wstring fileName) {
    wstring character = L".";
    int i = findLast(fileName, character);

    if (i != wstring::npos) {
        fileName = fileName.substr(i, fileName.size() - i);
        return fileName;
    }

    return L"";
}

/* Функция преобразует строку к виду windows. Например:
 * "D:/q/w.ttml" -> "D:\\q\\w.ttml" */
void ConvertToStrOp::convertPathFile(wstring &pathFile) {
    int pos = wstring::npos;

    //заменяем слеш на два обратных слеша
    while ((pos = pathFile.find(L"/", pos + 1)) != wstring::npos) {
        wstring str = pathFile.substr(0, pos);
        str += L"\\\\";
        str += pathFile.substr(pos + 1, pathFile.size());
        pathFile = str;
    }

    //убираем двойные кавычки
    while ((pos = pathFile.find(L"\"", pos + 1)) != wstring::npos) {
        pathFile.erase(pos, 1);
    }
}

/* Функция взять тег: <?>
 * параметр first: если true то первый попашийся
 * false - последний
 * возвращает пару: позиция, сам тег */
pair<int, wstring> ConvertToStrOp::getTagXML(wstring &line, bool first) {
    int pos = wstring::npos;

    //если первый
    if (first) {
        pos = line.find(L"<");
    }
    else {
        pos = findLast(line, L"<");
    }

    pair<int, wstring> tmp_pair(pos, L"");

    if (pos != wstring::npos) {
        wstring str = line.substr(pos, line.size() - pos);
        pos = str.find(L">");
        str = str.substr(0, pos + 1);
        tmp_pair.second = str;
    }

    return tmp_pair;
}

/* Функция удаляет все пробелы в строке */
void ConvertToStrOp::deleteAllSpace(wstring &str) {
    int pos;
    while ((pos = str.find(L" ")) != wstring::npos)
        str.erase(pos, 1);
}

/* Просто копирует данные новый файл */
void ConvertToStrOp::handleSRT(wifstream &in, wofstream &out) {
    //считываем файл srt до конца
    wstring read_line;
    while (getline(in, read_line))
        out << read_line << L'\n';
}

/* Считытать файл TTML и записать файл SRT */
void ConvertToStrOp::handleTTML(wifstream &in, wofstream &out) {
    //считываем файл ttml до конца
    wstring read_line;
    int cur_num_title = 0;//порядковый номер sutitles
    while (getline(in, read_line)) {
        //ищем кодировку исходного файла
        if (this->ttmlEncoding_not_find && getEncoding(read_line) != L"") {
            /* Your CODE */
        }

        //ищем tickRate
        getTTP_tickRate(read_line);

        //ищем xml_language
        //getXML_Language(read_line);

        //проверяем строку
        if (checkLineTTMLonSubtitles(read_line)) {
            //если можно преобразовать, преобразуем и записываем в файл srt
            out << to_wstring(++cur_num_title) << L'\n' << getTTML2SRT(read_line) << L"\n\n";
        }
    }
}

/* Ищем кодировку, если не найдена */
wstring ConvertToStrOp::getEncoding(wstring &line) {
    //если encoding не найдено
    if (this->ttmlEncoding_not_find) {
        int pos = findLast(line, TTML_ENCODING);

        if (pos != wstring::npos) {
            wstring str = TTML_ENCODING;
            pos += str.size();
            pos++;

            str = line.substr(pos, line.size());

            pos = str.find(L'"');
            str = str.substr(0, pos);

            //нашли encoding
            this->ttmlEncoding_not_find = false;
            this->ttmlEncoding = str;

            return this->ttmlEncoding;
        }

        return L"";
    }

    return this->ttmlEncoding;
}

/* Ищем tickRate (если не найден) */
long long ConvertToStrOp::getTTP_tickRate(wstring &line) {
    //если tickRate не найдено
    if (this->ttp_tickRate_not_find) {
        int pos = findLast(line, TTML_TTP_TICKRATE);

        if (pos != wstring::npos) {
            wstring str = TTML_TTP_TICKRATE;
            pos += str.size();
            pos++;

            str = line.substr(pos, line.size());

            pos = str.find(L'"');
            str = str.substr(0, pos);

            //нашли tickRate
            this->ttp_tickRate_not_find = false;
            this->ttp_tickRate = stoll(str);

            return this->ttp_tickRate;
        }

        return -1;
    }

    return this->ttp_tickRate;
}

/* Функция ищет в строке Language (если не найден) */
wstring ConvertToStrOp::getXML_Language(wstring &line) {
    //если language не найдено
    if (this->xml_language_not_find) {
        int pos = findLast(line, TTML_XML_LANGUAGE);

        if (pos != wstring::npos) {
            wstring str = TTML_XML_LANGUAGE;
            pos += str.size();
            pos++;

            str = line.substr(pos, line.size());

            pos = str.find(L'"');
            str = str.substr(0, pos);

            //нашли language
            this->xml_language_not_find = false;
            this->xml_language = str;

            return this->xml_language;
        }

        return TTML_XML_NOT_LANG;
    }

    return this->xml_language;
}

/* Функция проверяет строку. если функция вернула true
 * значит можно преобразовать строку в srt
 * т.е. это строка, непосредственно с субтитрами!!!
 * значит можно преобразовать строку в
 * субтитры для SRT */
bool ConvertToStrOp::checkLineTTMLonSubtitles(wstring &line) {
    //строка пустая
    if (line.empty())
        return false;

    //удаялем пробелы спереди
    while (!line.empty() && line[0] == L' ')
        line.erase(0, 1);

    //удаляем пробелы в конце
    while (!line.empty() && line[line.size() - 1] == L' ')
        line.erase(line.size() - 1, 1);

    //строка дожна начинаться на "<p "
    wstring tmp_str = TTML_START_LINE;
    if (line.size() < 3)
        return false;
    if (line[0] != tmp_str[0] || line[1] != tmp_str[1] || line[2] != tmp_str[2]) {
        tmp_str = TTML_START_LINE2;
        if (line.size() < 6 || line[0] != tmp_str[0] || line[1] != tmp_str[1] || line[2] != tmp_str[2] || line[3] != tmp_str[3] || line[4] != tmp_str[4] || line[5] != tmp_str[5])
            return false;
    }

    pair<int, wstring> tmp_pair;

    //последним тегом должгл быть </p>
    tmp_pair = getTagXML(line, false);
    deleteAllSpace(tmp_pair.second);
    if (tmp_pair.first == -1 || (tmp_pair.second != TTML_STOP_LINE && tmp_pair.second != TTML_STOP_LINE2))
        return false;

    //в строке нет начала времени
    if (line.find(TTML_BEGIN) == wstring::npos)
        return false;

    //в строке нет конца времени
    if (line.find(TTML_END) == wstring::npos)
        return false;

    return true;
}

/* Функция из строки ttml делает текст srt и возвращает его */
wstring ConvertToStrOp::getTTML2SRT(wstring &line) {
    wstring res;

    //ищем начальное время
    int pos = line.find(TTML_BEGIN);
    if (pos != wstring::npos) {
        wstring str = TTML_BEGIN;
        pos += str.size();
        pos++;
        str = line.substr(pos, line.size());
        pos = str.find(L'"');
        str = str.substr(0, pos);

        res += getTimeTTML2SRT(str);
    }

    res += L" --> ";

    //ищем конечное время
    pos = line.find(TTML_END);
    if (pos != wstring::npos) {
        wstring str = TTML_END;
        pos += str.size();
        pos++;
        str = line.substr(pos, line.size());
        pos = str.find(L'"');
        str = str.substr(0, pos);

        res += getTimeTTML2SRT(str);
    }

    res += L'\n';

    //текст
    pos = line.find(L'>');
    if (pos != wstring::npos) {
        pos++;
        wstring str = line.substr(pos, line.size());
        pos = findLast(str, TTML_STOP_LINE);
        if (pos == wstring::npos)
            pos = findLast(str, TTML_STOP_LINE2);
        
        str = str.substr(0, pos);

        pair<int, wstring> tmp_pair = getTagXML(str);
        //цикл, который будем выполнять пока в строке есть теги
        while (tmp_pair.first != -1) {
            wstring tag = tmp_pair.second;

            //удаляем все пробелы
            deleteAllSpace(tag);

            //если это <br/>, то заменяем на \n (переход на новую строку)
            if (tag == TTML_BR || tag == TTML_BR2) {
                str.erase(tmp_pair.first, tmp_pair.second.size());
                str.insert(tmp_pair.first, L"\n");
            }
            //если это какой-то другой тег, то просто удаляем его
            else {
                str.erase(tmp_pair.first, tmp_pair.second.size());
            }

            //берем тег
            tmp_pair = getTagXML(str);
        }

        res += str;
    }

    return res;
}

/* Функция преобразует время ttml во время соответсвующую srt формату */
wstring ConvertToStrOp::getTimeTTML2SRT(wstring &str) {
    wstring res = str;

    //для проверки времени
    wregex rx_1(L"[0-9][0-9][:][0-9][0-9][:][0-9][0-9][,][0-9][0-9][0-9]");
    wregex rx_2(L"[0-9][0-9][:][0-9][0-9][:][0-9][0-9][.][0-9][0-9][0-9]");

    //если в конце 't'
    if (!str.empty() && str[str.size() - 1] == L't') {
        //очищаем res
        res.clear();

        //удаялем букву t
        str.erase(str.size() - 1, 1);

        //преобразуем время в милисекунды
        double timeMS = stod(str);

        //делим на tickRate
        timeMS /= this->ttp_tickRate;

        wstring tmp_str;
        //часы
        int tmp = timeMS / 3600;
        tmp_str = to_wstring(tmp);
        for (int i = tmp_str.size(); i < 2; ++i)
            res += L'0';
        res += tmp_str;
        timeMS -= (double)tmp * 3600;
        res += L':';
        //минуты
        tmp = timeMS / 60;
        tmp_str = to_wstring(tmp);
        for (int i = tmp_str.size(); i < 2; ++i)
            res += L'0';
        res += tmp_str;
        timeMS -= (double)tmp * 60;
        res += L':';
        //секунды
        tmp = timeMS;
        tmp_str = to_wstring(tmp);
        for (int i = tmp_str.size(); i < 2; ++i)
            res += L'0';
        res += tmp_str;
        timeMS -= tmp;
        res += L',';
        //млисекунды
        tmp = timeMS * 1000;
        tmp_str = to_wstring(tmp);
        for (int i = tmp_str.size(); i < 3; ++i)
            res += L'0';
        res += tmp_str;

        return res;
    }
    //если время в формате hh:mm:ss,zzz
    //значит ничего не делаем а просто возвращаем это значение!
    else if (!str.empty() 
        && (regex_match(str, rx_1) || regex_match(str, rx_2))) {
        //вместо точек устанавливаем запятые
        for (unsigned int i = 0; i < res.size(); ++i) {
            if (res[i] == L'.') {
                res.erase(i, 1);
                res.insert(i, L",");
            }
        }
        return res;
    }

    return L"Error calculate time!";
}

/* Считытать файл VTT и записать файл SRT. */
void ConvertToStrOp::handleVTT(wifstream &in, wofstream &out) {
    //считываем файл vtt до конца
    wstring read_line;
    int cur_num_title = 0;//порядковый номер sutitles
    while (getline(in, read_line)) {
        //удаялем пробелы спереди
        while (!read_line.empty() && read_line[0] == L' ')
            read_line.erase(0, 1);

        //проверяем строку на то что это время
        //для проверки времени
        wregex rx_1(L"[0-9][0-9][:][0-9][0-9][:][0-9][0-9][,][0-9][0-9][0-9]");
        wregex rx_2(L"[0-9][0-9][:][0-9][0-9][:][0-9][0-9][.][0-9][0-9][0-9]");

        bool this_time = false;

        //берем 12 символов
        if (read_line.size() > 11) {
            wstring tmp = read_line.substr(0, 12);

            //если это время
            if (regex_match(tmp, rx_1) || regex_match(tmp, rx_2)) {
                //если номер не первый
                if (cur_num_title != 0) {
                    //переход на сроку
                    out << L"\n";
                }

                //записываем номер
                out << to_wstring(++cur_num_title) << L'\n';

                //вместо точек устанавливаем запятые
                for (unsigned int i = 0; i < read_line.size(); ++i) {
                    if (read_line[i] == L'.') {
                        read_line.erase(i, 1);
                        read_line.insert(i, L",");
                    }
                }

                //записываем в файл начальное время --> конечное время
                out << read_line.substr(0, 30) << L"\n";

                //эта строка является временем
                this_time = true;
            }
        }
        //если это не время, то пишем в файл
        if (!this_time) {
            //при этом!, должно быть конечное время, это означает, что время найдено
            if (cur_num_title > 0) {
                //пополняем пока есть теги
                pair<int, wstring> pair_tmp = getTagXML(read_line);
                while (pair_tmp.first != wstring::npos) {
                    //удаляем тег
                    read_line.erase(pair_tmp.first, pair_tmp.second.length());

                    //ищем тег
                    pair_tmp = getTagXML(read_line);
                }

                if (read_line != L"") {
                    out << read_line << L"\n";
                }
            }
        }
    }
}

/* Функция проверяет строка XML(TTML) является ли субтитрами */
bool ConvertToStrOp::checkLineXML_TTMLonSubtitles(wstring& line) {
    //строка пустая
    if (line.empty())
        return false;

    //удаялем пробелы спереди
    while (!line.empty() && line[0] == L' ')
        line.erase(0, 1);

    //удаляем пробелы в конце
    while (!line.empty() && line[line.size() - 1] == L' ')
        line.erase(line.size() - 1, 1);

    //в строке нет начала времени
    if (line.find(TTML_BEGIN) == wstring::npos)
        return false;

    //в строке нет конца времени
    if (line.find(TTML_END) == wstring::npos)
        return false;

    return true;
}

/* Взять время из XML (TTML) */
wstring ConvertToStrOp::getTIMEfromXML_TTML(std::wstring& line) {
    wstring res = L"";

    //ищем начальное время
    int pos = line.find(TTML_BEGIN);
    if (pos != wstring::npos) {
        wstring str = TTML_BEGIN;
        pos += str.size();
        pos++;
        str = line.substr(pos, line.size());
        pos = str.find(L'"');
        str = str.substr(0, pos);

        res += getTimeTTML2SRT(str);
    }

    res += L" --> ";

    //ищем конечное время
    pos = line.find(TTML_END);
    if (pos != wstring::npos) {
        wstring str = TTML_END;
        pos += str.size();
        pos++;
        str = line.substr(pos, line.size());
        pos = str.find(L'"');
        str = str.substr(0, pos);

        res += getTimeTTML2SRT(str);
    }

    return res;
}