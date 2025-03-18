#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "bitstream.h"
#include "base32.h"
#include "base64.h"

#include <map>

#ifdef _WIN32
#include <winhttp.h>
#include <tchar.h>
#include <versionhelpers.h>
#else
#include <curl/curl.h>
#endif

using namespace std;

class KeyGeneratorImpl {
public:
    KeyGeneratorImpl()
    {
        Init();
    }

    KeyGeneratorImpl(const LicenseTemplateImpl * templ)
    {
        Init();

        SetKeyTemplate(templ);
    }

    ~KeyGeneratorImpl()
    {
        if (m_rawKey) delete m_rawKey;
        if (m_validationData) delete m_validationData;
    }

    void SetKeyTemplate(const LicenseTemplateImpl * templ)
    {
        m_keyTemplate = templ;

        if (m_rawKey) delete m_rawKey;
        if (m_validationData) delete m_validationData;

        m_rawKey = new BitStream(m_keyTemplate->m_numGroups * m_keyTemplate->m_charsPerGroup * m_keyTemplate->m_keyEncoding);
        m_rawKey->Clear();
        m_validationData = new BitStream(m_keyTemplate->m_validationDataSize);
        m_validationData->Clear();
    }

    void SetKeyData(const char_t * fieldName, const void * buf, int len)
    {
        int sizeInBits;
        void * buffer = m_rawKey->GetBuffer(&sizeInBits);
        m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_keyDataMap->Set(fieldName, buf, len);
    }

    void SetKeyData(const char_t * fieldName, const char_t * data)
    {
        int sizeInBits;
        void * buffer = m_rawKey->GetBuffer(&sizeInBits);
        m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_keyDataMap->Set(fieldName, data);
    }

    void SetKeyData(const char_t * fieldName, int data)
    {
        int sizeInBits;
        void * buffer = m_rawKey->GetBuffer(&sizeInBits);
        m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_keyDataMap->Set(fieldName, data);
    }

    void SetKeyData(const char_t * fieldName, int year, int month, int day)
    {
        int sizeInBits;
        void * buffer = m_rawKey->GetBuffer(&sizeInBits);
        m_keyTemplate->m_keyDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_keyDataMap->Set(fieldName, year, month, day);
    }

    void SetValidationData(const char_t * fieldName, const void * buf, int len)
    {
        int sizeInBits;
        void * buffer = m_validationData->GetBuffer(&sizeInBits);
        m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_validationDataMap->Set(fieldName, buf, len);
    }

    void SetValidationData(const char_t * fieldName, const char_t * data)
    {
        int sizeInBits;
        void * buffer = m_validationData->GetBuffer(&sizeInBits);
        m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_validationDataMap->Set(fieldName, data);
    }

    void SetValidationData(const char_t * fieldName, int data)
    {
        int sizeInBits;
        void * buffer = m_validationData->GetBuffer(&sizeInBits);
        m_keyTemplate->m_validationDataMap->Attach(buffer, sizeInBits);
        m_keyTemplate->m_validationDataMap->Set(fieldName, data);
    }

#ifndef _WIN32
    static size_t
    CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    {
        KeyGeneratorImpl * pthis = (KeyGeneratorImpl *)userp;
        size_t realsize = size * nmemb;

        if (realsize > 1024 || pthis->m_keyBuffer.length() > 1024)
            return realsize - 1; // signal error

        if (realsize > 0)
            pthis->m_keyBuffer.append((char *)contents, realsize);

        return realsize;
    }
#endif

