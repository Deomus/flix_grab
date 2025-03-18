#pragma once

namespace vending {

#define LICENSE_PRODUCT_ID                  "ProductId"
#define LICENSE_EXPIRATION_DATE				"ExpirationDate"
#define LICENSE_REGISTRATION_EMAIL          "RegistrationEmail"


    static const char kLicenseTemplate[] =
        "<?xml version='1.0' encoding='utf-8'?>"
        "<LicenseTemplate version='3'>"
        "<LicenseKey encoding='BASE32X' characterGroups='5' charactersPerGroup='6' groupSeparator='-' header='' footer=''>"
        "<Data>"
        "<DataFields>"
		"<Field name='ProductId' type='Integer' size='8' offset='0' />"
		"<Field name='ExpirationDate' type='Date14' size='14' offset='8' />"
        "</DataFields>"
        "<ValidationFields>"
        "<Field name='RegistrationEmail' type='String' size='400' offset='0' />"
        "</ValidationFields>"
        "</Data>"
        "<Signature size='109'>"
        //"<SIGNATURE_PUBLIC_KEY>SIGNATURE_PUBLIC_KEY</SIGNATURE_PUBLIC_KEY>"
        //"<SIGNATURE_PRIVATE_KEY>VENDING_PRIVATE_KEY</SIGNATURE_PRIVATE_KEY>"
        "<SigningServiceUrl>http://vending.freegrabapp.com</SigningServiceUrl>"
		//"<SigningServiceUrl>http://localhost:60001</SigningServiceUrl>"
        //"<SigningServiceTemplateId>VENDING_PRODUCT_ID</SigningServiceTemplateId>"
        "</Signature>"
        "</LicenseKey>"
        "<Properties>"
        "</Properties>"
        "</LicenseTemplate>";

//Only for FlixGrab!
//#define SIGNATURE_PRIVATE_KEY                0x0041,0x0051,0x006B,0x0054,0x0079,0x0031,0x0051,0x0032,0x0039,0x006D,0x0045,0x0071,0x006C,0x0042,0x0038,0x003D

#define SIGNATURE_PRIVATE_KEY                0x0000

}
