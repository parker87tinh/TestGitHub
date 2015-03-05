//
//  GenericAd.m
//  Alien Tower
//
//  Created by Asad Khan on 05/02/2013.
//
//

#import "GenericAd.h"
///////////////////////////////////////////ChartBoost/////////////////////////////
#import <Chartboost/Chartboost.h>
#import <Chartboost/CBNewsfeed.h>
#import <CommonCrypto/CommonDigest.h>
//////////////////////////////////////////////////////////////////////////////////
#import <AdSupport/AdSupport.h>
#import "SNAdsManager.h"
#import "ALInterstitialAd.h"
#import "ALSdk.h"
static int count = 0;
static BOOL isAdVisible;
static BOOL isAppLovinWorking;
@interface GenericAd()


@property (nonatomic, retain) NSTimer *playHavenTimer;
@property (nonatomic, assign)BOOL hasPlayHavenAdLoaded;
@end


@implementation GenericAd

/**
Chartboost AdNetwork calls the didFailToLoadInterstitial: delegate method even when it tries and fails to load the cache which results in 2 callbacks. The Problem with our admanager than is it loads two lower priority ads. To mitigate this we will be using nstimer to distinguish between 2 consecutive calls. If the time difference between the calls is less than a specified limit we will ignore the second call.
 For this we will be declaring a static float variable to store first intial failure time and than we will compare it with every second call we wll recieve.

 Another option is to wait a predefined number of seconds before calling another ad network. This option is NOT currently implemented in this code.
*/

static double firstCallBackTime;
static int callBackCount;

/**
 Sometimes RevMob fails to notify that loading ad has been failed so as a back we're adding fail time for RevMob FullScreen and Banner both
 */


@synthesize adNetworkType = _adNetworkType;
@synthesize adType = _adType;
@synthesize isTestAd = _isTestAd;
@synthesize adPriority = _adPriority;
@synthesize adView;
@synthesize delegate = _delegate;
@synthesize playHavenTimer = _playHavenTimer;


- (id) initWithAdNetworkType:(NSUInteger)adNetworkType andAdType:(NSUInteger)adType{
	self = [super init];
	if(self !=nil){
        _adType = adType;
        _adNetworkType = adNetworkType;
        switch (adNetworkType) {
            case kAppLovin:
                if (_adType == kBannerAd){
                    adView = [[ALAdView alloc] initBannerAd];
                    adView.adDisplayDelegate = self;
                    adView.adLoadDelegate = self;
                    _adPriority = kAppLovinBannerAdPriority;
                }
                else if (adType == kFullScreenAd){
                    [ALInterstitialAd shared].adDisplayDelegate = self;
                    [ALInterstitialAd shared].adLoadDelegate = self;
                    _adPriority = kAppLovinFullScreenAdPriority;
                }
                break;
            case kChartBoost:
                if (adType == kFullScreenAd){
                    _adPriority = kChartBoostFullScreeAdPriority;
                    self.chartBoost = [SNAdsManager sharedManager].chartBoost;
                    [Chartboost startWithAppId:ChartBoostAppID appSignature:ChartBoostAppSignature delegate:self];
                    [Chartboost cacheInterstitial:CBLocationHomeScreen];
                    
                    //self.chartBoost.delegate = self;

                }
                else if (adType == kMoreAppsAd)
                    _adPriority = kChartBoostMoreAppsAd;
                break;
            case kPlayHaven:
                if (adType == kFullScreenAd) {
                    _adPriority = kPlayHavenFullScreenAdPriority;
                    [[PHPublisherContentRequest requestForApp:kPlayHavenAppToken secret:kPlayHavenSecret placement:kPlayHavenPlacement delegate:self] preload];
                }
                break;
           
            default:
               // NSAssert(!adNetworkType == kUndefined, @"Value for Ad Network cannot be Undefined");
                [NSException raise:@"Undefined Ad Network" format:@"Ad Network is unknown cannot continue"];
                break;
        }
	}
	return self;
}

- (id) initWithAdType:(NSUInteger)adType{
    self = [super init];
	if(self !=nil){
        _adType = adType;   
    }
    return self;
}

