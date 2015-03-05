//
//  LetItRideAppController.mm
//  LetItRide
//
//  Created by lion on 1/27/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"

#import "RootViewController.h"

//#import "PlayHavenSDK.h"

#import "GameKit/GKLocalPlayer.h"
#import "GameKit/GKScore.h"
#import "GameKit/GKAchievement.h"

#import "LetItRideSetting.h"

#import "MKStoreManager.h"
#import "MKStoreKitConfigs.h"
#import "MBProgressHUD.h"

#import "GameSetting.h"

//#import "ALSdk.h"
//#import "ALInterstitialAd.h"

#import "Extern.h"

@implementation AppController

@synthesize window;
@synthesize viewController;

//@synthesize cb;

@synthesize m_bCoin1000;
@synthesize m_bCoin2500;
@synthesize m_bCoin6000;
@synthesize m_bCoin14000;
@synthesize m_bCoin30000;
@synthesize m_bRemoveAds;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.

    isPaid = false;
#ifdef PAID_ELE_VERSION
    [MKStoreManager makePaidVersion];
    isPaid = true;
#endif
    // APPLOVIN commented
    //[ALSdk initializeSdk];
    
    LetItRide::LetItRideSetting* gc = LetItRide::LetItRideSetting::shared();
    
    if( gc->isAvailable() ){
        gc->playerLogin();
    }
    //enabling in app purchase
    
    m_bCoin1000 = false;
    m_bCoin2500 = false;
    m_bCoin6000 = false;
    m_bCoin14000 = false;
    m_bCoin30000 = false;
    m_bRemoveAds = false;
    GameSetting* gameSetting = GameSetting::shared();
//    gameSetting->isRemoveAds = m_bRemoveAds;
//    gameSetting->isRemoveAds = gameSetting->getRemoveAd();
//    gameSetting->saveRemoveAd(gameSetting->isRemoveAds);
    
    [MKStoreManager sharedManager];
    
    
    if([[UIApplication sharedApplication] respondsToSelector:@selector(registerUserNotificationSettings:)]) {
        [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert|UIUserNotificationTypeSound categories:nil]];
    }
//    if ([MKStoreManager isFeaturePurchased:strCoin1000])
//        m_bCoin1000 = YES;
//    
//    if ([MKStoreManager isFeaturePurchased:strCoin2500])
//        m_bCoin2500 = YES;
//    if ([MKStoreManager isFeaturePurchased:strCoin6000])
//        m_bCoin6000 = YES;
//    if ([MKStoreManager isFeaturePurchased:strCoin14000])
//        m_bCoin14000 = YES;
//    if ([MKStoreManager isFeaturePurchased:strCoin30000])
//        m_bCoin30000 = YES;
//    
//    if ([MKStoreManager isFeaturePurchased:strRemoveAd])
//        m_bRemoveAds = YES;
    
//    gameSetting->isRemoveAds = m_bRemoveAds;
//    gameSetting->saveRemoveAd(gameSetting->isRemoveAds);
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    /******** chart boost************/
//    cb = [Chartboost sharedChartboost];
//    
//    cb.appId = CB_ID;
//    cb.appSignature = CB_APPSECURITY;
//    cb.delegate = self;
//    
//    [cb startSession];
//    
//    [cb cacheInterstitial];
//    
//    [cb cacheMoreApps];
    
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];

    [[UIApplication sharedApplication] setStatusBarHidden: YES];

//    if ( !isPaid )
//    {
//        [self dispAppLovin];
//    }
    
    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
    
    [super dealloc];
}

-(BOOL)isPaidVersion
{
    return  isPaid;
}

#pragma mark other
-(void)alertDialog
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"Notice"
                                                        message:@"You have no enough coins!"
                                                       delegate:self
                                              cancelButtonTitle:@"Ok"
                                              otherButtonTitles:nil];
    [alertView show];
    [alertView release];
}

-(void)alertBetDialog
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:@"Notice"
                                                        message:@"Please Select Bet!"
                                                       delegate:self
                                              cancelButtonTitle:@"Ok"
                                              otherButtonTitles:nil];
    [alertView show];
    [alertView release];
}

#pragma mark chartboost
-(void)dispChartboostBanner
{
    if ( [self isPaidVersion] )
        return;
    
   // [cb showInterstitial];
}

-(void)dispChartboostMoreGame
{
    if ( [self isPaidVersion] )
        return;

   // [cb showMoreApps];
}

#pragma mark GAME CENTER
- (void) showLeaderboard{
	NSLog(@"show leaderboard");
	
    GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardController != nil)
    {
        leaderboardController.leaderboardDelegate = self;
        [viewController presentModalViewController:leaderboardController animated: YES];
    }
}

- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)_viewController{
	[_viewController dismissModalViewControllerAnimated:YES];
}

