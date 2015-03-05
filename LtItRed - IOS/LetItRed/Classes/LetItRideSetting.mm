//
//  LetItRideSetting.cpp
//  LetItRide
//
//  Created by lion on 2/17/14.
//
//

#import "LetItRideSetting.h"


#import "AppController.h"

#import "GameKit/GKLocalPlayer.h"
#import "GameKit/GKScore.h"
#import "GameKit/GKAchievement.h"

#import "MKStoreManager.h"
#import "SNAdsManager.h"
#import "LocalNotificationManager.h"
#import "RateManager.h"

#import "MKStoreKitConfigs.h"

static void static_setEnable(bool enable){
    NSLog(@"enable GC %d", enable);
    
    LetItRide::LetItRideSetting *gc = LetItRide::LetItRideSetting::shared();
    gc->isEnabled = enable;
    
}

namespace LetItRide
{
    static LetItRideSetting* bjSetting;
    LetItRideSetting::LetItRideSetting()
    {
        
    }
    LetItRideSetting::~LetItRideSetting()
    {
        
    }
    
    LetItRideSetting* LetItRideSetting::shared()
    {
        if ( !bjSetting )
        {
            bjSetting = new LetItRideSetting();
        }
        return bjSetting;
    }
    
    void LetItRideSetting::alertDialog()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app alertDialog];
    }
    
    void LetItRideSetting::alertBetDialog()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app alertBetDialog];
    }
    
    void LetItRideSetting::dispChartboostBanner()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app dispChartboostBanner];
    }
    
    void LetItRideSetting::dispChartboostMoreGames()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app dispChartboostMoreGame];
    }
    
    void LetItRideSetting::dispAppLovin()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app dispAppLovin];
    }
    
    bool LetItRideSetting::isPaidVersion()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        return [app isPaidVersion];
    }
    
    void LetItRideSetting::showPlayHavenBanner(int nType)
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app showPlayHavenBanner:nType];
    }
    
    
    void LetItRideSetting::onRestore()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app onRestore];
    }
    
    void LetItRideSetting::processIAP(const char* sz)
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app processIAP:[NSString stringWithUTF8String:sz]];
    }
    
    bool LetItRideSetting::isCoins1000()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        return [app isCoins1000];
        return false;
    }
    
    bool LetItRideSetting::isCoins2500()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        return [app isCoins2500];
        return false;
    }
    
    bool LetItRideSetting::isCoins6000()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        return [app isCoins6000];
        return false;
    }
    
    bool LetItRideSetting::isCoins14000()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        return [app isCoins14000];
        return false;
    }
    
    bool LetItRideSetting::isCoins30000()
    {
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        return [app isCoins30000];
        return false;
    }
    
    
    bool LetItRideSetting::isAvailable(){
        // Check for presence of GKLocalPlayer API.
        Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
        
        // The device must be running running iOS 4.1 or later.
        NSString *reqSysVer = @"4.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
        
        return (gcClass && osVersionSupported);
        
        return YES;
    }
    
    void LetItRideSetting::playerLogin(){
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error) {
         if (error == nil)
         {
         static_setEnable( true );
         
         NSLog(@" Authenticate local player complete");
         
         }
         else
         {
         static_setEnable( false );
         NSLog(@"Authenticate local player Error: %@", [error description]);
         }
         }];
    }
    
    
    void LetItRideSetting::showLeaderboard(){
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        [app showLeaderboard];
        
    }
    
    void LetItRideSetting::submitScore(int score){
        
        AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
        
        NSString *category =@"com.goldcoin.LetItRed.Leaderboard";
        if ( [app isPaidVersion] )
        {
            category =@"gr.com.goldcoin.LetItRedPaid.Leaderboard";
        }
        [NSBundle mainBundle];
        
        
        NSLog(@"Submitting score for identifier: %@", category);
        GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
        scoreReporter.value = score;
        
        [scoreReporter reportScoreWithCompletionHandler:^(NSError *error) {
         if (error != nil)
         {
         NSLog(@"Submitting score error: %@", [error description]);
         }
         else {
         NSLog(@"Submitting score success");
         }
         
         }];
    }
}


void LetItRide::LetItRideSetting::onRemoveAds()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app processIAP:strRemoveAd];
}

void LetItRide::LetItRideSetting::buyCoins1000()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app processIAP:strCoin1000];
}
void LetItRide::LetItRideSetting::buyCoins2500()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app processIAP:strCoin2500];
}
void LetItRide::LetItRideSetting::buyCoins6000()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app processIAP:strCoin6000];
}
void LetItRide::LetItRideSetting::buyCoins14000()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app processIAP:strCoin14000];
}
void LetItRide::LetItRideSetting::buyCoins30000()
{
    AppController* app = (AppController*)[[UIApplication sharedApplication] delegate];
    [app processIAP:strCoin30000];
}

void LetItRide::LetItRideSetting::giveMeGameOverAd()
{
    [[SNAdsManager sharedManager] giveMeGameOverAd];
}

void LetItRide::LetItRideSetting::giveMeThirdGameOverAd()
{
    [[SNAdsManager sharedManager] giveMeThirdGameOverAd];
}

void LetItRide::LetItRideSetting::giveMeBootUpAd()
{
    [[SNAdsManager sharedManager] giveMeBootUpAd];
}

void LetItRide::LetItRideSetting::giveMePaidBootUpAd()
{
    [[SNAdsManager sharedManager] giveMePaidFullScreenAd];
}

void LetItRide::LetItRideSetting::giveMeFullScreenAd()
{
    [[SNAdsManager sharedManager] giveMeFullScreenAd];
}

void LetItRide::LetItRideSetting::giveMeBannerAd()
{
    [[SNAdsManager sharedManager] giveMeBannerAd];
}

void LetItRide::LetItRideSetting::giveMeMoreGamesAd()
{
    [[SNAdsManager sharedManager] giveMeMoreAppsAd];
}

void LetItRide::LetItRideSetting::giveMeLinkAd()
{
    [[SNAdsManager sharedManager] giveMeLinkAd];
}

void LetItRide::LetItRideSetting::giveMeWillEnterForegroundAd()
{
    [[SNAdsManager sharedManager] giveMeWillEnterForegroundAd];
}

void LetItRide::LetItRideSetting::locallyNotifyWithText(const char* str)
{
    [[LocalNotificationManager alloc] initWithMessage:[NSString stringWithUTF8String:str]];
}

void LetItRide::LetItRideSetting::RateMe()
{
    [[RateManager sharedManager] showReviewApp];
}
