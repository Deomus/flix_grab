#include "addtags_op.h"
#include "windows.h"

#include <regex>

#include <minwinbase.h>
#include <processthreadsapi.h>
#include <fileapi.h>
#include <processenv.h>
#include <WinBase.h>

using namespace recode;
using namespace std;

std::mutex   AddTagsOp::mutex_;

// input url
AddTagsOp::AddTagsOp() {}

AddTagsOp::~AddTagsOp() {
    //
}

void AddTagsOp::Add(const wstring& out) {
    streams_.push_back({ out });
}

Status AddTagsOp::Do(OperationListener* listener) {
    //std::lock_guard<std::mutex> lock(mutex_);
    Status status;

    for (auto& s : streams_) {
        cout << "AddTagsOp::Do() " << s << endl;

        std::wstring fileType = s.substr(s.find_last_of(L".") + 1);
        std::transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);
        std::wcout << "audio filetype: " << fileType << std::endl;
        
        std::string imageFileType = id3tag_image_path_.substr(id3tag_image_path_.find_last_of(".") + 1);
        std::transform(imageFileType.begin(), imageFileType.end(), imageFileType.begin(), ::tolower);
        std::cout << "image filetype: " << imageFileType << std::endl;

        ImageFile imageFile(id3tag_image_path_.c_str());
        TagLib::ByteVector imageData = imageFile.data();

        std::string mimeType;
        if (imageFileType == "jpg"
            || imageFileType == "jpeg"
            || imageFileType == "tmp") {
            mimeType = "image/jpeg";
        }
        else if (imageFileType == "png") {
            mimeType = "image/png";
        }
        std::cout << "image mimetype " << mimeType << std::endl;

        if (fileType == L"m4a"
            || fileType == L"aac") {

            TagLib::MP4::File audioFile(s.c_str());
            TagLib::MP4::Tag* tag = audioFile.tag();
            //TagLib::MP4::ItemMap map = tag->itemMap();

            if (!id3tag_image_path_.empty()) {
                TagLib::MP4::CoverArt::Format coverArtFormat;

                if (imageFileType == "jpg"
                    || imageFileType == "jpeg"
                    || imageFileType == "tmp") {
                    coverArtFormat = TagLib::MP4::CoverArt::JPEG;
                }
                else if (imageFileType == "png") {
                    coverArtFormat = TagLib::MP4::CoverArt::PNG;
                }

                TagLib::MP4::CoverArt coverArt(coverArtFormat, imageData);
                TagLib::MP4::CoverArtList coverArtList = tag->item("covr").toCoverArtList();

                coverArtList.clear();
                coverArtList.append(coverArt);
                tag->setItem("covr", coverArtList);
            }
            if (!id3tag_title_.empty())
                tag->setTitle(id3tag_title_.c_str());
            if (!id3tag_artist_.empty())
                tag->setArtist(id3tag_artist_.c_str());
            if (!id3tag_album_.empty())
                tag->setAlbum(id3tag_album_.c_str());
            if (!id3tag_comment_.empty())
                tag->setComment(id3tag_comment_.c_str());
            if (!id3tag_genre_.empty())
                tag->setGenre(id3tag_genre_.c_str());
            if (id3tag_year_ > 0)
                tag->setYear(id3tag_year_);
            if (id3tag_track_ > 0)
                tag->setTrack(id3tag_track_);

            if (!audioFile.save())
                return Status(error::Code::WRITE_METADATA_ERROR, "Writing metadata error");
        }
        else if (fileType == L"mp3") {
            TagLib::MPEG::File audioFile(s.c_str());

            std::cout << "ape " << audioFile.hasAPETag() << std::endl;
            std::cout << "id3v1 " << audioFile.hasID3v1Tag() << std::endl;
            std::cout << "id3v2 " << audioFile.hasID3v2Tag() << std::endl;

            TagLib::ID3v2::Tag* tag = audioFile.ID3v2Tag(true);
            if (!id3tag_image_path_.empty()) {
                TagLib::ID3v2::AttachedPictureFrame* frame = new TagLib::ID3v2::AttachedPictureFrame;
                frame->setMimeType(mimeType);
                frame->setPicture(imageData);
                frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);
                // remove the existing album cover & add the new one
                tag->removeFrames("APIC");
                tag->addFrame(frame);
            }
            if (!id3tag_title_.empty())
                tag->setTitle(id3tag_title_.c_str());
            if (!id3tag_artist_.empty())
                tag->setArtist(id3tag_artist_.c_str());
            if (!id3tag_album_.empty())
                tag->setAlbum(id3tag_album_.c_str());
            if (!id3tag_comment_.empty())
                tag->setComment(id3tag_comment_.c_str());
            if (!id3tag_genre_.empty())
                tag->setGenre(id3tag_genre_.c_str());
            if (id3tag_year_ > 0)
                tag->setYear(id3tag_year_);
            if (id3tag_track_ > 0)
                tag->setTrack(id3tag_track_);

            TagLib::Tag::duplicate(tag, audioFile.ID3v1Tag(true), true);

            if (!audioFile.save(TagLib::MPEG::File::ID3v1 | TagLib::MPEG::File::ID3v2))
                return Status(error::Code::WRITE_METADATA_ERROR, "Writing metadata error");
        }
        else if (fileType == L"ogg") {
            TagLib::Ogg::Vorbis::File audioFile(s.c_str());
            TagLib::Ogg::XiphComment* tag = audioFile.tag();
            if (!id3tag_image_path_.empty()) {
                TagLib::FLAC::Picture* pic = new TagLib::FLAC::Picture();
                pic->setMimeType(mimeType);
                pic->setData(imageData);
                pic->setType(TagLib::FLAC::Picture::FrontCover);
                pic->setDescription("Front Cover");

                tag->removeAllPictures();
                tag->addPicture(pic);
            }

            if (!id3tag_title_.empty())
                tag->setTitle(id3tag_title_.c_str());
            if (!id3tag_artist_.empty())
                tag->setArtist(id3tag_artist_.c_str());
            if (!id3tag_album_.empty())
                tag->setAlbum(id3tag_album_.c_str());
            if (!id3tag_comment_.empty())
                tag->setComment(id3tag_comment_.c_str());
            if (!id3tag_genre_.empty())
                tag->setGenre(id3tag_genre_.c_str());
            if (id3tag_year_ > 0)
                tag->setYear(id3tag_year_);
            if (id3tag_track_ > 0)
                tag->setTrack(id3tag_track_);

            if (!audioFile.save())
                return Status(error::Code::WRITE_METADATA_ERROR, "Writing metadata error");
        }
        else {
            std::cout << fileType << " is unsupported." << std::endl;
        }
    }

    //if (listener)
    //    listener->OnProgress(1);

    return status;
}

