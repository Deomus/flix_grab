#include "precomp.h"
#include "download.h"
#include "uniconv.h"
#include "except.h"

#ifdef _WIN32
#include <winhttp.h>
#else
#include <curl/curl.h>
#endif

#ifndef _WIN32
	class CurlWriteBuffer
	{
	public:
		CurlWriteBuffer(void * buffer, size_t size)
		{
			this->buffer = (char *)buffer;
			this->size = size;
			offset = 0;
		}

		int Append(void * buffer, size_t size)
		{
			if (this->offset + size > this->size)
				size = this->size - offset;

			if (size > 0)
			{
				memcpy(this->buffer + offset, buffer, size);
				offset += size;
			}

			return size;
		}

		static size_t
		Callback(void *contents, size_t size, size_t nmemb, void *userp)
		{
			return ((CurlWriteBuffer *)userp)->Append(contents, size * nmemb);
		}

		size_t offset;
		size_t size;
		char * buffer;
	};

#endif

void UrlDownloadToString(const char_t * url, char * buffer, int * len)
{
#ifdef _WIN32
	USES_CONVERSION;
	HINTERNET internet = NULL,
		      connection = NULL,
			  request = NULL;

	HRESULT result = ERROR_SUCCESS;

	URL_COMPONENTS urlComponents;

	do {
		ZeroMemory(&urlComponents, sizeof(urlComponents));
		urlComponents.dwStructSize = sizeof(urlComponents);

		urlComponents.dwHostNameLength  = (DWORD)-1;
		urlComponents.dwUrlPathLength = (DWORD)-1;

		const wchar_t * urlw = T2W(url);

		if (!WinHttpCrackUrl(urlw, wcslen(urlw), 0L, &urlComponents))
		{
			result = GetLastError();
			break;
		}

		LPWSTR hostName = (LPWSTR)_alloca((1 + urlComponents.dwHostNameLength) * sizeof (WCHAR));
		wcsncpy_s(hostName, 1 + urlComponents.dwHostNameLength, urlComponents.lpszHostName, _TRUNCATE);
		hostName[urlComponents.dwHostNameLength] = '\0';

		internet = WinHttpOpen(L"Downloader", 0L, NULL, NULL, 0L);
		if (internet == NULL)
		{
			result = GetLastError();
			break;
		}

		connection = WinHttpConnect(internet, hostName, urlComponents.nPort, 0L);
		if (connection == NULL)
		{
			result = GetLastError();
			break;
		}

		LPWSTR objectPath = (LPWSTR)_alloca((1 + urlComponents.dwUrlPathLength) * sizeof (WCHAR));
		wcsncpy_s(objectPath, 1 + urlComponents.dwUrlPathLength, urlComponents.lpszUrlPath, _TRUNCATE);
		objectPath[urlComponents.dwUrlPathLength] = '\0';

		request = WinHttpOpenRequest(connection, L"GET", objectPath, NULL, NULL, NULL, WINHTTP_FLAG_BYPASS_PROXY_CACHE | WINHTTP_FLAG_REFRESH);
		if (!request)
		{
			result = GetLastError();
			break;
		}

		if (!WinHttpSendRequest(request, NULL, 0L, NULL, 0, 0L, NULL))
		{
			result = GetLastError();
			break;
		}

		if (!WinHttpReceiveResponse(request, NULL))
		{
			result = GetLastError();
			break;
		}

		DWORD status;
		DWORD statusSize = sizeof(DWORD);

		if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &statusSize, WINHTTP_NO_HEADER_INDEX))
		{
			result = GetLastError();
			break;
		}

		if (status != HTTP_STATUS_OK)
		{
			WCHAR buffer[1024];
			DWORD bufferSize = 1024;

			if (!WinHttpQueryHeaders(request, WINHTTP_QUERY_STATUS_TEXT, WINHTTP_HEADER_NAME_BY_INDEX, buffer, &bufferSize, WINHTTP_NO_HEADER_INDEX))
			{
				result = GetLastError();
				break;
			}

			result = ERROR_WINHTTP_INVALID_URL;
			break;
		}

		if (status != HTTP_STATUS_OK)
		{
			result = GetLastError();
			break;
		}

		DWORD offset = 0, count;
		char lbuf[0x100];

		do {
			if (WinHttpReadData(request, lbuf, 0x100, &count))
			{
				if (count == 0)
				{
					*len = offset;
					break;
				}

				if (offset + count > *len)
				{
					result = ERROR_INSUFFICIENT_BUFFER;
					break;
				}

				memcpy(buffer + offset, lbuf, count);
				offset += count;
			} else
			{
				result = GetLastError();
				break;
			}
		} while (true);
	} while (false);

	if (request) WinHttpCloseHandle(request);
	if (connection) WinHttpCloseHandle(connection);
	if (internet) WinHttpCloseHandle(internet);

	if (result != ERROR_SUCCESS)
		throw new KeyExceptionImpl(result, _T("networking error while downloading url contents"));
#else
	CURL *curl;
	CURLcode res;
 
	if ((curl = curl_easy_init()) == NULL)
		throw new KeyExceptionImpl(STATUS_NET_ERROR, _T("cURL initalization failed"));

	curl_easy_setopt(curl, CURLOPT_URL, url);
			
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
	CurlWriteBuffer curlBuffer(buffer, *len);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteBuffer::Callback);
 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);

	res = curl_easy_perform(curl);
			
	if(res != CURLE_OK)
	{
		curl_easy_cleanup(curl);
		throw new KeyExceptionImpl(STATUS_NET_ERROR, curl_easy_strerror(res));
	}
 
	*len = curlBuffer.offset;

	curl_easy_cleanup(curl);
#endif
}
