//
//  LetItRideAppController.h
//  LetItRide
//
//  Created by lion on 1/27/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//

//#import "Chartboost.h"
#import "GameKit/GKLeaderboardViewController.h"

#define CB_ID               @"5306bc1b2d42da0e6fcb9f68"
#define CB_APPSECURITY      @"4407aa5bd206c453eb184b0c8720606385c4742d"

#define phtoken @"a1e86d3cfcef46b4883406087324f1f6"
#define phsecret @"7c9e6d9a831f4e1abcc36c2e9c54537e"

#define NAG_MORE_GAME @"more_screen"            // PlayHaven
#define NAG_ON_START_UP @"game_launch"                  // PlayHaven
#define NAG_ON_PAUSE_MENU @"pause_menu"                  // PlayHaven
#define NAG_LEVEL_COMPLETE @"level_complete"    // PlayHaven

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate,GKLeaderboardViewControllerDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
    
    //Chartboost *cb; and delegate above is ChartboostDelegate,
    
    bool m_bCoin1000;
    bool m_bCoin2500;
    bool m_bCoin6000;
    bool m_bCoin14000;
    bool m_bCoin30000;
    bool m_bRemoveAds;
    
    BOOL isPaid;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;

//@property (nonatomic, retain)   Chartboost *cb;
@property (nonatomic, assign)   bool m_bCoin1000;
@property (nonatomic, assign)   bool m_bCoin2500;
@property (nonatomic, assign)   bool m_bCoin6000;
@property (nonatomic, assign)   bool m_bCoin14000;
@property (nonatomic, assign)   bool m_bCoin30000;
@property (nonatomic, assign)   bool m_bRemoveAds;


-(void)alertDialog;
-(void)alertBetDialog;

-(void)dispChartboostBanner;
-(void)dispChartboostMoreGame;
- (void) dispAppLovin;

- (void)preloadPlayHaven;
- (void)showPlayHavenBanner:(int)nType;

- (void) showLeaderboard;

- (void)restoreCompletedTransaction;

-(void)processIAP:(NSString*)szIAP;
-(void) onRestore;

-(bool)isCoins1000;
-(bool)isCoins2500;
-(bool)isCoins6000;
-(bool)isCoins14000;
-(bool)isCoins30000;

-(BOOL)isPaidVersion;

@end

