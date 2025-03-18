
#include "utils/http.h"
#include "utils/string.h"

#include <stdio.h>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#include <Winhttp.h>
#include <schannel.h>
#include <versionhelpers.h>
#endif
#include <chrono>
#include <thread>


//winhttp.lib; crypt32.lib
#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"crypt32.lib")

#define HTTP_RETRY_COUNT                (3)
#define HTTP_SEND_RECEIVE_TIMEOUT       (60000)
#define HTTP_RESOLVE_CONNECT_TIMEOUT    (30000)

using namespace utils;

static PCCERT_CONTEXT GetCertificate(HCERTSTORE cert_store, HINTERNET request) {
    SecPkgContext_IssuerListInfoEx* issuer_list = NULL;
    DWORD dwBufferSize = sizeof(SecPkgContext_IssuerListInfoEx*);
    PCCERT_CONTEXT cert = NULL;

    if (WinHttpQueryOption(request,
        WINHTTP_OPTION_CLIENT_CERT_ISSUER_LIST,
        &issuer_list,
        &dwBufferSize) == TRUE) {

        PCCERT_CHAIN_CONTEXT cert_chain = NULL;

        CERT_CHAIN_FIND_BY_ISSUER_PARA search_criteria = {0};
        //::ZeroMemory(&search_criteria, sizeof(CERT_CHAIN_FIND_BY_ISSUER_PARA));
        search_criteria.cbSize = sizeof(CERT_CHAIN_FIND_BY_ISSUER_PARA);
        search_criteria.cIssuer = issuer_list->cIssuers;
        search_criteria.rgIssuer = issuer_list->aIssuers;

        cert_chain = CertFindChainInStore(
            cert_store,
            X509_ASN_ENCODING,
            CERT_CHAIN_FIND_BY_ISSUER_CACHE_ONLY_URL_FLAG |
            // Do not perform wire download when building chains.
            CERT_CHAIN_FIND_BY_ISSUER_CACHE_ONLY_FLAG,
            // Do not search pCacheEntry->_ClientCertStore 
            // for issuer certs.
            CERT_CHAIN_FIND_BY_ISSUER,
            &search_criteria,
            NULL);

        if (cert_chain) {
            cert = cert_chain->rgpChain[0]->rgpElement[0]->pCertContext;
            CertDuplicateCertificateContext(cert);
            CertFreeCertificateChain(cert_chain);
            cert_chain = NULL;
        }

        GlobalFree(issuer_list); // Free the issuer list when done.
    }
    //else {
    //    //12019 error
    //    LOG(ERROR) << "WinHTTP: Cant query client cert issuer list: " << GetLastError();
    //}

    return cert;
}

static BOOL SetupCertificate(HINTERNET request) {
    BOOL results = FALSE;
    //MY is the store the certificate is in.
    HCERTSTORE      cert_store = CertOpenSystemStoreW(0, L"MY");
    PCCERT_CONTEXT  cert_context = NULL;

    if (cert_store) {
        cert_context = GetCertificate(cert_store, request);
        if (cert_context) {
            results = WinHttpSetOption(request,
                WINHTTP_OPTION_CLIENT_CERT_CONTEXT,
                (LPVOID)cert_context,
                sizeof(CERT_CONTEXT));
            CertFreeCertificateContext(cert_context);
        }
        CertCloseStore(cert_store, 0);

        // NOTE: Application should now resend the request.
    }

    return results;
}

static BOOL SendReceiveRequest(HINTERNET request) {
    BOOL result = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

    if (!result &&
        GetLastError() == ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED &&
        (result = SetupCertificate(request))) {
        result = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    }

    if (result)
        result = WinHttpReceiveResponse(request, NULL);

    return result;
}

static DWORD TrySendReceiveRequest(HINTERNET request) {
    DWORD err = 0; 
    BOOL result = TRUE;
    for (int retry = 0; retry < HTTP_RETRY_COUNT; ++retry) {
        result = SendReceiveRequest(request);
        err = GetLastError(); 
        //TODO: add logger
        if (!result)
            (std::cout << "[" << std::this_thread::get_id() << "] Http SendReceiveRequest error: " << err << std::endl).flush();
        if (result || (err != ERROR_WINHTTP_TIMEOUT && err != ERROR_WINHTTP_CONNECTION_ERROR))
            break;
        //TODO: add logger
        (std::cout << "Http SendReceiveRequest retrying " << retry + 1 << std::endl).flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(500 * (retry + 1)));
    }
    return err; // 0 if success
}


