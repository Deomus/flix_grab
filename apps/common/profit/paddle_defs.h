// paddle_defs.h
#pragma once

#ifndef PADDLE_DEFS_H
#define PADDLE_DEFS_H

#include <Paddle.h>

//
#define api_key                 "a7145dc81561fa0d638617949e2d8d8e"
#define vendor_id               "19321"


using namespace PaddleSDK;


inline void initPaddleSDK(LPWSTR productName, LPWSTR companyName, char *productId, double appPriceInUSD, int trialPeriodInDays, LPWSTR appIconPath)
{
    auto paddle = Paddle::initSharedInstance(api_key, productId, vendor_id);
    paddle->setProductData(productName, companyName, appPriceInUSD, trialPeriodInDays, appIconPath);
    paddle->startLicencing(true);
}

namespace paddle
{
    const int appTrialLengthInDays = 7;

    inline bool enableProfit(fga::EAppId nId)
    {
        switch (nId)
        {
        case fga::EAppId::eUnknown:
            return false;
        case fga::EAppId::eFreeYouTubeDownload:
            return false;
        case fga::EAppId::eFreeYouTubeToMP3Converter:
            return false;
        case fga::EAppId::eFreeVimeoDownload:
            return false;
        case fga::EAppId::eFreeMySpassDownload:
            return false;
        case fga::EAppId::eFreeYouKuDownload:
            return false;
        case fga::EAppId::eFreeDailymotionDownload:
            return false;
        case fga::EAppId::eFreeClipFishDownload:
            return false;
        case fga::EAppId::eFreeTwitchDownload:
            return false;
        case fga::EAppId::eFreeXVideosDownload:
            return false;
        case fga::EAppId::eFreeFacebookVideoDownload:
            return false;
        case fga::EAppId::eFreeNetflixDownload:
            return false;
        }
        return false;
    }


    // return app price in $
    inline double getAppPrice(fga::EAppId nId)
    {
        switch (nId)
        {
        case fga::EAppId::eUnknown:
            return 0;
        case fga::EAppId::eFreeYouTubeDownload:
            return 0;
        case fga::EAppId::eFreeYouTubeToMP3Converter:
            return 0;
        case fga::EAppId::eFreeVimeoDownload:
            return 8;
        case fga::EAppId::eFreeMySpassDownload:
            return 0;
        case fga::EAppId::eFreeYouKuDownload:
            return 0;
        case fga::EAppId::eFreeDailymotionDownload:
            return 0;
        case fga::EAppId::eFreeClipFishDownload:
            return 0;
        case fga::EAppId::eFreeTwitchDownload:
            return 0;
        case fga::EAppId::eFreeXVideosDownload:
            return 0;
        case fga::EAppId::eFreeFacebookVideoDownload:
            return 0;
        case fga::EAppId::eFreeNetflixDownload:
            return 5;
        }
        return 0;
    }

    inline char *getProductId(fga::EAppId nId)
    {
        switch (nId)
        {
        case fga::EAppId::eUnknown:
            return "";
        case fga::EAppId::eFreeYouTubeDownload:
            return "";
        case fga::EAppId::eFreeYouTubeToMP3Converter:
            return "";
        case fga::EAppId::eFreeVimeoDownload:
            return "514847";
        case fga::EAppId::eFreeMySpassDownload:
            return "";
        case fga::EAppId::eFreeYouKuDownload:
            return "";
        case fga::EAppId::eFreeDailymotionDownload:
            return "";
        case fga::EAppId::eFreeClipFishDownload:
            return "";
        case fga::EAppId::eFreeTwitchDownload:
            return "";
        case fga::EAppId::eFreeXVideosDownload:
            return "";
        case fga::EAppId::eFreeFacebookVideoDownload:
            return "";
        case fga::EAppId::eFreeNetflixDownload:
            return "514362";
        }
        return "";
    }
} //paddle


#endif // PADDLE_DEFS_H
