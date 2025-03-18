#include "precomp.h"
#include "licensingclient.h"

namespace FreeGrabApp {
    namespace Licensing {

        template<>
        LicensingClientT<wchar_t>::LicensingClientT():
        m_Impl(*(new LicensingClientImpl(this, true)))
        {

        }

        template<>
        LicensingClientT<char>::LicensingClientT(): 
        m_Impl(*(new LicensingClientImpl(this, false)))
        {

        }

        template<>
        LicensingClientT<char>::~LicensingClientT()
        {
            delete & m_Impl;
        }

        template<>
        LicensingClientT<wchar_t>::~LicensingClientT()
        {
            delete & m_Impl;
        }

        template<>
        void LicensingClientT<char>::SetLicenseTemplate(const LicenseTemplateT<char> & tmpl)
        {
            m_Impl.SetLicenseTemplate(tmpl.m_Impl);
        }

        template<>
        void LicensingClientT<wchar_t>::SetLicenseTemplate(const LicenseTemplateT<wchar_t> & tmpl)
        {
            m_Impl.SetLicenseTemplate(tmpl.m_Impl);
        }

        template<>
        void LicensingClientT<char>::SetLicenseKey(const char * key)
        {
            USES_CONVERSION;
            m_Impl.SetLicenseKey(A2T(key));
        }

        template<>
        void LicensingClientT<wchar_t>::SetLicenseKey(const wchar_t * key)
        {
            USES_CONVERSION;
            m_Impl.SetLicenseKey(W2T(key));
        }

        template<>
        void LicensingClientT<char>::SetActivationKey(const char * key)
        {
            USES_CONVERSION;
            m_Impl.SetActivationKey(A2T(key));
        }

        template<>
        void LicensingClientT<wchar_t>::SetActivationKey(const wchar_t * key)
        {
            USES_CONVERSION;
            m_Impl.SetActivationKey(W2T(key));
        }

        template<>
        const char * LicensingClientT<char>::GetActivationKey()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string key;
            key = W2A(m_Impl.GetActivationKey());
            return key.c_str();
        #else
            return m_Impl.GetActivationKey();
        #endif
        }

        template<>
        const wchar_t * LicensingClientT<wchar_t>::GetActivationKey()
        {
        #ifdef _UNICODE
            return m_Impl.GetActivationKey();
        #else
            USES_CONVERSION;
            static std::wstring key;
            key = A2W(m_Impl.GetActivationKey());
            return key.c_str();
        #endif
        }

        template<>
        void LicensingClientT<char>::SetHardwareId(const char * hwid)
        {
            USES_CONVERSION;
            m_Impl.SetHardwareId(A2T(hwid));
        }

        template<>
        void LicensingClientT<wchar_t>::SetHardwareId(const wchar_t * hwid)
        {
            USES_CONVERSION;
            m_Impl.SetHardwareId(W2T(hwid));
        }