-(void)showBannerAdAtTop{
    switch(self.adNetworkType){
                case kAppLovin:
            dispatch_async(dispatch_get_main_queue(), ^{
                [[SNAdsManager getRootViewController].view addSubview: adView];
                if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
                    self.adView.frame = CGRectMake(250, 0, 640, adView.frame.size.height);
                } else {
                    self.adView.frame = CGRectMake(100, 0, 320, adView.frame.size.height);
                }
                [adView loadNextAd];
                });
            break;
        default:
            [NSException raise:@"Undefined Ad Network" format:@"Ad Network is unknown or does not have a banner Ad."];
            break;
    }
}

-(void)showBannerAd{
    switch(self.adNetworkType){
            case kAppLovin:
            dispatch_async(dispatch_get_main_queue(), ^{
            adView.frame = CGRectMake( 0,
                                      [SNAdsManager getRootViewController].view.frame.size.height - adView.frame.size.height,
                                      adView.frame.size.width,
                                      adView.frame.size.height );
             
            [[SNAdsManager getRootViewController].view addSubview: adView];
            [adView loadNextAd];
               // [NSTimer scheduledTimerWithTimeInterval:kAppLovinTimeOutThresholdValue target:self selector:@selector(adService:didFailToLoadAdWithError:) userInfo:nil repeats:NO];
            });
        break;
    default:
            [NSException raise:@"Undefined Ad Network" format:@"Ad Network is unknown or does not have a banner Ad."];
        break;
    }
}


-(void)showFullScreenAd{
    if (isAdVisible) {
        DebugLog(@"Ad already visible");
        //return;
    }
    
    switch(self.adNetworkType){
            case kAppLovin:{
            dispatch_async(dispatch_get_main_queue(), ^{
                NSLog(@"%@", [[ALSdk shared] description]);
                
                if (self.AppLovinTimer == nil) {
                    [ALInterstitialAd showOver:[UIApplication sharedApplication].keyWindow];
                    [[[ALSdk shared] adService] loadNextAd:[ALAdSize sizeInterstitial] andNotify:self];
                    self.AppLovinTimer = [NSTimer scheduledTimerWithTimeInterval:kAppLovinTimeOutThresholdValue target:self selector:@selector(adService:didFailToLoadAdWithError:) userInfo:nil repeats:NO];
                    DebugLog(@"Scheduled AppLovin Timer ");
                }
                
                
            }); 
        }
            break;
        case kChartBoost:{
            dispatch_async(dispatch_get_main_queue(), ^{
                //self.chartBoost.delegate = self;
                [Chartboost showInterstitial:CBLocationHomeScreen];
                //self.chartBoostTimer = [NSTimer scheduledTimerWithTimeInterval:kChartBoostTimeOutThresholdValue target:self selector:@selector(didFailToLoadInterstitial:) userInfo:nil repeats:NO];
            });
        }
            break;
        case kPlayHaven:{
            count++;
            NSLog(@"COunt for play HAVEN %d", count);
          //  dispatch_async(dispatch_get_main_queue(), ^{
                [self showPlayHavenFullScreenAd];
          //  });
            
        }
            break;
        default:
            [NSException raise:@"Undefined Ad Network" format:@"Ad Network is unknown or does not have a banner Ad."];
            break;
    }
}

-(void)showLinkButtonAd{
    //[self.adLink openLink];
}

+(CGSize) sizeInOrientation:(UIInterfaceOrientation)orientation
{
    CGSize size = [UIScreen mainScreen].bounds.size;
    UIApplication *application = [UIApplication sharedApplication];
    if (UIInterfaceOrientationIsLandscape(orientation))
    {
        size = CGSizeMake(size.height, size.width);
    }
    if (application.statusBarHidden == NO)
    {
#ifdef IS_IOS7_AND_UP
        //return size;
#else
        size.height -= MIN(application.statusBarFrame.size.width, application.statusBarFrame.size.height);
#endif
    }
    return size;
}

+(CGSize) currentSize
{
    return [GenericAd sizeInOrientation:[UIApplication sharedApplication].statusBarOrientation];
}


- (void)showPlayHavenFullScreenAd{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    DebugLog(@"YAAY PLAY HAVEN");
    dispatch_async(dispatch_get_main_queue(), ^{
        PHPublisherContentRequest * request = [PHPublisherContentRequest requestForApp:kPlayHavenAppToken secret:kPlayHavenSecret placement:kPlayHavenPlacement delegate:self];
        [request setShowsOverlayImmediately:YES];
        [request setAnimated:YES];
        [request send];
    });
   
}

