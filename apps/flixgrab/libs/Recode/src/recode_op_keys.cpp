#include "recode_op_keys.h"
#include <minwinbase.h>
#include <processthreadsapi.h>
#include <fileapi.h>
#include <processenv.h>
#include <WinBase.h>
#include <utils/string.h>

using namespace recode;

RecodeOpKeys::RecodeOpKeys(const std::string& video_url, const std::string& output_file, const std::vector<std::string>& keys)
    : video_url_(video_url)
    , output_file_(output_file)
    , keys_(keys) {
    std::cout << "RecodeOpKeys() " << output_file << std::endl;
}

RecodeOpKeys::~RecodeOpKeys() {}

Status RecodeOpKeys::Do(OperationListener* listener) {
    auto start_clock = std::chrono::steady_clock::now();

    progress_listener_ = listener;

    //runMp4Decrypt();

    decryptWithKeys(keys_, video_url_, output_file_);

    progress_listener_ = nullptr;

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_clock).count();
    std::cout << "Decryption duration (secs): " << seconds << ", status: " << (status_.ok()? "ok": status_.error_message() + " " + std::to_string(status_.subsystem_code())) << std::endl;
    return status_;
}

/*
void RecodeOpKeys::runMp4Decrypt() {
    char szPath[MAX_PATH+86];

    GetModuleFileNameA(NULL, szPath, MAX_PATH + 85); // 85 is the marker for process_injecto
    std::string base_path = szPath; // get current module path name
    std::string::size_type pos = base_path.find_last_of("\\/");
    base_path = base_path.substr(0, pos);
    std::string decryptCmd = base_path + "\\mp4decrypt.exe";
    
    std::string decryptArgs;
    for (const auto k : keys_) {
        decryptArgs += " --key " + k;
    }
    decryptArgs += " \"" + video_url_ + "\"";
    decryptArgs += " \"" + output_file_ + "\"";

    std::string arg(decryptCmd + decryptArgs);
    std::cout << arg << std::endl;

    std::string workingDir = "./";

    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
        throw std::runtime_error("Error StdoutRd CreatePipe");
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
        throw std::runtime_error("Error Stdout SetHandleInformation");
    //if (!CreatePipe(&g_hChildStd_ERR_Rd, &g_hChildStd_ERR_Wr, &saAttr, 0))
    //  throw runtime_error("Error StdERR Rd CreatePipe");
    //if (!SetHandleInformation(g_hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
    //  throw runtime_error("Error StdERR SetHandleInformation");


    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = g_hChildStd_OUT_Wr;
    si.hStdOutput = g_hChildStd_OUT_Wr;
    //si.hStdInput = g_hChildStd_IN_Rd;
    si.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::string filename;

    if (CreateProcessA(NULL,
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
            if (dwRes == WAIT_FAILED || is_cancelled())
                break; // error

            DWORD dwRead, dwWritten;
            const int BUFSIZE = 10000;
            CHAR chBuf[BUFSIZE] = { 0 };

            // read ERR std stream if any
            //auto bSuccess = ReadFile(g_hChildStd_ERR_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            //if (bSuccess && dwRead > 0)
            //{
            //  HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            //  WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL);
            //  logger.Error(chBuf);
            //}
            // read OUTPUT std stream:
            auto bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
            if (!bSuccess || dwRead == 0)
                break;

            HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
            WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, NULL);
            std::cout << "mp4decryptor (" << dwRead << "):" << chBuf << std::endl;

            std::stringstream buf(chBuf);
            std::string line;
            while (getline(buf, line, '\r')) {
                auto pos = line.find('%');
                if (pos != std::string::npos) {
                    double p = min(line.size() / 100.0, 100);
                    progress_listener_->OnProgress(p);
                }
            }

            if (dwRes == WAIT_OBJECT_0)
                break; // process closed
        }
    }
    else {
        std::cout << "Error starting mp4decrypt: " << GetLastError() << std::endl;
    }


    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}*/

extern "C" {
    typedef int (*ProgressListenerFunc)(unsigned int, unsigned int, void* p);
    __declspec(dllimport) int bento4decrypt(int argc, char** argv, ProgressListenerFunc* progress_listener, void* p);
}


bool RecodeOpKeys::decryptWithKeys(const std::vector<std::string>& keys, const std::string& input_file,
    const std::string& output_file) {

    char szPath[MAX_PATH + 86];

    GetModuleFileNameA(NULL, szPath, MAX_PATH + 85); // 85 is the marker for process_injecto
    std::string base_path = szPath; // get current module path name
    std::string::size_type pos = base_path.find_last_of("\\/");
    base_path = base_path.substr(0, pos);
    std::string dllPath = base_path + "\\Mp4Decrypt.dll";

    auto h = ::LoadLibraryA(dllPath.c_str());
    if (!h) {
        auto err = GetLastError();
        status_ = Status(recode::error::Code::CDM_ERROR, "Error loading mp4decrypt.dll from " + dllPath, err);
        return false;
    }

    decltype(&bento4decrypt) decrypt = reinterpret_cast<decltype(&bento4decrypt)>(GetProcAddress(h, "bento4decrypt"));

    if (!decrypt) {
        auto err = GetLastError();
        status_ = Status(recode::error::Code::CDM_ERROR, "Error loading bento4decrypt", err);
        return false;
    }
    // mp4decrypt.exe --key 634aa8128b76498781b37438d4fb1c1a:064aab7fafc7718330b705703f517249 in2.mp4  output2.mp4
    std::vector<char*> args;
    args.push_back("bento4decrypt");
    args.push_back("--show-progress");
    for (auto& k : keys) {
        args.push_back("--key");
        args.emplace_back((char*)k.c_str());
    }
    auto inputFile = input_file;
    auto outputFile = output_file;
    args.push_back(inputFile.data());
    args.push_back(outputFile.data());
    args.push_back(NULL);

    ProgressListenerFunc progress_listener = [](unsigned int step, unsigned int total, void* p) {
        auto ptrs = (std::tuple<Operation*, recode::OperationListener*>*) p;
        auto operation = std::get<0>(*ptrs); // unpack pointers
        auto listner = std::get<1>(*ptrs);

        if (listner != nullptr)
            listner->OnProgress((double)step / total);
        
        if (operation == nullptr || operation->is_cancelled())
            return -1;

        return 0;
    };

    std::cout << "mp4decr " << args[2] << args[3] << " " << args[5] << std::endl;
    
    std::tuple<Operation*, recode::OperationListener*> ptrs{ (Operation*)this, progress_listener_}; // pack two pointers 
    
    if (decrypt
        && decrypt(args.size() - 1, args.data(), &progress_listener, (void*)&ptrs) == 0)
        return true;
    
    status_ = Status(recode::error::Code::CDM_ERROR, "failed to call mp4decrypt");
    //auto uloadRes = FreeLibrary(h);
    return false;
}
