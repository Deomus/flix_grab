#pragma once

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <comdef.h>
#include <Wbemidl.h>
#include <atlbase.h>
#include <list>
#include <string>

#pragma comment(lib, "wbemuuid.lib")

class WmiHelper
{
public:
	WmiHelper()
	{
		HRESULT hres;

		wbemServices = NULL;
		wbemLocator = NULL;
		ownCoInitialize = false;

		// Initialize COM.
		hres =  CoInitializeEx(0, COINIT_MULTITHREADED); 
		if (hres == S_OK)
		{
			ownCoInitialize = true;
		}

		// Initialize 
		hres =  CoInitializeSecurity(
			NULL,     
			-1,      // COM negotiates service                  
			NULL,    // Authentication services
			NULL,    // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,    // authentication
			RPC_C_IMP_LEVEL_IMPERSONATE,  // Impersonation
			NULL,             // Authentication info 
			EOAC_NONE,        // Additional capabilities
			NULL              // Reserved
			);

		hres = CoCreateInstance(
			CLSID_WbemLocator,             
			0, 
			CLSCTX_INPROC_SERVER, 
			IID_IWbemLocator, (LPVOID *) &wbemLocator);
 
		if (FAILED(hres))
		{
			if (ownCoInitialize)
				CoUninitialize();

			throw new exception("failed to create IWbemLocator", hres);
		}

		// Connect to the root\cimv2 namespace with the
		// current user and obtain pointer pSvc
		// to make IWbemServices calls.

		hres = wbemLocator->ConnectServer(
        
			_bstr_t(L"ROOT\\CIMV2"), // WMI namespace
			NULL,                    // User name
			NULL,                    // User password
			0,                       // Locale
			NULL,                    // Security flags                 
			0,                       // Authority       
			0,                       // Context object
			&wbemServices            // IWbemServices proxy
			);                              
    
		if (FAILED(hres))
		{
			wbemLocator->Release();

			if (ownCoInitialize)
				CoUninitialize();

			throw new exception("could not connect to WMI", hres);
		}

		// Set the IWbemServices proxy so that impersonation
		// of the user (client) occurs.
		hres = CoSetProxyBlanket(
		   wbemServices,                         // the proxy to set
		   RPC_C_AUTHN_WINNT,            // authentication service
		   RPC_C_AUTHZ_NONE,             // authorization service
		   NULL,                         // Server principal name
		   RPC_C_AUTHN_LEVEL_CALL,       // authentication level
		   RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
		   NULL,                         // client identity 
		   EOAC_NONE                     // proxy capabilities     
		);

		if (FAILED(hres))
		{
			wbemServices->Release(); 
			wbemLocator->Release();     
			if (ownCoInitialize) CoUninitialize();
			throw new exception("could not connecto to WMI", hres);
		}
	}

	~WmiHelper()
	{
		if (wbemServices) wbemServices->Release();
		if (wbemLocator) wbemLocator->Release();
		if (ownCoInitialize) CoUninitialize();
	}

	void GetPropertyList(const char * wqlQuery, const char * propertyName, list<string> * propList, int maxElements = 0x7FFFFFFF)
	{
		USES_CONVERSION;
		HRESULT hres;

		// For example, query for all the running processes
		IEnumWbemClassObject* pEnumerator = NULL;
		hres = wbemServices->ExecQuery(
			bstr_t("WQL"), 
			bstr_t(wqlQuery),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
			NULL,
			&pEnumerator);
    
		if (FAILED(hres))
		{
			throw new exception("WMI query failed");
		}
		else
		{ 
			IWbemClassObject *pclsObj;
			ULONG uReturn = 0;
			int numElements = 0;
   
			while (1)
			{
				hres = pEnumerator->Next(WBEM_INFINITE, 1, 
					&pclsObj, &uReturn);

				if(0 == uReturn)
				{
					break;
				}

				VARIANT vtProp;
				VariantInit(&vtProp);

				// Get the value of the Name property
				hres = pclsObj->Get(_bstr_t(propertyName), 0, &vtProp, 0, 0);
				//
				if (SUCCEEDED(hres))
				{
					switch (vtProp.vt)
					{
						case VT_BSTR:
							propList->push_back(W2A(vtProp.bstrVal));
							numElements++;
							break;

						case VT_I1:
						case VT_UI1:
						case VT_I2:
						case VT_UI2:
						case VT_I4:
						case VT_UI4:
							{
								char strVal[32];
								_itoa(vtProp.intVal, strVal, 10);
								propList->push_back(strVal);
								numElements++;
							}
							break;

						case VT_I8:
						case VT_UI8:
							{
								char strVal[32];
								_i64toa(vtProp.llVal, strVal, 10);
								propList->push_back(strVal);
								numElements++;
							}
							break;

						default:
							break;
					}

					VariantClear(&vtProp);
				}

				pclsObj->Release();

				if (numElements >= maxElements)
					break;
			}

			pEnumerator->Release();
		}
	}

private:
	bool ownCoInitialize;
	IWbemServices * wbemServices;
	IWbemLocator * wbemLocator;
};