namespace utils {

    class HttpPrivate {

    public:

        HttpPrivate() {
            Initialize();
        }
        ~HttpPrivate() {
            Close();
        }

    public:

        Status      Open(const char* url) {
            if (session_) {
                if (ParseUrl(url)) {
                    connect_ = WinHttpConnect(session_, host_.c_str(),
                        INTERNET_DEFAULT_PORT, 0);

                    if (connect_)
                        return Status::Success;

                    return GetLastStatus(GetLastError());
                }
                else return { Status::InvalidUrl, url };
            }
            return Status::InternalError;
        }

        //Send HEAD request
        Status      Headers(HttpHeaders* headers) {
            if (!connect_)
                return Status::ConnectionError;

            if (request_)
                WinHttpCloseHandle(request_);

            // Create an HTTP request handle.
            request_ = WinHttpOpenRequest(connect_, L"HEAD", path_.c_str(),
                NULL, WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                is_secure_ ? WINHTTP_FLAG_SECURE : 0);

            if (request_ &&
                (0 == TrySendReceiveRequest(request_)) &&
                QueryHeaders(headers)) {

                return Status::Success;

            }
            return  GetLastStatus(GetLastError());
        }
        //Send GET request
        Status      Get(int64_t from = 0, int64_t to = -1, HttpHeaders* headers = nullptr) {
            if (!connect_)
                return Status::ConnectionError;
            if (request_)
                WinHttpCloseHandle(request_);

            request_ = WinHttpOpenRequest(connect_, L"GET", path_.c_str(),
                NULL, WINHTTP_NO_REFERER,
                WINHTTP_DEFAULT_ACCEPT_TYPES,
                is_secure_ ? WINHTTP_FLAG_SECURE : 0);
            auto err = GetLastError();

            //TODO: add logger
            if (!request_)
                (std::cout << "WinHttpOpenRequest error: " << err << std::endl).flush();

            std::wstringstream ss_range;

            ss_range << L"Range:bytes=" << from << L"-";
            if (to > from)
                ss_range << to - 1;
#ifdef _DEBUG    
            (std::cout << "[" << std::this_thread::get_id() << "] WinHttpOpenRequest: " << from << "->" << to << std::endl).flush();
#endif
            if (request_ &&
                WinHttpAddRequestHeaders(request_, ss_range.str().c_str(), -1, WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE) &&
                (0 == (err=TrySendReceiveRequest(request_))) &&
                (!headers || QueryHeaders(headers)))
                return Status::Success;

            return GetLastStatus(err);
        }
        //After GET request
        Status      Read(void* buffer, uint64_t* length) {
            if (!connect_)
                return Status::ConnectionError;

            if (!request_)
                return Status::InternalError;
            
            DWORD err = 0;
            BOOL result = FALSE;
            for (int retry = 0; retry < HTTP_RETRY_COUNT; ++retry) {
                result = WinHttpReadData(request_, buffer, *length, (LPDWORD)length);
                err = GetLastError();
                //TODO: add logger
                if (!result)
                    (std::cout << "[" << std::this_thread::get_id() << "] Http ReadData error: " << err << std::endl).flush();
                if (result || err != ERROR_WINHTTP_TIMEOUT)
                    break;
                //TODO: add logger
                (std::cout << "Http ReadData retrying " << retry + 1 << std::endl).flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(500 * (retry + 1)));
            }

            return result ? Status::Success : GetLastStatus(err);
        }


        //After GET request
        Status      DataAvailable(uint32_t* length) {

            if (!connect_)
                return Status::ConnectionError;

            if (!request_)
                return Status::InternalError;

            if (WinHttpQueryDataAvailable(request_, (LPDWORD)length))
                return Status::Success;

            return GetLastStatus(GetLastError());
        }

        void        Close() {
            if (request_) WinHttpCloseHandle(request_);
            if (connect_) WinHttpCloseHandle(connect_);
            if (session_) WinHttpCloseHandle(session_);

            session_ = connect_ = request_ = nullptr;
        }