    const char_t * GenerateKey()
    {
        BitStream signedData;
        BitStream signature;
        ECC::Signer signer;
        int sigLen, sigLenBits;

        if (m_keyTemplate->m_signingKey == NULL)
        {
            USES_CONVERSION;

            string_t activationQuery(_T("Activate.ashx?"));

            //if (activationQuery[m_keyTemplate->m_signingServiceUrl.length() - 1] != '/')
            //  activationQuery.append(1, '/');
            //activationQuery.append(_T("Activate.ashx?"));

            BitStream bits;
            bits.Attach(m_validationData->GetBuffer(), m_validationData->GetSize());
            unsigned char * buf = (unsigned char *)_alloca(1 + ((m_validationData->GetSize() + 7) >> 3));

            void * enumHandle = NULL;
            const char_t * name;
            BitStruct::FIELD_TYPE type;
            int size, offset;

            while (m_keyTemplate->m_validationDataMap->EnumFields(&enumHandle, &name, &type, &size, &offset))
            {
                bits.Seek(offset);
                bits.Read(buf, size);
                
                activationQuery.append(name);
                activationQuery.append(_T("="));

                if ((int)type == FIELD_TYPE_STRING)
                {
                    buf[(size + 7) >> 3] = '\0';
                    activationQuery.append(A2T((char *)buf));
                } else
                {
                    BASE64 base64;

                    std::string base64Value(base64.encode(buf, (size + 7) >> 3, true));
                    
                    StrReplaceA(base64Value, "+", "%2B");
                    StrReplaceA(base64Value, "/", "%2F");
                    StrReplaceA(base64Value, "=", "%3D");

                    activationQuery.append(A2T(base64Value.c_str()));
                }

                activationQuery.append(_T("&"));
            }

            if (m_keyTemplate->m_signingServiceParameters.length() > 0)
            {
                activationQuery.append(m_keyTemplate->m_signingServiceParameters);
                activationQuery.append(_T("&"));
            }

            activationQuery.erase(activationQuery.length() - 1, 1);

            m_keyBuffer.clear();

#ifdef _WIN32
            char_t  errorMessage[1024];
   //         //Setup proxy;
   //         DWORD proxy = IsWindows8Point1OrGreater() ? WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY : WINHTTP_ACCESS_TYPE_DEFAULT_PROXY;
            //HINTERNET internet = WinHttpOpen(L"FreeGrabApp Licensing SDK 2.0", proxy, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0L);

            HINTERNET internet = WinHttpOpen(L"FreeGrabApp Licensing SDK 2.0",
                WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                WINHTTP_NO_PROXY_NAME,
                WINHTTP_NO_PROXY_BYPASS, 0L);

            if (!internet) {
                //Try proxy settings
                WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxy_settings;
                if (WinHttpGetIEProxyConfigForCurrentUser(&proxy_settings) != TRUE
                    || proxy_settings.lpszProxy == NULL) {
                    internet = WinHttpOpen(L"FreeGrabApp Licensing SDK 2.0",
                        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                        WINHTTP_NO_PROXY_NAME,
                        WINHTTP_NO_PROXY_BYPASS, 0L);
                }
                else {
                    internet = WinHttpOpen(L"FreeGrabApp Licensing SDK 2.0",
                        WINHTTP_ACCESS_TYPE_NAMED_PROXY,
                        proxy_settings.lpszProxy,
                        proxy_settings.lpszProxyBypass, 0L);
                }
            }

            if (internet == NULL) 
            {
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (1) code (%d)", GetLastError()));
                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (2)"));
            }

            //Get URL Components
            URL_COMPONENTS urlComponents;

            ZeroMemory(&urlComponents, sizeof(urlComponents));
            urlComponents.dwStructSize = sizeof(urlComponents);

            urlComponents.dwHostNameLength = (DWORD)-1;
            urlComponents.dwUrlPathLength = (DWORD)-1;

            if (!WinHttpCrackUrl(T2W(m_keyTemplate->m_signingServiceUrl.c_str()), m_keyTemplate->m_signingServiceUrl.length(), 0L, &urlComponents)) {
                wprintf(_T("Parse URL %s Error %d"), T2W(m_keyTemplate->m_signingServiceUrl.c_str()), GetLastError());
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (2) code (%d)", GetLastError()));
                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (1)"));
            }

            std::wstring hostName(urlComponents.lpszHostName, urlComponents.dwHostNameLength);

            HINTERNET connection = WinHttpConnect(internet, hostName.c_str(), urlComponents.nPort, 0L);
            if (connection == NULL)
            {
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (3) code (%d)", GetLastError()));
                WinHttpCloseHandle(internet);
                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (3)"));
            }

            activationQuery.insert(0, W2T(urlComponents.lpszUrlPath), urlComponents.dwUrlPathLength);

            HINTERNET request = WinHttpOpenRequest(connection, L"GET", T2W(activationQuery.c_str()), NULL, NULL, NULL, WINHTTP_FLAG_BYPASS_PROXY_CACHE);
            if (!request)
            {
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (4) code (%d)", GetLastError()));

                WinHttpCloseHandle(connection);
                WinHttpCloseHandle(internet);
                
                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (4)"));
            }

            if (!WinHttpSendRequest(request, NULL, 0L, NULL, 0L, 0L, 0L))
            {
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (5) code (%d)", GetLastError()));

                WinHttpCloseHandle(request);
                WinHttpCloseHandle(connection);
                WinHttpCloseHandle(internet);

                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (5)"));
            }

            if (!WinHttpReceiveResponse(request, NULL))
            {
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (6) code (%d)", GetLastError()));

                WinHttpCloseHandle(request);
                WinHttpCloseHandle(connection);
                WinHttpCloseHandle(internet);
                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (6)"));
            }

            DWORD status;
            DWORD statusSize = sizeof(DWORD);

            if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &statusSize, WINHTTP_NO_HEADER_INDEX))
            {
                t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (7) code (%d)", GetLastError()));

                WinHttpCloseHandle(request);
                WinHttpCloseHandle(connection);
                WinHttpCloseHandle(internet);
                //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (7)"));
                throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
            }