#pragma mark InApp
-(void)processIAP:(NSString*)szIAP
{
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.window animated:YES];
    [hud hide:true afterDelay:60];
    hud.mode = MBProgressHUDModeIndeterminate;
    hud.labelText = @"Loading";
    
    [[MKStoreManager sharedManager] buyFeature:szIAP
                                    onComplete:^(NSString *purchasedFeature, NSData *purchasedReceipt,SKPaymentTransaction* transaction)
     {
         @try
         {
             
             [MBProgressHUD hideAllHUDsForView:self.window animated:true];
             
             if([purchasedFeature isEqualToString:strCoin1000])
             {
                 m_bCoin1000 = YES;
                 g_nYourCoin += 1000;
                 GameSetting* gameSetting = GameSetting::shared();
                 gameSetting->saveCoins();
             }
             else if([purchasedFeature isEqualToString:strCoin2500])
             {
                 m_bCoin2500 = YES;
                 g_nYourCoin += 2500;
                 GameSetting* gameSetting = GameSetting::shared();
                 gameSetting->saveCoins();
             }
             else if([purchasedFeature isEqualToString:strCoin6000])
             {
                 m_bCoin6000 = YES;
                 g_nYourCoin += 6000;
                 GameSetting* gameSetting = GameSetting::shared();
                 gameSetting->saveCoins();
             }
             else if([purchasedFeature isEqualToString:strCoin14000])
             {
                 m_bCoin14000 = YES;
                 g_nYourCoin += 14000;
                 GameSetting* gameSetting = GameSetting::shared();
                 gameSetting->saveCoins();
             }
             else if([purchasedFeature isEqualToString:strCoin30000])
             {
                 m_bCoin30000 = YES;
                 g_nYourCoin += 30000;
                 GameSetting* gameSetting = GameSetting::shared();
                 gameSetting->saveCoins();
             }
             else if([purchasedFeature isEqualToString:strRemoveAd])
             {
                 m_bRemoveAds = YES;
                 GameSetting* gameSetting = GameSetting::shared();
                 gameSetting->isRemoveAds = m_bRemoveAds;
                 gameSetting->saveRemoveAd(gameSetting->isRemoveAds);
             }
         }
         @catch (NSException *exception)
         {
             NSLog(@"Coin IAP Error");
         }
     }
                                   onCancelled:^
     {
         [MBProgressHUD hideAllHUDsForView:self.window animated:true];
         
     }];
}

-(void) onRestore
{
    //    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
    [[MKStoreManager sharedManager] restorePreviousTransactionsOnComplete:^
     {
         m_bRemoveAds = YES;
         GameSetting* gameSetting = GameSetting::shared();
         gameSetting->isRemoveAds = m_bRemoveAds;
         gameSetting->saveRemoveAd(gameSetting->isRemoveAds);
     }
                                                                  onError:
     ^(NSError* error)
     {
         //         [[MKStoreManager sharedManager] restoreFailedWithError:error];
         NSLog(@"Restore Failed");
     }];
    
}

- (void)restoreCompletedTransaction{
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
    
}

-(void)transactionDidError{
    
}

-(void)transactionDidFinishRestoring
{
    
}

-(void)transactionDidFinish:(NSString *)transactionIdentifier
{
    
}



#pragma mark SKPaymentTransactionObserver

-(void)paymentQueue:(SKPaymentQueue *)queue restoreCompletedTransactionsFailedWithError:(NSError *)error
{
    NSLog(@"Restore Failed With Error:%@", error);
}

-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    
}

-(void)paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    
    for (SKPaymentTransaction* transaction in queue.transactions) {
        NSString* productID = transaction.payment.productIdentifier;
        
        if([productID isEqualToString:strRemoveAd])
        {
            m_bRemoveAds = YES;
            GameSetting* gameSetting = GameSetting::shared();
            gameSetting->isRemoveAds = m_bRemoveAds;
            gameSetting->saveRemoveAd(gameSetting->isRemoveAds);
        }
    }
}

-(bool)isCoins1000
{
    return m_bCoin1000;
}
-(bool)isCoins2500
{
    return m_bCoin2500;
}
-(bool)isCoins6000
{
    return m_bCoin6000;
}
-(bool)isCoins14000
{
    return m_bCoin14000;
}
-(bool)isCoins30000
{
    return m_bCoin30000;
}

#pragma mark ChartboostDelegate

- (void)didFailToLoadInterstitial:(NSString *)location
{
    [self dispAppLovin];
}

// Called when the user dismisses the interstitial
// If you are displaying the add yourself, dismiss it now.
- (void)didDismissInterstitial:(NSString *)location
{
//    [self dispAppLovin];
}

// Same as above, but only called when dismissed for a close
- (void)didCloseInterstitial:(NSString *)location
{
    [self dispAppLovin];
}

@end