- (BOOL)shouldDisplayInterstitial:(NSString *)location {
    NSLog(@"about to display interstitial at location %@", location);
    
    // For example:
    // if the user has left the main menu and is currently playing your game, return NO;
    
    // Otherwise return YES to display the interstitial
    isAdVisible = YES;
    return YES;
}


/*
 * didFailToLoadInterstitial
 *
 * This is called when an interstitial has failed to load. The error enum specifies
 * the reason of the failure
 */

- (void)didFailToLoadInterstitial:(NSString *)location withError:(CBLoadError)error {
    
    callBackCount++;
    if(callBackCount == 1){
        firstCallBackTime = [[NSDate date] timeIntervalSince1970];
        [self.delegate chartBoostFullScreenDidFailToLoad:self];
    }
    else if (callBackCount == 2){
        NSDate *now = [NSDate date];
        double end = [now timeIntervalSince1970];
        double difference = end - firstCallBackTime;
        NSLog(@"Difference between calls is %f", difference);
        if (difference > 7.5) {
            [self.delegate chartBoostFullScreenDidFailToLoad:self];
        }
    }else{
        [self.delegate chartBoostFullScreenDidFailToLoad:self];
    }
    
    switch(error){
        case CBLoadErrorInternetUnavailable: {
            NSLog(@"Failed to load Interstitial, no Internet connection !");
        } break;
        case CBLoadErrorInternal: {
            NSLog(@"Failed to load Interstitial, internal error !");
        } break;
        case CBLoadErrorNetworkFailure: {
            NSLog(@"Failed to load Interstitial, network error !");
        } break;
        case CBLoadErrorWrongOrientation: {
            NSLog(@"Failed to load Interstitial, wrong orientation !");
        } break;
        case CBLoadErrorTooManyConnections: {
            NSLog(@"Failed to load Interstitial, too many connections !");
        } break;
        case CBLoadErrorFirstSessionInterstitialsDisabled: {
            NSLog(@"Failed to load Interstitial, first session !");
        } break;
        case CBLoadErrorNoAdFound : {
            NSLog(@"Failed to load Interstitial, no ad found !");
        } break;
        case CBLoadErrorSessionNotStarted : {
            NSLog(@"Failed to load Interstitial, session not started !");
        } break;
        case CBLoadErrorNoLocationFound : {
            NSLog(@"Failed to load Interstitial, missing location parameter !");
        } break;
        default: {
            NSLog(@"Failed to load Interstitial, unknown error !");
        }
    }
}

/*
 * didCacheInterstitial
 *
 * Passes in the location name that has successfully been cached.
 *
 * Is fired on:
 * - All assets loaded
 * - Triggered by cacheInterstitial
 *
 * Notes:
 * - Similar to this is: (BOOL)hasCachedInterstitial:(NSString *)location;
 * Which will return true if a cached interstitial exists for that location
 */

- (void)didCacheInterstitial:(NSString *)location {
    NSLog(@"interstitial cached at location %@", location);
}


/*
 * didDismissInterstitial
 *
 * This is called when an interstitial is dismissed
 *
 * Is fired on:
 * - Interstitial click
 * - Interstitial close
 *
 */


- (void)didDismissInterstitial:(NSString *)location {
    NSLog(@"dismissed interstitial at location %@", location);
    isAdVisible = NO;
}

/*
 * didFailToLoadMoreApps
 *
 * This is called when the more apps page has failed to load for any reason
 *
 * Is fired on:
 * - No network connection
 * - No more apps page has been created (add a more apps page in the dashboard)
 * - No publishing campaign matches for that user (add more campaigns to your more apps page)
 *  -Find this inside the App > Edit page in the Chartboost dashboard
 */