void AddTagsOp::Cancel() {
    Operation::Cancel();
}

void AddTagsOp::set_metadata_title(const wstring& metadata_title) {
    id3tag_title_ = metadata_title;
}
void AddTagsOp::set_metadata_artist(const wstring& metadata_artist) {
    id3tag_artist_ = metadata_artist;
}
void AddTagsOp::set_metadata_album(const wstring& metadata_album) {
    id3tag_album_ = metadata_album;
}
void AddTagsOp::set_metadata_album_artist(const wstring& metadata_album_artist) {
    id3tag_album_artist_ = metadata_album_artist;
}
void AddTagsOp::set_metadata_track(const int metadata_track) {
    id3tag_track_ = metadata_track;
}
void AddTagsOp::set_metadata_year(const int metadata_year) {
    id3tag_year_ = metadata_year;
}
void AddTagsOp::set_metadata_comment(const wstring& metadata_comment) {
    id3tag_comment_ = metadata_comment;
}
void AddTagsOp::set_metadata_genre(const wstring& metadata_genre) {
    id3tag_genre_ = metadata_genre;
}
void AddTagsOp::set_metadata_image_path(const string& metadata_image_path) {
    id3tag_image_path_ = metadata_image_path;
}

void AddTagsOp::runFpcalc(const std::wstring& stream, std::string* fingerprint, std::string* duration) {
    wchar_t szPath[MAX_PATH + 86];

    GetModuleFileNameW(NULL, szPath, MAX_PATH + 85); // 85 is the marker for process_injecto
    std::wstring base_path = szPath; // get current module path name
    std::wstring::size_type pos = base_path.find_last_of(L"\\/");
    base_path = base_path.substr(0, pos);
    std::wstring decryptCmd = base_path + L"\\fpcalc.exe";

    //std::wstring target_file = base_path + L"\\test.m4a";
    std::wstring decryptArgs = L" \"" + stream + L"\"";

    std::wstring arg(decryptCmd + decryptArgs);
    std::wcout << arg << std::endl;

    std::wstring workingDir = L"./";

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        throw std::runtime_error("Error StdoutRd CreatePipe");
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        throw std::runtime_error("Error Stdout SetHandleInformation");

    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = g_hChildStd_OUT_Wr;
    si.hStdOutput = g_hChildStd_OUT_Wr;
    //si.hStdInput = g_hChildStd_IN_Rd;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessW(NULL,
        &arg[0],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to TRUE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        workingDir.c_str(),  // starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    )) {
        CloseHandle(g_hChildStd_OUT_Wr);
        //CloseHandle(g_hChildStd_ERR_Wr);

        for (;;) {
            auto dwRes = WaitForSingleObject(pi.hProcess, 1000);
            if (dwRes == WAIT_FAILED)
                break; // error

            DWORD dwRead, dwWritten;
            const int BUFSIZE = 10000;
            WCHAR chBuf[BUFSIZE + 1] = { 0 };

            auto bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            if (!bSuccess || dwRead == 0)
                break;

            std::string output((const char*)chBuf);
            std::regex expression("FILE=(.*)\r\nDURATION=(.*)\r\nFINGERPRINT=(.*)\r\n");
            std::smatch match;

            if (std::regex_search(output, match, expression)) {
                duration->assign(match[2]);
                fingerprint->assign(match[3]);
            }

            //HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            //WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL);
            //std::cout << "fpcalc (" << dwRead << "):" << chBuf << std::endl;

            if (dwRes == WAIT_OBJECT_0)
                break; // process closed
        }
    }
    else {
        std::cout << "Error starting fpcalc: " << GetLastError() << std::endl;
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
