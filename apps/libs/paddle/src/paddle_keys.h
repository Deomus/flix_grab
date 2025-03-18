#pragma once

namespace paddle {
    //Endpoints
#define DeactivateRequest           "license/deactivate"
#define ActivateRequest             "license/activate"
#define ProductInfoRequest          "product/data"
#define VerifyLicenceRequest        "license/verify"

//Result Keys;
#define ApiKey                      "api_key"
#define VendorId                    "vendor_id"
#define ProductId                   "product_id"

#define ActivationId                "activation_id"

#define BlockId						"block_id"

#define LicenseEmail                "email"
#define LicenseKey                  "license_key"
#define MachineID                   "uuid"

#define LastRefresh                 "last_refresh"
#define ProductName                 "name"
#define DeveloperName               "vendor_name"
#define PaymentUrl                  "payment_url"
#define ImageUrl                    "image"
#define Prices                      "current_prices"
#define Currency                    "currency"
#define DiscountValue               "discount_value"
#define DiscountPercentage          "discount_percentage"

#define TrialDuration               "duration"
#define TrialText                   "text"
#define Trial                       "trial"

#define FirstRun                    "start"

#define ErrorCode                   "code"
#define ErrorMessage                "message"
#define ExpireDate                  "expire_date"
#define VerifyDate                  "verify_date"

#define TrialFinished               "trial_finished"
#define TrialExpire					"trial_expire"
#define InfoExpire					"info_expire"

}