- (void)didFailToLoadMoreApps:(CBLoadError)error {
    switch(error){
        case CBLoadErrorInternetUnavailable: {
            NSLog(@"Failed to load More Apps, no Internet connection !");
        } break;
        case CBLoadErrorInternal: {
            NSLog(@"Failed to load More Apps, internal error !");
        } break;
        case CBLoadErrorNetworkFailure: {
            NSLog(@"Failed to load More Apps, network error !");
        } break;
        case CBLoadErrorWrongOrientation: {
            NSLog(@"Failed to load More Apps, wrong orientation !");
        } break;
        case CBLoadErrorTooManyConnections: {
            NSLog(@"Failed to load More Apps, too many connections !");
        } break;
        case CBLoadErrorFirstSessionInterstitialsDisabled: {
            NSLog(@"Failed to load More Apps, first session !");
        } break;
        case CBLoadErrorNoAdFound: {
            NSLog(@"Failed to load More Apps, Apps not found !");
        } break;
        case CBLoadErrorSessionNotStarted : {
            NSLog(@"Failed to load More Apps, session not started !");
        } break;
        default: {
            NSLog(@"Failed to load More Apps, unknown error !");
        }
    }
}


/*
 * didDismissMoreApps
 *
 * This is called when the more apps page is dismissed
 *
 * Is fired on:
 * - More Apps click
 * - More Apps close
 *
 */

- (void)didDismissMoreApps:(NSString *)location {
    NSLog(@"dismissed more apps page at location %@", location);
}


//- (void)didFailToLoadInterstitial:(NSString *)location{
//    NSLog(@"%s", __PRETTY_FUNCTION__);
//    // NSLog(@"%@",[NSThread callStackSymbols]);
//    /**
//     On every callback increment the callback count by one
//     on second callback check if the difference between first and second call is more than 1.5 sec
//     If its more than 4.5 than most probably its a genuine failure callback
//     else if its less than 4.5 just ignore it
//     To make things quicker and not having to calculate nsdate instances everytime we're placing them in if else statements with respect to the callback counters.
//     */
//    callBackCount++;
//    if(callBackCount == 1){
//       firstCallBackTime = [[NSDate date] timeIntervalSince1970];
//        [self.delegate chartBoostFullScreenDidFailToLoad:self];
//    }
//    else if (callBackCount == 2){
//        NSDate *now = [NSDate date];
//        double end = [now timeIntervalSince1970];
//        double difference = end - firstCallBackTime;
//        NSLog(@"Difference between calls is %f", difference);
//        if (difference > 7.5) {
//            [self.delegate chartBoostFullScreenDidFailToLoad:self];
//        }
//    }else{
//        [self.delegate chartBoostFullScreenDidFailToLoad:self];
//    }
//        
//    
//}
//- (BOOL)shouldDisplayInterstitial:(NSString *)location{
//    isAdVisible = YES;
//    return YES;
//}
//- (BOOL)shouldDisplayLoadingViewForMoreApps{
//    return YES;
//}
//
//- (void)didCloseInterstitial:(NSString *)location{
//    isAdVisible = NO;
//}
//- (void)didClickInterstitial:(NSString *)location{
//    
//}

#pragma mark -
#pragma mark Play Haven

-(void)request:(PHPublisherContentRequest *)request contentDidDisplay:(PHContent *)content{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self.playHavenTimer invalidate];
    self.playHavenTimer = nil;
    isAdVisible = YES;
    [self.delegate playHavenFullScreenDidLoadAd:self];
    
}
- (void)request:(PHPublisherContentRequest *)request contentDidDismissWithType:(PHPublisherContentDismissType *)type{
    isAdVisible = NO;
}

-(void)request:(PHPublisherContentRequest *)request didFailWithError:(NSError *)error{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    if (self.hasPlayHavenAdLoaded) {
        return;
    }
    self.hasPlayHavenAdLoaded = YES;
    [self.delegate playHavenFullScreenDidFailToLoad:self];
}


-(void)requestDidGetContent:(PHPublisherContentRequest *)request{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    self.hasPlayHavenAdLoaded = YES;
  //  isAdVisible = YES;
}

-(void)requestWillGetContent:(PHPublisherContentRequest *)request{
    NSLog(@"%s", __PRETTY_FUNCTION__);
   // if (self.playHavenTimer == nil) {
        self.playHavenTimer = [NSTimer scheduledTimerWithTimeInterval:kPlayHavenAdTimeOutThresholdValue target:self selector:@selector(request:didFailWithError:) userInfo:nil repeats:NO];
        self.hasPlayHavenAdLoaded = NO;
        DebugLog(@"PlayHaven resheduled");
   // }
    
}

