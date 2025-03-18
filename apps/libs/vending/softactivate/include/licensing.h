#ifndef  __FREEGRABAPP_LICENSING_H
#define  __FREEGRABAPP_LICENSING_H

#ifndef _CHAR_T_DEFINED
#define _CHAR_T_DEFINED
#ifdef _UNICODE
#ifdef __cplusplus
typedef wchar_t char_t;
#else
#define char_t wchar_t
#endif
#else
#ifdef __cplusplus
typedef char char_t;
#else
#define char_t char
#endif
#endif
#endif

// Error codes for license key library exceptions
#define STATUS_SUCCESS                0
#define STATUS_GENERIC_ERROR          1
#define STATUS_OUT_OF_MEMORY          2
#define STATUS_FIELD_NOT_FOUND        3
#define STATUS_BUFFER_TOO_SMALL       4
#define STATUS_INVALID_XML            5
#define STATUS_INVALID_LICENSE_KEY    6
#define STATUS_INVALID_KEY_ENCODING   7
#define STATUS_INVALID_PARAM          8
#define STATUS_INVALID_SIGNATURE_SIZE 9
#define STATUS_UNSUPPORTED_VERSION    10
#define STATUS_NET_ERROR              11
#define STATUS_INVALID_HARDWARE_ID    12
#define STATUS_LICENSE_EXPIRED        13
#define STATUS_INVALID_ACTIVATION_KEY 14

// License key textual encoding - a slightly modified version of BASE32 or BASE64 in order to omit confusing characters like o, i, 1, etc.
#define ENCODING_BASE32X  5
#define ENCODING_BASE64X  6

// Data or validation field types
#define FIELD_TYPE_RAW     0
#define FIELD_TYPE_INTEGER 1
#define FIELD_TYPE_STRING  2
#define FIELD_TYPE_DATE14  3
#define FIELD_TYPE_DATE16  4
#define FIELD_TYPE_DATE13  5

#define PREFER_INTERNET_TIME 0
#define USE_INTERNET_TIME    1
#define USE_LOCAL_TIME       2

#ifdef _WIN32

#ifndef LICENSING_STATIC
#ifdef LICENSING_EXPORTS
#define LICENSING_API __declspec(dllexport)
#define LICENSEKEY_EXTINT
#else
#define LICENSING_API
#define LICENSEKEY_EXTINT
#endif
#else
#define LICENSING_API
#endif

#else // !_WIN32

#define LICENSING_API

#endif