        const char* url() const { return url_.c_str(); }

    private:
        void        Initialize() {
            ////Setup proxy;
            //DWORD proxy = IsWindows8Point1OrGreater() ? WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY : WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;
            //// Use WinHttpOpen to obtain a session handle.
            //session_ = WinHttpOpen(L"Mozilla/5.0 (compatible)",
            //    proxy,
            //    WINHTTP_NO_PROXY_NAME,
            //    WINHTTP_NO_PROXY_BYPASS, 0);

            //if (!session_) {
            //    (std::cout << "NOTE: Setting up IE Proxy! Error: " << GetLastError() << std::endl).flush();
            //    //Try proxy settings
            //    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxy_settings;
            //    if (WinHttpGetIEProxyConfigForCurrentUser(&proxy_settings) == TRUE
            //        && proxy_settings.lpszProxy != NULL) {
            //        session_ = WinHttpOpen(L"Mozilla/5.0 (compatible)",
            //            WINHTTP_ACCESS_TYPE_NAMED_PROXY,
            //            proxy_settings.lpszProxy,
            //            proxy_settings.lpszProxyBypass, 0);
            //    }
            //}

            session_ = WinHttpOpen(L"Mozilla/5.0 (compatible)",
                WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                WINHTTP_NO_PROXY_NAME,
                WINHTTP_NO_PROXY_BYPASS, 0);

            if (!session_) {
                (std::cout << "Cant open with automatic proxy: " << GetLastError() << std::endl).flush();
                //Try proxy settings
                WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxy_settings;
                if (WinHttpGetIEProxyConfigForCurrentUser(&proxy_settings) != TRUE
                    || proxy_settings.lpszProxy == NULL) {
                    (std::cout << "Setting up Default (Windows 7) Proxy"  << std::endl).flush();
                    session_ = WinHttpOpen(L"Mozilla/5.0 (compatible)",
                        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                        WINHTTP_NO_PROXY_NAME,
                        WINHTTP_NO_PROXY_BYPASS, 0);
                }
                else {
                    (std::cout << "Setting up IE Proxy" << std::endl).flush();
                    session_ = WinHttpOpen(L"Mozilla/5.0 (compatible)",
                        WINHTTP_ACCESS_TYPE_NAMED_PROXY,
                        proxy_settings.lpszProxy,
                        proxy_settings.lpszProxyBypass, 0);
                }
            }

            if (session_)
                // Use a 90-second timeout (1.5 times the default) for connect.
                WinHttpSetTimeouts(session_,    HTTP_RESOLVE_CONNECT_TIMEOUT, 
                                                HTTP_RESOLVE_CONNECT_TIMEOUT, 
                                                HTTP_SEND_RECEIVE_TIMEOUT, 
                                                HTTP_SEND_RECEIVE_TIMEOUT);

        }


        static Status    GetLastStatus(DWORD lastErr) {
            Status status;
            switch (lastErr) {
            case NO_ERROR:
                break;
            case ERROR_WINHTTP_INVALID_URL:
            case ERROR_WINHTTP_UNRECOGNIZED_SCHEME:
                status = { Status::InvalidUrl, "Invalid or unrecognized url" };
                break;
            case ERROR_WINHTTP_CONNECTION_ERROR:
                status = { Status::ConnectionError, "Http connection error" };
                break;
            case ERROR_WINHTTP_TIMEOUT:
                status = { Status::Timeout, "Http operation timeout" };
                break;
            case ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW:
                status = { Status::BufferOverflow, "Http buffer overflow" };
                break;
            case ERROR_NOT_ENOUGH_MEMORY:
                status = { Status::NoMemory, "Not enough memory" };
                break;
            default:
                status = { Status::InternalError, "Http internal error" };
                break;
            }
            return status;
        }