#pragma mark -
#pragma mark AppLovin Methods
-(void) ad:(ALAd *) ad wasDisplayedIn: (ALAdView *)view;
{
    // Resize main view to give room for the Ad view
    /*CGRect mainViewFrame = CGRectMake( 0,
                                      0,
                                      [SNAdsManager getRootViewController].view.frame.size.width,
                                      [SNAdsManager getRootViewController].view.frame.size.height - adView.frame.size.height );
    [UIView animateWithDuration: 0.01
                          delay: 0.0
                        options: UIViewAnimationOptionCurveLinear
                     animations: ^{
                         [SNAdsManager getRootViewController].view.frame = mainViewFrame;
                     }
                     completion: NULL];
     */
    
    isAppLovinWorking = YES;
    if (self.adType == kBannerAd) {
        [self.delegate appLovinBannerDidLoadAd:self];
    }else if(self.adType == kFullScreenAd) {

        if (isAdVisible) {
            [[ALInterstitialAd shared] dismiss];
            NSLog(@" APPLOVIN DISMSSSED MANUALLY");
            isAdVisible = NO;
        }else{
            NSLog(@"FULLSCREEN APPLOVIN SUCCESS");
            isAdVisible = YES;
        }
        [self.delegate appLovinFullScreenDidLoadAd:self];
    }
}

/**
 * This method is invoked when the ad is hidden from in the view. This occurs
 * when the ad is rotated or when it is explicitly closed.
 *
 * @param ad     Ad that was just hidden. Guranteed not to be null.
 * @param view   Ad view in which the ad was hidden. It will be this controller's view.
 */
-(void) ad:(ALAd *) ad wasHiddenIn: (ALAdView *)view
{
    // Resize main view to fill the whole screen
//    CGRect mainViewFrame = CGRectMake( 0,
//                                      0,
//                                      [SNAdsManager getRootViewController].view.frame.size.width,
//                                      [SNAdsManager getRootViewController].view.frame.size.height );
//    [UIView animateWithDuration: 0.01
//                          delay: 0.0
//                        options: UIViewAnimationOptionCurveLinear
//                     animations: ^{
//                         [SNAdsManager getRootViewController].view.frame = mainViewFrame;
//                     }
//                     completion: NULL];
    isAdVisible = NO;
    isAppLovinWorking = YES;
    if (self.adType == kFullScreenAd){
        if([self.AppLovinTimer isValid]){
            [self.AppLovinTimer invalidate];
            self.AppLovinTimer = nil;
        }
    }
}


-(void)ad:(ALAd *)ad wasClickedIn:(UIView *)view
{
    NSLog(@"wasClickedIn");
    isAppLovinWorking = YES;
}
-(void)adService:(ALAdService *)adService didLoadAd:(ALAd *)ad{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    NSLog(@"DIDLAOD APP LOVIN");
    isAppLovinWorking = YES;
    
    if (self.adType == kBannerAd) {
        //   [AppLovinTimer invalidate];
        NSLog(@"Banner APPLOVIN SUCCESS");
    }else if (self.adType == kFullScreenAd){
        if([self.AppLovinTimer isValid]){
            [self.AppLovinTimer invalidate];
            self.AppLovinTimer = nil;
        }
        
        
    }
}

-(void)adService:(ALAdService *)adService didFailToLoadAdWithError:(int)code{
    isAppLovinWorking = YES;
    NSLog(@"%s", __PRETTY_FUNCTION__);
    if (self.adType == kBannerAd) {
        [self.delegate appLovinBannerDidFailToLoad:self];
        NSLog(@"Banner APPLOVIN FAILED");
    }else if (self.adType == kFullScreenAd){
        NSLog(@"FULLSCREEN APPLOVIN FAILED");
        
        if (self.adType == kFullScreenAd){
            if([self.AppLovinTimer isValid]){
                [self.delegate appLovinFullScreenDidFailToLoad:self];
                [self.AppLovinTimer invalidate];
                self.AppLovinTimer = nil;
            }else{
                return;
            }
        }
        
    }
    
}
-(void)hideBannerAppLovinAd{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self.adView removeFromSuperview];
}
@end