            if (status != HTTP_STATUS_OK)
            {
                WCHAR buffer[1024];
                DWORD bufferSize = 1024;

                if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_TEXT, WINHTTP_HEADER_NAME_BY_INDEX, buffer, &bufferSize, WINHTTP_NO_HEADER_INDEX))
                {
                    t_snprintf(errorMessage, sizeof(errorMessage) / sizeof(char_t), _T("networking error (8) code (%d)", GetLastError()));
                    WinHttpCloseHandle(request);
                    WinHttpCloseHandle(connection);
                    WinHttpCloseHandle(internet);

                    //throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("networking error (8)"));
                    throw new KeyExceptionImpl(STATUS_NET_ERROR, errorMessage);
                }

                switch (status) {
                case HTTP_STATUS_FORBIDDEN:
                    throw new KeyExceptionImpl(STATUS_INVALID_LICENSE_KEY, W2T(buffer));

                case HTTP_STATUS_SERVER_ERROR:
                    throw new KeyExceptionImpl(STATUS_INVALID_SIGNATURE_SIZE, W2T(buffer));

                case HTTP_STATUS_BAD_REQUEST:
                    throw new KeyExceptionImpl(STATUS_INVALID_HARDWARE_ID, W2T(buffer));

                default:
                    throw new KeyExceptionImpl(STATUS_NET_ERROR, W2T(buffer));
                }
            }

            DWORD dataSize, count;
            char buffer[0x100];
            

            do {
                if (!WinHttpQueryDataAvailable(request, &dataSize))
                    break;

                if (size > 0x100)
                    size = 0x100;

                if (WinHttpReadData(request, buffer, dataSize, &count))
                    m_keyBuffer.append(buffer, count);

            } while (dataSize > 0);

            WinHttpCloseHandle(request);
            WinHttpCloseHandle(connection);
            WinHttpCloseHandle(internet);
#else
            CURL *curl;
            CURLcode res;
 
            if ((curl = curl_easy_init()) == NULL)
                throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("cURL initalization failed"));

            if (*(m_keyTemplate->m_signingServiceUrl.rbegin()) != '/')
                activationQuery.insert(0, _T("/"));

            activationQuery.insert(0, m_keyTemplate->m_signingServiceUrl);

            curl_easy_setopt(curl, CURLOPT_URL, activationQuery.c_str());
            
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
 
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

            res = curl_easy_perform(curl);
            
            if(res != CURLE_OK)
              throw new KeyExceptionImpl(STATUS_NET_ERROR, curl_easy_strerror(res));
 
            curl_easy_cleanup(curl);