        bool    QueryHeaders(HttpHeaders* headers) {

            std::wstring    http_headers;
            DWORD   headers_size = 0;

            BOOL results = WinHttpQueryHeaders(request_, WINHTTP_QUERY_RAW_HEADERS_CRLF, NULL, WINHTTP_NO_OUTPUT_BUFFER, &headers_size, WINHTTP_NO_HEADER_INDEX);
            //BOOL results = WinHttpQueryHeaders(request_, WINHTTP_QUERY_RAW_HEADERS, NULL, WINHTTP_NO_OUTPUT_BUFFER, &headers_size, WINHTTP_NO_HEADER_INDEX);
            if (!results && (GetLastError() == ERROR_INSUFFICIENT_BUFFER)) {
                http_headers.resize(headers_size / sizeof(wchar_t));
                results = WinHttpQueryHeaders(request_, WINHTTP_QUERY_RAW_HEADERS_CRLF, NULL, &http_headers[0], &headers_size, WINHTTP_NO_HEADER_INDEX);
                //(std::wcout << "HEADER: " << http_headers << std::endl).flush();

               /* http_headers = L"HTTP/1.1 420\r\n"
                    "Cache-Control: no-store\r\n"
                    "Connection: keep-alive\r\n"
                    "Date: Mon, 22 Oct 2018 07:19:20 GMT\r\n"
                    "Pragma: no-cache\r\n"
                    "Content-Length: 0\r\n"
                    "Server: nginx\r\n"
                    "X-Netflix-Geo-Check: failed\r\n"
                    "Timing-Allow-Origin: *\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Access-Control-Expose-Headers: X-TCP-Info,X-Session-Info\r\n"
                    "X-TCP-Info: h0=4175758842;h1=4186133469;h2=524947120;h3=813861149;h4=3024468311;\r\n"
                    "X-Session-Info: addr=39.188.54.180;port=11013;argp=6.2HImd4Llh_v5Hbe9qqgGqS1yUtWdEFuhSIohmxX9wfU\r\n";*/

                ////DEBUG::
                //const char* p = UTF16ToASCII(http_headers).c_str();
                //headers->SetHeaders(p);
                //int a = 0;

                //headers->SetHeaders(UTF16ToASCII(http_headers).c_str());

                headers->SetHeaders(Utf16ToUtf8(http_headers).c_str());

            }
            return results;
        }
        bool    ParseUrl(const char* url) {
            URL_COMPONENTS      url_components;

            memset(&url_components, 0, sizeof(URL_COMPONENTS));

            // Set required component lengths to non-zero 
            // so that they are cracked.
            url_components.dwSchemeLength = (DWORD)-1;
            url_components.dwHostNameLength = (DWORD)-1;
            url_components.dwUrlPathLength = (DWORD)-1;
            url_components.dwExtraInfoLength = (DWORD)-1;
            url_components.dwStructSize = sizeof(URL_COMPONENTS);

            //std::wstring    wurl = ASCIIToUTF16(url);
            std::wstring    wurl = Utf8ToUtf16(url);

            if (WinHttpCrackUrl(wurl.c_str(), wurl.size(), 0, &url_components)) {
                is_secure_ = url_components.nScheme == INTERNET_SCHEME_HTTPS;
                host_ = std::wstring(url_components.lpszHostName, url_components.dwHostNameLength);
                path_ = std::wstring(url_components.lpszUrlPath, url_components.dwUrlPathLength) +
                    std::wstring(url_components.lpszExtraInfo, url_components.dwExtraInfoLength);
                url_ = url;
                return true;
            }
            return false;
        }

    private:
        HINTERNET       session_ = nullptr;
        HINTERNET       connect_ = nullptr;
        HINTERNET       request_ = nullptr;

        std::string     url_;
        std::wstring    host_;
        std::wstring    path_;
        bool            is_secure_ = false;


    };

    ///////////////////////////////////////////////
    // CLASS: Http

    Http::Http() 
        : private_(std::make_unique<HttpPrivate>()) {
    }
    Http::~Http() {
    }
    Status Http::Open(const char * url) {
        return private_->Open(url);
    }

    const char* Http::url() const {
        return private_->url();
    }
    Status Http::Headers(HttpHeaders * headers) {
        return private_->Headers(headers);
    }
    Status Http::Get(int64_t from, int64_t to, HttpHeaders * headers) {
        return private_->Get(from, to, headers);
    }
    Status Http::Read(void * buffer, uint64_t * length) {
        return private_->Read(buffer, length);
    }
    Status Http::DataAvailable(uint32_t * length) {
        return private_->DataAvailable(length);
    }
    void Http::Close() {
        private_->Close();
    }
}