#ifdef __cplusplus
extern "C" 
{
#endif

void * KeyGenerator_Create();
void KeyGenerator_Destroy(void * generator);
int KeyGenerator_SetKeyTemplate(void * generator, const void * tmpl);

int KeyGenerator_SetKeyDataA(void * generator, const char * fieldName, const void * data, int len);
int KeyGenerator_SetKeyDataW(void * generator, const wchar_t * fieldName, const void * data, int len);
#ifdef _UNICODE
#define KeyGenerator_SetKeyData KeyGenerator_SetKeyDataW
#else
#define KeyGenerator_SetKeyData KeyGenerator_SetKeyDataA
#endif

int KeyGenerator_SetIntKeyDataA(void * generator, const char * fieldName, int data);
int KeyGenerator_SetIntKeyDataW(void * generator, const wchar_t * fieldName, int data);
#ifdef _UNICODE
#define KeyGenerator_SetIntKeyData KeyGenerator_SetIntKeyDataW
#else
#define KeyGenerator_SetIntKeyData KeyGenerator_SetIntKeyDataA
#endif

int KeyGenerator_SetStringKeyDataA(void * generator, const char * fieldName, const char * data);
int KeyGenerator_SetStringKeyDataW(void * generator, const wchar_t * fieldName, const wchar_t * data);
#ifdef _UNICODE
#define KeyGenerator_SetStringKeyData KeyGenerator_SetStringKeyDataW
#else
#define KeyGenerator_SetStringKeyData KeyGenerator_SetStringKeyDataA
#endif

int KeyGenerator_SetDateKeyDataA(void * generator, const char * fieldName, int year, int month, int day);
int KeyGenerator_SetDateKeyDataW(void * generator, const wchar_t * fieldName, int year, int month, int day);
#ifdef _UNICODE
#define KeyGenerator_SetDateKeyData KeyGenerator_SetDateKeyDataW
#else
#define KeyGenerator_SetDateKeyData KeyGenerator_SetDateKeyDataA
#endif

int KeyGenerator_SetValidationDataA(void * generator, const char * fieldName, const void * buf, int len);
int KeyGenerator_SetValidationDataW(void * generator, const wchar_t * fieldName, const void * buf, int len);
#ifdef _UNICODE
#define KeyGenerator_SetValidationData KeyGenerator_SetValidationDataW
#else
#define KeyGenerator_SetValidationData KeyGenerator_SetValidationDataA
#endif

int KeyGenerator_SetIntValidationDataA(void * generator, const char * fieldName, int data);
int KeyGenerator_SetIntValidationDataW(void * generator, const wchar_t * fieldName, int data);
#ifdef _UNICODE
#define KeyGenerator_SetIntValidationData KeyGenerator_SetIntValidationDataW
#else
#define KeyGenerator_SetIntValidationData KeyGenerator_SetIntValidationDataA
#endif

int KeyGenerator_SetStringValidationDataA(void * generator, const char * fieldName, const char * data);
int KeyGenerator_SetStringValidationDataW(void * generator, const wchar_t * fieldName, const wchar_t * data);
#ifdef _UNICODE
#define KeyGenerator_SetStringValidationData KeyGenerator_SetStringValidationDataW
#else
#define KeyGenerator_SetStringValidationData KeyGenerator_SetStringValidationDataA
#endif

int KeyGenerator_GenerateKeyA(void * generator, const char **key);
int KeyGenerator_GenerateKeyW(void * generator, const wchar_t **key);
#ifdef _UNICODE
#define KeyGenerator_GenerateKey KeyGenerator_GenerateKeyW
#else
#define KeyGenerator_GenerateKey KeyGenerator_GenerateKeyA
#endif

void * KeyValidator_Create();
void KeyValidator_Destroy(void * validator);
int KeyValidator_SetKeyTemplate(void * validator, const void * tmpl);

int KeyValidator_SetKeyA(void * validator, const char * key);
int KeyValidator_SetKeyW(void * validator, const wchar_t * key);
#ifdef _UNICODE
#define KeyValidator_SetKey KeyValidator_SetKeyW
#else
#define KeyValidator_SetKey KeyValidator_SetKeyA
#endif

int KeyValidator_SetValidationDataA(void * validator, const char * fieldName, const void * buf, int len);
int KeyValidator_SetValidationDataW(void * validator, const wchar_t * fieldName, const void * buf, int len);
#ifdef _UNICODE
#define KeyValidator_SetValidationData KeyValidator_SetValidationDataW
#else
#define KeyValidator_SetValidationData KeyValidator_SetValidationDataA
#endif

int KeyValidator_SetIntValidationDataA(void * validator, const char * fieldName, int data);
int KeyValidator_SetIntValidationDataW(void * validator, const wchar_t * fieldName, int data);
#ifdef _UNICODE
#define KeyValidator_SetIntValidationData KeyValidator_SetIntValidationDataW
#else
#define KeyValidator_SetIntValidationData KeyValidator_SetIntValidationDataA
#endif

int KeyValidator_SetStringValidationDataA(void * validator, const char * fieldName, const char * data);
int KeyValidator_SetStringValidationDataW(void * validator, const wchar_t * fieldName, const wchar_t * data);
#ifdef _UNICODE
#define KeyValidator_SetStringValidationData KeyValidator_SetStringValidationDataW
#else
#define KeyValidator_SetStringValidationData KeyValidator_SetStringValidationDataA
#endif

int KeyValidator_IsKeyValid(void * validator);

int KeyValidator_QueryKeyDataA(void * validator, const char * dataField, void * buf, int * len);
int KeyValidator_QueryKeyDataW(void * validator, const wchar_t * dataField, void * buf, int * len);
#ifdef _UNICODE
#define KeyValidator_QueryKeyData KeyValidator_QueryKeyDataW
#else
#define KeyValidator_QueryKeyData KeyValidator_QueryKeyDataA
#endif

int KeyValidator_QueryIntKeyDataA(void * validator, const char * dataField, int * data);
int KeyValidator_QueryIntKeyDataW(void * validator, const char * dataField, int * data);
#ifdef _UNICODE
#define KeyValidator_QueryIntKeyData KeyValidator_QueryIntKeyDataW
#else
#define KeyValidator_QueryIntKeyData KeyValidator_QueryIntKeyDataA
#endif

int KeyValidator_QueryDateKeyDataA(void * validator, const char * dataField, int * year, int * month, int * day);
int KeyValidator_QueryDateKeyDataW(void * validator, const wchar_t * dataField, int * year, int * month, int * day);
#ifdef _UNICODE
#define KeyValidator_QueryDateKeyData KeyValidator_QueryDateKeyDataW
#else
#define KeyValidator_QueryDateKeyData KeyValidator_QueryDateKeyDataA
#endif

int KeyValidator_QueryValidationDataA(void * validator, const char * dataField, void * buf, int * len);
int KeyValidator_QueryValidationDataW(void * validator, const wchar_t * dataField, void * buf, int * len);
#ifdef _UNICODE
#define KeyValidator_QueryValidationData KeyValidator_QueryValidationDataW
#else
#define KeyValidator_QueryValidationData KeyValidator_QueryValidationDataA
#endif

void * LicenseTemplate_Create();
void LicenseTemplate_Destroy(void * tmpl);
int LicenseTemplate_SetVersion(void * tmpl, int version);
int LicenseTemplate_GetVersion(void * tmpl, int * version);
int LicenseTemplate_SetNumberOfGroups(void * tmpl, int numGroups);
int LicenseTemplate_GetNumberOfGroups(void * tmpl, int * numGroups);
int LicenseTemplate_SetCharactersPerGroup(void * tmpl, int charsPerGroup);
int LicenseTemplate_GetCharactersPerGroup(void * tmpl, int * charsPerGroup);

int LicenseTemplate_SetGroupSeparatorA(void * tmpl, const char * groupSep);
int LicenseTemplate_SetGroupSeparatorW(void * tmpl, const wchar_t * groupSep);
#ifdef _UNICODE
#define LicenseTemplate_SetGroupSeparator LicenseTemplate_SetGroupSeparatorW
#else
#define LicenseTemplate_SetGroupSeparator LicenseTemplate_SetGroupSeparatorA
#endif

int LicenseTemplate_GetGroupSeparatorA(void * tmpl, const char **groupSep);
int LicenseTemplate_GetGroupSeparatorW(void * tmpl, const wchar_t **groupSep);
#ifdef _UNICODE
#define LicenseTemplate_GetGroupSeparator LicenseTemplate_GetGroupSeparatorW
#else
#define LicenseTemplate_GetGroupSeparator LicenseTemplate_GetGroupSeparatorA
#endif

int LicenseTemplate_SetEncoding(void * tmpl, int encoding);
int LicenseTemplate_GetEncoding(void * tmpl, int * encoding);

int LicenseTemplate_SetHeaderA(void * tmpl, const char * header);
int LicenseTemplate_SetHeaderW(void * tmpl, const wchar_t * header);
#ifdef _UNICODE
#define LicenseTemplate_SetHeader LicenseTemplate_SetHeaderW
#else
#define LicenseTemplate_SetHeader LicenseTemplate_SetHeaderA
#endif

int LicenseTemplate_GetHeaderA(void * tmpl, const char** header);
int LicenseTemplate_GetHeaderW(void * tmpl, const wchar_t** header);
#ifdef _UNICODE
#define LicenseTemplate_GetHeader LicenseTemplate_GetHeaderW
#else
#define LicenseTemplate_GetHeader LicenseTemplate_GetHeaderA
#endif

int LicenseTemplate_SetFooterA(void * tmpl, const char * footer);
int LicenseTemplate_SetFooterW(void * tmpl, const wchar_t * footer);
#ifdef _UNICODE
#define LicenseTemplate_SetFooter LicenseTemplate_SetFooterW
#else
#define LicenseTemplate_SetFooter LicenseTemplate_SetFooterA
#endif

int LicenseTemplate_GetFooterA(void * tmpl, const char **footer);
int LicenseTemplate_GetFooterW(void * tmpl, const wchar_t **footer);
#ifdef _UNICODE
#define LicenseTemplate_GetFooter LicenseTemplate_GetFooterW
#else
#define LicenseTemplate_GetFooter LicenseTemplate_GetFooterA
#endif

int LicenseTemplate_SetDataSize(void * tmpl, int sizeInBits);
int LicenseTemplate_GetDataSize(void * tmpl, int * sizeInBits);

int LicenseTemplate_AddDataFieldA(void * tmpl, const char * fieldName, int fieldType, int fieldBitSize, int fieldOffset);
int LicenseTemplate_AddDataFieldW(void * tmpl, const wchar_t * fieldName, int fieldType, int fieldBitSize, int fieldOffset);
#ifdef _UNICODE
#define LicenseTemplate_AddDataField LicenseTemplate_AddDataFieldW
#else
#define LicenseTemplate_AddDataField LicenseTemplate_AddDataFieldA
#endif

int LicenseTemplate_GetDataFieldA(void * tmpl, const char *fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
int LicenseTemplate_GetDataFieldW(void * tmpl, const wchar_t *fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
#ifdef _UNICODE
#define LicenseTemplate_GetDataField LicenseTemplate_GetDataFieldW
#else
#define LicenseTemplate_GetDataField LicenseTemplate_GetDataFieldA
#endif

int LicenseTemplate_EnumDataFieldsA(void * tmpl, void **enumHandle, const char **fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
int LicenseTemplate_EnumDataFieldsW(void * tmpl, void **enumHandle, const wchar_t **fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
#ifdef _UNICODE
#define LicenseTemplate_EnumDataFields LicenseTemplate_EnumDataFieldsW
#else
#define LicenseTemplate_EnumDataFields LicenseTemplate_EnumDataFieldsA
#endif

int LicenseTemplate_SetValidationDataSize(void * tmpl, int sizeInBits);
int LicenseTemplate_GetValidationDataSize(void * tmpl, int * sizeInBits);

int LicenseTemplate_AddValidationFieldA(void * tmpl, const char * fieldName, int fieldType, int fieldBitSize, int fieldOffset);
int LicenseTemplate_AddValidationFieldW(void * tmpl, const wchar_t * fieldName, int fieldType, int fieldBitSize, int fieldOffset);
#ifdef _UNICODE
#define LicenseTemplate_AddValidationField LicenseTemplate_AddValidationFieldW
#else
#define LicenseTemplate_AddValidationField LicenseTemplate_AddValidationFieldA
#endif

int LicenseTemplate_GetValidationFieldA(void * tmpl, const char *fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
int LicenseTemplate_GetValidationFieldW(void * tmpl, const wchar_t *fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
#ifdef _UNICODE
#define LicenseTemplate_GetValidationField LicenseTemplate_GetValidationFieldW
#else
#define LicenseTemplate_GetValidationField LicenseTemplate_GetValidationFieldA
#endif

int LicenseTemplate_EnumValidationFieldsA(void * tmpl, void **enumHandle, const char * fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
int LicenseTemplate_EnumValidationFieldsW(void * tmpl, void **enumHandle, const wchar_t * fieldName, int * fieldType, int * fieldSize, int * fieldOffset);
#ifdef _UNICODE
#define LicenseTemplate_EnumValidationFields LicenseTemplate_EnumValidationFieldsW
#else
#define LicenseTemplate_EnumValidationFields LicenseTemplate_EnumValidationFieldsA
#endif

int LicenseTemplate_SetSignatureSize(void * tmpl, int signatureSize);
int LicenseTemplate_GetSignatureSize(void * tmpl, int * signatureSize);
int LicenseTemplate_LoadXml(void * tmpl, const char * xmlTemplate);
int LicenseTemplate_SaveXml(void * tmpl, int savePrivateKey, const char **xmlString);

int LicenseTemplate_SetPublicKeyCertificateA(void * tmpl, const char * base64PublicKey);
int LicenseTemplate_SetPublicKeyCertificateW(void * tmpl, const wchar_t * base64PublicKey);
#ifdef _UNICODE
#define LicenseTemplate_SetPublicKeyCertificate LicenseTemplate_SetPublicKeyCertificateW
#else
#define LicenseTemplate_SetPublicKeyCertificate LicenseTemplate_SetPublicKeyCertificateA
#endif

int LicenseTemplate_GetPublicKeyCertificateA(void * tmpl, const char **publicKey);
int LicenseTemplate_GetPublicKeyCertificateW(void * tmpl, const wchar_t **publicKey);
#ifdef _UNICODE
#define LicenseTemplate_GetPublicKeyCertificate KeyHelper_GetPublicKeyCertificateW
#else
#define LicenseTemplate_GetPublicKeyCertificate KeyHelper_GetPublicKeyCertificateA
#endif

int LicenseTemplate_SetPrivateKeyA(void * tmpl, const char * base64PrivateKey);
int LicenseTemplate_SetPrivateKeyW(void * tmpl, const wchar_t * base64PrivateKey);
#ifdef _UNICODE
#define LicenseTemplate_SetPrivateKey LicenseTemplate_SetPrivateKeyW
#else
#define LicenseTemplate_SetPrivateKey LicenseTemplate_SetPrivateKeyA
#endif

int LicenseTemplate_GetPrivateKeyA(void * tmpl, const char **privateKey);
int LicenseTemplate_GetPrivateKeyW(void * tmpl, const wchar_t **privateKey);
#ifdef _UNICODE
#define LicenseTemplate_GetPrivateKey KeyHelper_GetPrivateKeyW
#else
#define LicenseTemplate_GetPrivateKey KeyHelper_GetPrivateKeyA
#endif

int LicenseTemplate_GenerateSigningKeyPair(void * tmpl);

int LicenseTemplate_SetSigningServiceUrlA(void * tmpl, const char * url);
int LicenseTemplate_SetSigningServiceUrlW(void * tmpl, const wchar_t * url);
#ifdef _UNICODE
#define LicenseTemplate_SetSigningServiceUrl LicenseTemplate_SetSigningServiceUrlW
#else
#define LicenseTemplate_SetSigningServiceUrl LicenseTemplate_SetSigningServiceUrlA
#endif

int LicenseTemplate_GetSigningServiceUrlA(void * tmpl, const char **url);
int LicenseTemplate_GetSigningServiceUrlW(void * tmpl, const wchar_t **url);
#ifdef _UNICODE
#define LicenseTemplate_GetSigningServiceUrl LicenseTemplate_GetSigningServiceUrlW
#else
#define LicenseTemplate_GetSigningServiceUrl LicenseTemplate_GetSigningServiceUrlA
#endif

int LicenseTemplate_SetSigningServiceTemplateId(void * tmpl, int templateId);
int LicenseTemplate_GetSigningServiceTemplateId(void * tmpl, int * templateId);

int KeyHelper_GetCurrentHardwareIdA(const char **hwid);
int KeyHelper_GetCurrentHardwareIdW(const wchar_t **hwid);
#ifdef _UNICODE
#define KeyHelper_GetCurrentHardwareId KeyHelper_GetCurrentHardwareIdW
#else
#define KeyHelper_GetCurrentHardwareId KeyHelper_GetCurrentHardwareIdA
#endif

int KeyHelper_MatchCurrentHardwareIdA(const char **hwid);
int KeyHelper_MatchCurrentHardwareIdW(const wchar_t **hwid);
#ifdef _UNICODE
#define KeyHelper_MatchCurrentHardwareId KeyHelper_MatchCurrentHardwareIdW
#else
#define KeyHelper_MatchCurrentHardwareId KeyHelper_MatchCurrentHardwareIdA
#endif

int SDKRegistration_SetLicenseKeyA(const char * key);
int SDKRegistration_SetLicenseKeyW(const wchar_t * key);
#ifdef _UNICODE
#define SDKRegistration_SetLicenseKey SDKRegistration_SetLicenseKeyW
#else
#define SDKRegistration_SetLicenseKey SDKRegistration_SetLicenseKeyA
#endif

void LicenseTemplate_SetPropertyA(const char * path, const char * name, const char * value);
void LicenseTemplate_SetPropertyW(const wchar_t * path, const wchar_t * name, const wchar_t * value);
#ifdef _UNICODE
#define LicenseTemplate_SetProperty LicenseTemplate_SetPropertyW
#else
#define LicenseTemplate_SetProperty LicenseTemplate_SetPropertyA
#endif

const char * LicenseTemplate_GetPropertyA(const char *path, const char *name);
const wchar_t * LicenseTemplate_GetPropertyW(const wchar_t *path, const wchar_t*name);
#ifdef _UNICODE
#define LicenseTemplate_GetProperty LicenseTemplate_GetPropertyW
#else
#define LicenseTemplate_GetProperty LicenseTemplate_GetPropertyA
#endif

int Certificate_SignA(const char * csr, const char * privateKey, const char * signingPrivateKey, const char * signingCertificate, const char * certificateAuthorityUrl, int expYear, int expMonth, int expDay, const char **signedCert);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

// Opaque - skip over these
class LicenseTemplateImpl;
class KeyValidatorImpl;
class KeyHelperImpl;
class KeyGeneratorImpl;
class SDKRegistrationImpl;
class LicensingClientImpl;

namespace FreeGrabApp 
{
    namespace Licensing 
    {


        // Exceptions of this type are thrown by the license key classes
        class Exception {
        public:
            virtual int GetCode() = 0;
            virtual const char_t * GetExceptionMessage() = 0;
            virtual void Destroy() = 0;
        };

        // This class represents a template from which a license key can be generated, 
        // or by which a license key can be validated (see the samples for more).
        template<typename _XCHAR>
        class LICENSING_API LicenseTemplateT {
        public:
            LicenseTemplateT();
            ~LicenseTemplateT();

            static LicenseTemplateT<_XCHAR> * Create();
            static void Destroy(LicenseTemplateT<_XCHAR> *);

            void SetVersion(int version);
            unsigned GetVersion();

            // Sets the number of character groups per license key (eg. ABC-DEF-GHL has 3 character groups)
            void SetNumberOfGroups(int numGroups);
            unsigned GetNumberOfGroups();

            // Sets the number of characters per group (eg. ABC-DEF-GHL-XYZ has 3 characters per group) 
            void SetCharactersPerGroup(int charsPerGroup);
            unsigned GetCharactersPerGroup();

            // Sets the string used as a group separator (eg. ABC-DEF-GHL has "-" as a group separator)
            void SetGroupSeparator(const _XCHAR * groupSep);
            const _XCHAR * GetGroupSeparator();

            // Sets licese key encoding (BASE32 or BASE64 for now)
            void SetEncoding(int encoding);
            int GetEncoding();

            void SetHeader(const _XCHAR * header);
            const _XCHAR * GetHeader();

            void SetFooter(const _XCHAR * footer);
            const _XCHAR * GetFooter();

            // Sets the total size in bits of the license key contained data, excluding the signature
            void SetDataSize(int sizeInBits);
            unsigned GetDataSize();

            // Add a data field (a named sequence of bits which will be included in the license key)
            void AddDataField(const _XCHAR * fieldName, int fieldType, int fieldBitSize, int offset = -1 );
            bool EnumDataFields(void **enumHandle, const _XCHAR **fieldName, int * fieldType, int * fieldSize, int * offset);
            bool GetDataField(const _XCHAR *fieldName, int * fieldType, int * fieldSize, int * fieldOffset);

            // Sets the total size of the validation data in bits
            // Validation data is similar to license key data above, 
            // But it is not included in the license key and is used only for key validation
            void SetValidationDataSize(int sizeInBits);
            unsigned GetValidationDataSize();

            // Add a validation field (a named sequence of bits which will be required at validation time)
            void AddValidationField(const _XCHAR * fieldName, int fieldType, int fieldBitSize, int offset = -1);
            bool EnumValidationFields(void **enumHandle, const _XCHAR **fieldName, int * fieldType, int * fieldSize, int * offset);
            bool GetValidationField(const _XCHAR *fieldName, int * fieldType, int * fieldSize, int * fieldOffset);

            void SetSignatureSize(int signatureSize);
            unsigned GetSignatureSize();

            // Loads a template parameters from an XML string. Very handy sometimes.
            void LoadXml(const char * xmlTemplate);

            // Saves the template to an XML string. Do not free the returned string. It is statically allocated.
            const char * SaveXml(bool savePrivateKey = true);

            void SetPublicKeyCertificate(const _XCHAR * base64Certificate);

            const _XCHAR * GetPublicKeyCertificate();

            void SetPrivateKey(const _XCHAR * base64PrivateKey);

            const _XCHAR * GetPrivateKey();

            void GenerateSigningKeyPair();

            // Set the url of the web service used to generate keys in the absence of a local private key. This is used for license activation purposes.
            void SetSigningServiceUrl(const _XCHAR * url);

            const _XCHAR * GetSigningServiceUrl();

            void SetSigningServiceTemplateId(int templateId);

            int GetSigningServiceTemplateId();

            void SetProperty(const _XCHAR * path, const _XCHAR * name, const _XCHAR * value);

            const _XCHAR * GetProperty(const _XCHAR * path, const _XCHAR * name);

        public:
            LicenseTemplateImpl & m_Impl;
        };
#ifndef LICENSING_STATIC
        template class LICENSING_API LicenseTemplateT<char>;
        template class LICENSING_API LicenseTemplateT<wchar_t>;
#endif
        typedef LicenseTemplateT<char> KeyTemplateA;
        typedef LicenseTemplateT<wchar_t> KeyTemplateW;
        #ifdef _UNICODE
        typedef KeyTemplateW KeyTemplate;
        #else
        typedef KeyTemplateA KeyTemplate;
        #endif

        // This class is used to validate license keys and also to query their content
        template<typename _XCHAR>
        class LICENSING_API KeyValidatorT {
        public:
            KeyValidatorT();
            KeyValidatorT(const LicenseTemplateT<_XCHAR> * keyTemplate);
            ~KeyValidatorT();

            static KeyValidatorT<_XCHAR> * Create();
            static void Destroy(KeyValidatorT<_XCHAR> *);

            // Set the template used for validation (the template contains the license key format and the ECC verification key)
            void SetKeyTemplate(const LicenseTemplateT<_XCHAR> & keyTemplate);
            void SetKeyTemplate(const LicenseTemplateT<_XCHAR> * keyTemplate);

            // Set the license key used for validation and data query
            void SetKey(const _XCHAR * key);

            void SetValidationData(const _XCHAR * fieldName, const void * buf, int len);
            void SetValidationData(const _XCHAR * fieldName, const _XCHAR * data);
            void SetValidationData(const _XCHAR * fieldName, int data);

            // Verifies the license key's signature using the public(verification) ECC key from the template
            bool IsKeyValid();

            // Queries the license key for data. The data is returned in raw format (a byte buffer).
            void QueryKeyData(const _XCHAR * dataField, void * buf, int * len);

            int QueryIntKeyData(const _XCHAR * dataField);

            void QueryDateKeyData(const _XCHAR * dataField, int * year, int * month, int * day);

            void QueryValidationData(const _XCHAR * filedName, void * buf, int * len);

        private:
            KeyValidatorImpl & m_Impl;
        };

#ifndef LICENSING_STATIC
        template class LICENSING_API KeyValidatorT<char>;
        template class LICENSING_API KeyValidatorT<wchar_t>;
#endif
        typedef KeyValidatorT<char> KeyValidatorA;
        typedef KeyValidatorT<wchar_t> KeyValidatorW;
        #ifdef _UNICODE
        typedef KeyValidatorW KeyValidator;
        #else
        typedef KeyValidatorA KeyValidator;
        #endif

        template<typename _XCHAR>
        // This class is used to generate license keys and set their content
        class LICENSING_API KeyGeneratorT {
        public:
            KeyGeneratorT();
            KeyGeneratorT(const LicenseTemplateT<_XCHAR> * keyTemplate);
            ~KeyGeneratorT();

            static KeyGeneratorT<_XCHAR> * Create();
            static void Destroy(KeyGeneratorT<_XCHAR> *);

            // Sets the template from which the license keys are generated.
            void SetKeyTemplate(const LicenseTemplateT<_XCHAR> & keyTemplate);
            void SetKeyTemplate(const LicenseTemplateT<_XCHAR> * keyTemplate);

            // Sets license key data fields with raw, string or integer values
            void SetKeyData(const _XCHAR * fieldName, const void * data, int len);
            void SetKeyData(const _XCHAR * fieldName, const _XCHAR * data);
            void SetKeyData(const _XCHAR * fieldName, int data);
            void SetKeyData(const _XCHAR * fieldName, int year, int month, int day);
 
            // Sets the validation fields. These are not included in the license key,
            // but the license key validates only if these values are also used for validation
            void SetValidationData(const _XCHAR * fieldName, const void * data, int len);
            void SetValidationData(const _XCHAR * fieldName, const _XCHAR * data);
            void SetValidationData(const _XCHAR * fieldName, int data);

            // Generates a license key. The returned string is statically 
            // allocated (thus it is overriden at the next call), do not try to deallocate it.
            const _XCHAR * GenerateKey();

        private:
            KeyGeneratorImpl & m_Impl;
        };

#ifndef LICENSING_STATIC
        template class LICENSING_API KeyGeneratorT<char>;
        template class LICENSING_API KeyGeneratorT<wchar_t>;
#endif
        typedef KeyGeneratorT<char> KeyGeneratorA;
        typedef KeyGeneratorT<wchar_t> KeyGeneratorW;
        #ifdef _UNICODE
        typedef KeyGeneratorW KeyGenerator;
        #else
        typedef KeyGeneratorA KeyGenerator;
        #endif

        template <typename _XCHAR>
        class LICENSING_API LicensingClientT
        {
        public:
            LicensingClientT();
            ~LicensingClientT();

            void SetLicensingServiceUrl(const _XCHAR * url);

            void SetLicenseTemplate(const LicenseTemplateT<_XCHAR> & tmpl);
            void SetLicenseKey(const _XCHAR * key);

            void SetActivationKey(const _XCHAR * key);
            const _XCHAR * GetActivationKey();
            
            void SetHardwareId(const _XCHAR * hardwareId);
            const _XCHAR * GetHardwareId();
            
            void SetLicenseKeyValidationData(void * buf, int len);
            void SetProductId(int productId);

            bool IsLicenseValid();
            void AcquireLicense();

            int GetLicenseActivationStatus();

            void GetLicenseExpirationDate(int * year, int * month, int * day);

            void SetTimeValidationMethod(int method); // method = [PREFER_INTERNET_TIME (default) | USE_INTERNET_TIME | USE_LOCAL_TIME]

            virtual const _XCHAR * GetCurrentHardwareId();
            virtual bool MatchCurrentHardwareId(const _XCHAR * hardwareId);
        
        private:
            LicensingClientImpl & m_Impl;
        };

#ifndef LICENSING_STATIC

        template class LICENSING_API LicensingClientT<char>;
        template class LICENSING_API LicensingClientT<wchar_t>;
#endif
        typedef LicensingClientT<char> LicensingClientA;
        typedef LicensingClientT<wchar_t> LicensingClientW;
        #ifdef _UNICODE
        typedef LicensingClientW LicensingClient;
        #else
        typedef LicensingClientA LicensingClient;
        #endif

        template< typename _XCHAR >
        class KeyHelperT 
        {
        public:
            static const _XCHAR * GetCurrentHardwareId();
            static bool MatchCurrentHardwareId(const _XCHAR * hwid);
            static bool DetectClockManipulation(int thresholdYear, int thresholdMonth, int thresholdDay);
        };

#ifndef LICENSING_STATIC
        template class LICENSING_API KeyHelperT<char>;
        template class LICENSING_API KeyHelperT<wchar_t>;
#endif
        typedef KeyHelperT<char> KeyHelperA;
        typedef KeyHelperT<wchar_t> KeyHelperW;
        #ifdef _UNICODE
        typedef KeyHelperW KeyHelper;
        #else
        typedef KeyHelperA KeyHelper;
        #endif

        template< typename _XCHAR >
        class SDKRegistrationT
        {
        public:
            static void SetLicenseKey(const _XCHAR * key);

        private:
            static SDKRegistrationImpl & m_Impl;
        };

#ifndef LICENSING_STATIC
        template class LICENSING_API SDKRegistrationT<char>;
        template class LICENSING_API SDKRegistrationT<wchar_t>;
#endif
        typedef SDKRegistrationT<char> SDKRegistrationA;
        typedef SDKRegistrationT<wchar_t> SDKRegistrationW;
        #ifdef _UNICODE
        typedef SDKRegistrationW SDKRegistration;
        #else
        typedef SDKRegistrationA SDKRegistration;
        #endif
    };
};

using namespace FreeGrabApp::Licensing;

#endif /* __cplusplus */
#endif /* LICENSEKEY_H */