        template<>
        const char * LicensingClientT<char>::GetHardwareId()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string hwid;
            hwid = W2A(m_Impl.GetHardwareId());
            return hwid.c_str();
        #else
            return m_Impl.GetHardwareId();
        #endif
        }

        template<>
        const wchar_t * LicensingClientT<wchar_t>::GetHardwareId()
        {
        #ifdef _UNICODE
            return m_Impl.GetHardwareId();
        #else
            USES_CONVERSION;
            static std::wstring hwid;
            hwid = A2W(m_Impl.GetHardwareId());
            return hwid.c_str();
        #endif
        }

        template<>
        const wchar_t * LicensingClientT<wchar_t>::GetCurrentHardwareId()
        {
        #ifdef _UNICODE
            return m_Impl.GetCurrentHardwareId();
        #else
            USES_CONVERSION;
            static std::wstring currenthwid;
            currenthwid = A2W(m_Impl.GetCurrentHardwareId());
            return currenthwid.c_str();         
        #endif
        }

        template<>
        const char * LicensingClientT<char>::GetCurrentHardwareId()
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            static std::string currenthwid;
            currenthwid = W2A(m_Impl.GetCurrentHardwareId());
            return currenthwid.c_str();
        #else
            return m_Impl.GetCurrentHardwareId();
        #endif
        }

        template<>
        bool LicensingClientT<char>::MatchCurrentHardwareId(const char * hwid)
        {
        #ifdef _UNICODE
            USES_CONVERSION;
            return m_Impl.MatchCurrentHardwareId(A2W(hwid));
        #else
            return m_Impl.MatchCurrentHardwareId(hwid);
        #endif
        }

        template<>
        bool LicensingClientT<wchar_t>::MatchCurrentHardwareId(const wchar_t * hwid)
        {
        #ifdef _UNICODE
            return m_Impl.MatchCurrentHardwareId(hwid);
        #else
            USES_CONVERSION;
            return m_Impl.MatchCurrentHardwareId(W2A(hwid));
        #endif
        }

        template<>
        void LicensingClientT<char>::SetLicensingServiceUrl(const char * url)
        {
            USES_CONVERSION;
            m_Impl.SetLicensingServiceUrl(A2T(url));
        }

        template<>
        void LicensingClientT<wchar_t>::SetLicensingServiceUrl(const wchar_t * url)
        {
            USES_CONVERSION;
            m_Impl.SetLicensingServiceUrl(W2T(url));
        }

        template<>
        void LicensingClientT<char>::SetProductId(int productId)
        {
            m_Impl.SetProductId(productId);
        }

        template<>
        void LicensingClientT<wchar_t>::SetProductId(int productId)
        {
            m_Impl.SetProductId(productId);
        }

        template<>
        void LicensingClientT<char>::AcquireLicense()
        {
            m_Impl.AcquireLicense();
        }

        template<>
        void LicensingClientT<wchar_t>::AcquireLicense()
        {
            m_Impl.AcquireLicense();
        }

        template<>
        bool LicensingClientT<char>::IsLicenseValid()
        {
            return m_Impl.IsLicenseValid();
        }

        template<>
        bool LicensingClientT<wchar_t>::IsLicenseValid()
        {
            return m_Impl.IsLicenseValid();
        }

        template<>
        void LicensingClientT<char>::SetLicenseKeyValidationData(void * data, int len)
        {
            return m_Impl.SetLicenseKeyValidationData(data, len);
        }

        template<>
        void LicensingClientT<wchar_t>::SetLicenseKeyValidationData(void * data, int len)
        {
            return m_Impl.SetLicenseKeyValidationData(data, len);
        }

        template<>
        int LicensingClientT<char>::GetLicenseActivationStatus()
        {
            return m_Impl.GetLicenseActivationStatus();
        }

        template<>
        int LicensingClientT<wchar_t>::GetLicenseActivationStatus()
        {
            return m_Impl.GetLicenseActivationStatus();
        }

        template<>
        void LicensingClientT<char>::GetLicenseExpirationDate(int * year, int * month, int * day)
        {
            m_Impl.GetLicenseExpirationDate(year, month, day);
        }

        template<>
        void LicensingClientT<wchar_t>::GetLicenseExpirationDate(int * year, int * month, int * day)
        {
            m_Impl.GetLicenseExpirationDate(year, month, day);
        }

        template<>
        void LicensingClientT<char>::SetTimeValidationMethod(int method)
        {
            return m_Impl.SetTimeValidationMethod(method);
        }

        template<>
        void LicensingClientT<wchar_t>::SetTimeValidationMethod(int method)
        {
            return m_Impl.SetTimeValidationMethod(method);
        }
    };
};

const wchar_t * GetCurrentHardwareIdW(void * licensingClient)
{
    USES_CONVERSION;
    static std::wstring result = ((FreeGrabApp::Licensing::LicensingClientT<wchar_t> *)licensingClient)->GetCurrentHardwareId();
    return result.c_str();
}

bool MatchCurrentHardwareIdW(void * licensingClient, const wchar_t * hwid)
{
    USES_CONVERSION;
    return ((FreeGrabApp::Licensing::LicensingClientT<wchar_t> *)licensingClient)->MatchCurrentHardwareId(hwid);
}

bool MatchCurrentHardwareIdA(void * licensingClient, const char * hwid)
{
    return ((FreeGrabApp::Licensing::LicensingClientT<char> *)licensingClient)->MatchCurrentHardwareId(hwid);
}

const char * GetCurrentHardwareIdA(void * licensingClient)
{
    return ((FreeGrabApp::Licensing::LicensingClientT<char> *)licensingClient)->GetCurrentHardwareId();
}
