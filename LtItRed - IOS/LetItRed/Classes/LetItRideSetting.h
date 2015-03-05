//
//  LetItRideSetting.h
//  LetItRide
//
//  Created by lion on 2/17/14.
//
//

#ifndef __LetItRide__LetItRideSetting__
#define __LetItRide__LetItRideSetting__

#include "Export.h"
#include <stddef.h>


namespace LetItRide
{
    class EXPORT_DLL LetItRideSetting
    {
    public:
        LetItRideSetting();
        ~LetItRideSetting();
        static LetItRideSetting* shared();
        static void alertDialog();
        static void alertBetDialog();
        static void dispChartboostBanner();
        static void dispChartboostMoreGames();
        static void dispAppLovin();
        static bool isPaidVersion();
        static void showPlayHavenBanner(int nType);
        
        static void onRestore();
        static void processIAP(const char* sz);
        
        
        static bool isCoins1000();
        static bool isCoins2500();
        static bool isCoins6000();
        static bool isCoins14000();
        static bool isCoins30000();
        
        static void buyCoins1000();
        static void buyCoins2500();
        static void buyCoins6000();
        static void buyCoins14000();
        static void buyCoins30000();
        static void onRemoveAds();
        
        static bool isAvailable();
        static void playerLogin();
        static void showLeaderboard();
        static void submitScore(int score);
        
        bool isEnabled;
        static void giveMeGameOverAd();
        static void giveMeThirdGameOverAd();
        static void giveMeBootUpAd();
        static void giveMePaidBootUpAd();
        static void giveMeFullScreenAd();
        static void giveMeBannerAd();
        static void giveMeMoreGamesAd();
        static void giveMeLinkAd();
        static void giveMeWillEnterForegroundAd();
        static void RateMe();
        
        static void locallyNotifyWithText(const char* str);
    };
}

#endif /* defined(__LetItRide__LetItRideSetting__) */