#endif

            m_key = A2T(m_keyBuffer.c_str());

            return m_key.c_str();
        }

        // signed data is license key data followed by validation data
        signedData.Create(m_keyTemplate->m_dataSize + m_keyTemplate->m_validationDataSize);

        if (m_keyTemplate->m_dataSize)
            signedData.Write(m_rawKey->GetBuffer(), m_keyTemplate->m_dataSize);

        if (m_keyTemplate->m_validationDataSize)
            signedData.Write(m_validationData->GetBuffer(), m_keyTemplate->m_validationDataSize);
        
        signedData.ZeroPadToNextByte();

        // create a bit stream to hold the signature
        signature.Create(m_keyTemplate->m_signatureSize);

        // sign data

        // we use a different algorithm than ECDSA when the signature size must be smaller than twice the key size
        if (m_keyTemplate->m_signatureSize < (m_keyTemplate->m_signatureKeySize << 1))
            signer.SetHashSize(m_keyTemplate->m_signatureSize - m_keyTemplate->m_signatureKeySize);
        else
            signer.SetHashSize(0);

        signer.SetPrivateKey(m_keyTemplate->m_signingKey);

        signer.Sign(signedData.GetBuffer(), (signedData.GetSize() + 7) >> 3, 
                    signature.GetBuffer(), &sigLen, &sigLenBits);

        signature.ReleaseBuffer(sigLenBits);

        // write the signature at the end of key data
        m_rawKey->Seek(m_keyTemplate->m_dataSize);
        m_rawKey->Write(signature.GetBuffer(), m_keyTemplate->m_signatureSize);

        // text-encode the license key
        EncodeKey();

        // success
        return m_key.c_str();
    }

    void EncodeKey()
    {
        USES_CONVERSION;
        char * buf = NULL;

        // set all bits to 0 until the next byte boundary
        //m_rawKey.ZeroPadToNextByte();

        // reverse last byte from the stream, 
        // so that we will not lose significant bits in the padless, truncated BAS32/64 encoding
        unsigned char * lastBytePtr = (unsigned char *)m_rawKey->GetBuffer() + ((m_rawKey->GetSize() + 7) >> 3) - 1;
        *lastBytePtr = (unsigned char)(((*lastBytePtr * 0x0802LU & 0x22110LU) | (*lastBytePtr * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
        
        switch (m_keyTemplate->m_keyEncoding)
        {
        case ENCODING_BASE32X:
            {
                BASE32 base32;

                buf = base32.encode((unsigned char *)m_rawKey->GetBuffer(), (m_rawKey->GetSize() + 7) >> 3, true);
            }
            break;

        case ENCODING_BASE64X:
            {
                BASE64 base64;
                buf = base64.encode((unsigned char *)m_rawKey->GetBuffer(), (m_rawKey->GetSize() + 7) >> 3, false);
            }
            break;

        default:
            throw new KeyExceptionImpl(STATUS_INVALID_KEY_ENCODING);
        }

        if (!buf)
            throw new KeyExceptionImpl(STATUS_OUT_OF_MEMORY);

        m_key = A2T(buf);
        if ((int)m_key.length() > m_keyTemplate->m_numGroups * m_keyTemplate->m_charsPerGroup)
            m_key.resize(m_keyTemplate->m_numGroups * m_keyTemplate->m_charsPerGroup);

        free(buf);

        int i;
        int insertPos;
        // separate character groups
        for (i = 0, insertPos = m_keyTemplate->m_charsPerGroup; i < m_keyTemplate->m_numGroups - 1; i++)
        {
            m_key.insert(insertPos, m_keyTemplate->m_groupSeparator);
            insertPos += (unsigned)( m_keyTemplate->m_groupSeparator.length() + m_keyTemplate->m_charsPerGroup );
        }

        // add header and footer (if any)
        if (!m_keyTemplate->m_header.empty())
            m_key.insert(0, m_keyTemplate->m_header + T_STR("\r\n") );

        if (!m_keyTemplate->m_footer.empty())
            m_key.append(T_STR("\r\n") + m_keyTemplate->m_footer);
    }

    void StrReplaceA(std::string& str, const std::string& oldStr, const std::string& newStr)
    {
      size_t pos = 0;
      while((pos = str.find(oldStr, pos)) != std::string::npos)
      {
         str.replace(pos, oldStr.length(), newStr);
         pos += newStr.length();
      }
    }

private:
    void Init()
    {
        m_rawKey = NULL;
        m_validationData = NULL;
        m_keyTemplate = NULL;
    }

    string m_keyBuffer;
    string_t m_key;
    BitStream * m_rawKey;
    BitStream * m_validationData;
    const LicenseTemplateImpl * m_keyTemplate;
};

#endif
