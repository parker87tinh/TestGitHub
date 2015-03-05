//
//  SNAdsManager.m
//  Alien Tower
//
//  Created by Asad Khan on 05/02/2013.
//
//

#import "SNAdsManager.h"
#import "GenericAd.h"
#import "Reachability.h"
#import "SNQueue.h"
#import <AdSupport/ASIdentifierManager.h>



static NSUInteger gameOverCount = 0;
static BOOL isBannerAdAtTop = NO;

@interface SNAdsManager()


@property (nonatomic, strong)NSArray *sortedBannerAdsArray;
@property (nonatomic, assign)NSUInteger currentBannerAdIndex;
@property (nonatomic, strong)NSArray *sortedFullScreenAdsArray;
@property (nonatomic, assign)NSUInteger currentSortedFullScreenAdIndex;

@property (nonatomic, assign)BOOL haveAdNetworksInitialized;
@property (nonatomic, strong)SNQueue *adRequestQueue;

- (void)loadAdWithLowerPriorityThanPreviousAd:(GenericAd*)ad;
- (void)loadBannerAdWithLowerPriorityhanPreviousAd:(GenericAd*)previousFailedAd;
- (void)loadFullscreenAdWithLowerPriorityThanPreviousAd:(GenericAd*)previousFailedAd;
- (void)loadLinkAdWithLowerPriorityThanPreviousAd:(GenericAd*)previousFailedAd;
- (void)failGracefully:(GenericAd*)previousAd;

- (ConnectionStatus)isReachableVia;
- (void) initializeAdNetworks;
- (void)startMobclix;
- (void)startRevMob;
- (void)startChartBoost;



@end


@implementation SNAdsManager

@synthesize myConnectionStatus = _myConnectionStatus;
@synthesize genericAd = _genericAd;
@synthesize currentAdsBucketArray = _currentAdsBucketArray;
@synthesize chartBoost = _chartBoost;
@synthesize sortedBannerAdsArray = _sortedBannerAdsArray;
@synthesize sortedFullScreenAdsArray = _sortedFullScreenAdsArray;
@synthesize currentBannerAdIndex;
@synthesize currentSortedFullScreenAdIndex;
@synthesize haveAdNetworksInitialized = _haveAdNetworksInitialized;
@synthesize adRequestQueue = _adRequestQueue;


static int callBackCount = 0; //KVO count for NSOperation objects in Ad Networks initialization

#pragma mark -
#pragma mark Singleton Methods

static SNAdsManager *sharedManager = nil;

+ (SNAdsManager*)sharedManager{
    if (sharedManager != nil)
    {
        return sharedManager;
    }
    
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_4_0
    static dispatch_once_t safer;
    dispatch_once(&safer, ^(void)
                  {
                      sharedManager = [[SNAdsManager alloc] init];
                      
                  });
#else
    @synchronized([SNAdsManager class])
    {
        if (sharedManager == nil)
        {
            sharedManager = [[SNAdsManager alloc] init];
            
        }
    }
#endif
    return sharedManager;
}



- (id)copyWithZone:(NSZone *)zone{
	return self;
}

- (id) init{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
	self = [super init];
	if(self !=nil){
        self.myConnectionStatus = [self isReachableVia];

        
        _currentAdsBucketArray = [[NSMutableArray alloc] init];
        //if(self.myConnectionStatus != kNotReachable){
            [self initializeAdNetworks];
        //}else{
//            NSLog(@"!!!Offline!!!");
        //}
	}
	return self;
	
}//end init

- (void)dealloc {
	NSLog(@"dealloc called");
    return;
    [super dealloc];
}

- (void)fetchAds{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    
    GenericAd *chartBoostFullScreenAd = [[GenericAd alloc] initWithAdNetworkType:kChartBoost andAdType:kFullScreenAd];
    chartBoostFullScreenAd.delegate = self;
    [self.currentAdsBucketArray addObject:chartBoostFullScreenAd];
    
    GenericAd *chartBoostMoreAppsAd = [[GenericAd alloc] initWithAdNetworkType:kChartBoost andAdType:kMoreAppsAd];
    chartBoostMoreAppsAd.delegate = self;
    [self.currentAdsBucketArray addObject:chartBoostMoreAppsAd];
    
    GenericAd *playHavenFullScreenAd = [[GenericAd alloc] initWithAdNetworkType:kPlayHaven andAdType:kFullScreenAd];
    playHavenFullScreenAd.delegate = self;
    [self.currentAdsBucketArray addObject:playHavenFullScreenAd];
    
    GenericAd *appLovinBannerAd = [[GenericAd alloc] initWithAdNetworkType:kAppLovin andAdType:kBannerAd];
    appLovinBannerAd.delegate = self;
    [self.currentAdsBucketArray addObject:appLovinBannerAd];
    
    GenericAd *appLovinFullScreenAd = [[GenericAd alloc] initWithAdNetworkType:kAppLovin andAdType:kFullScreenAd];
    appLovinFullScreenAd.delegate = self;
    [self.currentAdsBucketArray addObject:appLovinFullScreenAd];
    
    if ([self.adRequestQueue count] >= 1) {
        [self processAdQueue];
    }
}
+ (UIViewController *)getRootViewController{
    UIViewController *rootViewController =  [UIApplication sharedApplication].keyWindow.rootViewController;
    return rootViewController;
}

/**
 @brief returns the if device is a simulator or not
 @param None
 */
-(BOOL)isSimulator
{
    NSRange textRange;
    textRange =[[UIDevice currentDevice].model rangeOfString:@"simulator"];
    if(textRange.location != NSNotFound)
    {
        return NO;
    }else
        return YES;
}
- (ConnectionStatus)isReachableVia{
    Reachability *r = [Reachability reachabilityWithHostName:@"http://www.apple.com"];
    NetworkStatus internetStatus = [r currentReachabilityStatus];
    if(internetStatus == ReachableViaWiFi)
        return kWifiAvailable;
    else if (internetStatus == ReachableViaWWAN)
        return kWANAvailable;
    else
        return kNotReachable;
}

#pragma mark -
#pragma mark General Methods
- (void)initializeAdNetworks{
    
    self.haveAdNetworksInitialized = NO;
    self.adRequestQueue = [[SNQueue alloc] init];
    
   
    NSBlockOperation *startChartBoostAdsOperation = [NSBlockOperation blockOperationWithBlock:^{
           //[self startChartBoost];
        /**
         Chartboost if initialised on background thread automatically calls didFailToLoadInterstitial:
         Which is very wiered.
         Chartboosts tech support for now recommends to wrap everything within main dispatch queue in GCD
         in other words run on main thread.
         https://www.evernote.com/shard/s51/sh/b44435f4-49ae-498d-898f-1497d223152e/724c886f1abb09016c3bc4178e33ef28
         */
        dispatch_async(dispatch_get_main_queue(), ^{
            [self startChartBoost];
        });
    }];
    [startChartBoostAdsOperation setQueuePriority:NSOperationQueuePriorityVeryHigh];
    [startChartBoostAdsOperation addObserver:self forKeyPath:@"isFinished" options:NSKeyValueObservingOptionNew context:nil];
    NSBlockOperation *startAppLovinAdsOperation = [NSBlockOperation blockOperationWithBlock:^{
        [ALSdk initializeSdk];
    }];
    [startAppLovinAdsOperation addObserver:self forKeyPath:@"isFinished" options:NSKeyValueObservingOptionNew context:nil];
    
    NSBlockOperation *startPlayHavenAdsOperation = [NSBlockOperation blockOperationWithBlock:^{
        [[PHPublisherOpenRequest requestForApp:kPlayHavenAppToken secret:kPlayHavenSecret] send];
    }];
    [startPlayHavenAdsOperation addObserver:self forKeyPath:@"isFinished" options:NSKeyValueObservingOptionNew context:nil];
    
    NSBlockOperation *revMobAdsOperation = [NSBlockOperation blockOperationWithBlock:^{
        dispatch_async(dispatch_get_main_queue(), ^{
            [self startRevMob];
        });
    }];
    [startChartBoostAdsOperation setQueuePriority:NSOperationQueuePriorityVeryHigh];
    [startChartBoostAdsOperation addObserver:self forKeyPath:@"isFinished" options:NSKeyValueObservingOptionNew context:nil];
   
    
    NSOperationQueue *adNetwroksInitializationQueue = [[NSOperationQueue alloc] init];
    [adNetwroksInitializationQueue addOperation:startChartBoostAdsOperation];
    [adNetwroksInitializationQueue addOperation:startPlayHavenAdsOperation];
    [adNetwroksInitializationQueue addOperation:startAppLovinAdsOperation];
    [adNetwroksInitializationQueue addOperation:revMobAdsOperation];
    

    
    if (self.myConnectionStatus == kWANAvailable)
        [adNetwroksInitializationQueue setMaxConcurrentOperationCount:2];
    else if (self.myConnectionStatus == kWifiAvailable)
        [adNetwroksInitializationQueue setMaxConcurrentOperationCount:NSOperationQueueDefaultMaxConcurrentOperationCount];
}

- (void)addAdRequestToQueue:(SEL)selector{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    [self.adRequestQueue push:[NSValue valueWithPointer:selector]];
    NSLog(@"Contents of queue %@", self.adRequestQueue);
}

-(void)processAdQueue{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    if (![self.adRequestQueue count] >= 1) {
        return;
    }
    NSOperationQueue *adProcessQueue = [[NSOperationQueue alloc] init];
    for (int i = 0; i <= [self.adRequestQueue count]; i++) {
        SEL selector = [[self.adRequestQueue pop] pointerValue];
        NSAssert(selector, @"Selector cannot be nil");
        NSInvocationOperation *operation = [[NSInvocationOperation alloc] initWithTarget:self selector:selector object:nil];
        [adProcessQueue addOperation:operation];
        [operation release];
    }
}

- (void) observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context {
    NSLog(@"%s", __PRETTY_FUNCTION__);
   // NSLog(@"object %@", change);
    if ([keyPath isEqualToString:@"isFinished"]) {
        callBackCount++;
        if (callBackCount >= kNumberOfAdNetworks) {
            //[self fetchAds];
            /**
             Again RevMob is bit of a bitch crashing on other threads
             Once they fix it remove all this code and leave [self fetchAds];
             */
            dispatch_async(dispatch_get_main_queue(), ^{
                [self fetchAds];
                self.haveAdNetworksInitialized = YES;
            });
            
        }
    }
}

- (void)startChartBoost{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    
    [Chartboost startWithAppId:ChartBoostAppID appSignature:ChartBoostAppSignature delegate:self];
    [Chartboost cacheInterstitial:CBLocationHomeScreen];
    [Chartboost cacheMoreApps:CBLocationHomeScreen];
    
}

- (void)startRevMob{
     [RevMobAds startSessionWithAppID:kRevMobID];
}


- (void)loadAdWithLowerPriorityThanPreviousAd:(GenericAd*)previousFailedAd{
    switch (previousFailedAd.adType) {
        case kBannerAd:
            [self loadBannerAdWithLowerPriorityhanPreviousAd:previousFailedAd];
            break;
        case kFullScreenAd:
            [self loadFullscreenAdWithLowerPriorityThanPreviousAd:previousFailedAd];
            break;
        case kLinkAd:
            [self loadLinkAdWithLowerPriorityThanPreviousAd:previousFailedAd];
        default:
            break;
    }
}

- (void)loadBannerAdWithLowerPriorityhanPreviousAd:(GenericAd*)previousFailedAd{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    if (self.currentBannerAdIndex == [self.sortedBannerAdsArray count] - 1) {
        [self failGracefully:previousFailedAd];
        self.currentBannerAdIndex = 0;
    }else{
        GenericAd *genericAd = [self.sortedBannerAdsArray objectAtIndex:self.currentBannerAdIndex + 1];
        if (!genericAd) {
            [self loadSortedBannerAdsArray];
            @try {
                genericAd = [self.sortedBannerAdsArray objectAtIndex:self.currentBannerAdIndex + 1];
            }
            @catch (NSException *exception) {
                [self failGracefully:previousFailedAd];
            }            
        }
        isBannerAdAtTop ? [genericAd showBannerAdAtTop] : [genericAd showBannerAd];
        
        self.genericAd = genericAd;
        self.currentBannerAdIndex++;
    }
}

- (void)loadFullscreenAdWithLowerPriorityThanPreviousAd:(GenericAd*)previousFailedAd{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    if (self.currentSortedFullScreenAdIndex == [self.sortedFullScreenAdsArray count] - 1) {
        [self failGracefully:previousFailedAd];
        self.currentSortedFullScreenAdIndex = 0;//Reset the counter back to ZERO else it would keep failing
    }else{
        GenericAd *genericAd = [self.sortedFullScreenAdsArray objectAtIndex:self.currentSortedFullScreenAdIndex + 1];
       // NSAssert(genericAd, @"Ad cannot be NULL");
        if (!genericAd) {
            [self loadSortedFullScreenAdsArray];
            genericAd = [self.sortedFullScreenAdsArray objectAtIndex:self.currentSortedFullScreenAdIndex + 1];
        }
        genericAd.delegate = self;
        [genericAd showFullScreenAd];
        self.currentSortedFullScreenAdIndex++;
    }
}

- (void)loadSortedFullScreenAdsArray{
    NSMutableArray *fullScreenAdsInBucket;
    
    fullScreenAdsInBucket = [self.currentAdsBucketArray mutableCopy];
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"adType == %d", kFullScreenAd];
    [fullScreenAdsInBucket filterUsingPredicate:predicate];
    
    NSSortDescriptor *sortDescriptor;
    sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"adPriority"
                                                  ascending:NO] autorelease];
    NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
    NSArray *sortedArray;
    sortedArray = [fullScreenAdsInBucket sortedArrayUsingDescriptors:sortDescriptors];
    
    self.sortedFullScreenAdsArray = sortedArray;
}

- (void)loadSortedBannerAdsArray{
    NSMutableArray *bannerAdsInBucket;
    
    bannerAdsInBucket = [self.currentAdsBucketArray mutableCopy];
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"adType == %d", kBannerAd];
    [bannerAdsInBucket filterUsingPredicate:predicate];
    
    NSSortDescriptor *sortDescriptor;
    sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"adPriority"
                                                  ascending:NO] autorelease];
    NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
    NSArray *sortedArray;
    sortedArray = [bannerAdsInBucket sortedArrayUsingDescriptors:sortDescriptors];
    
    self.sortedBannerAdsArray = sortedArray;

}
-(void)loadLinkAdWithLowerPriorityThanPreviousAd:(GenericAd*)previousFailedAd{
     [self failGracefully:previousFailedAd];
}


- (void)failGracefully:(GenericAd*)previousAd{
    //There is nothing graceful than doing nothing
    switch (previousAd.adType) {
        case kBannerAd:
            if ([self.delegate respondsToSelector:@selector(bannerAdDidFailToLoad)]) {
                NSLog(@"All attempts to Banner load Ad failed");
                [self.delegate bannerAdDidFailToLoad];
            }
            break;
        case kFullScreenAd:
            if ([self.delegate respondsToSelector:@selector(fullScreenAdDidFailToLoad)]) {
                [self.delegate fullScreenAdDidFailToLoad];
                NSLog(@"All attempts to load FullScreen Ad failed");
            }
            break;
        case kLinkAd:
            if ([self.delegate respondsToSelector:@selector(linkAdDidFailToLoad)]) {
                [self.delegate linkAdDidFailToLoad];
            }
            //[self loadLinkAdWithLowerPriority];
        default:
            NSLog(@"Ad Type undefined");
            break;
    }
}

- (void) giveMeBannerAd{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    isBannerAdAtTop = NO;
    GenericAd *ad;
    NSMutableArray *bannerAdsInBucket;
  
    bannerAdsInBucket = [self.currentAdsBucketArray mutableCopy];
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"adType == %d", kBannerAd];
    [bannerAdsInBucket filterUsingPredicate:predicate];
    
    NSSortDescriptor *sortDescriptor;
    sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"adPriority"
                                                  ascending:NO] autorelease];
    NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
    NSArray *sortedArray;
    sortedArray = [bannerAdsInBucket sortedArrayUsingDescriptors:sortDescriptors];
    
    self.sortedBannerAdsArray = sortedArray;
    ad = [self.sortedBannerAdsArray objectAtIndex:0];
    self.genericAd = ad;
    self.genericAd.delegate = self;
    if ([ad respondsToSelector:@selector(showBannerAd)]) {
        [ad showBannerAd];
    }
    
    self.currentBannerAdIndex = 0;
}

- (void) giveMeBannerAdAtTop{
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    isBannerAdAtTop = YES;
    GenericAd *ad;
    NSMutableArray *bannerAdsInBucket;
    
    bannerAdsInBucket = [self.currentAdsBucketArray mutableCopy];
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"adType == %d", kBannerAd];
    [bannerAdsInBucket filterUsingPredicate:predicate];
    
    NSSortDescriptor *sortDescriptor;
    sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"adPriority"
                                                  ascending:NO] autorelease];
    NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
    NSArray *sortedArray;
    sortedArray = [bannerAdsInBucket sortedArrayUsingDescriptors:sortDescriptors];
    
    self.sortedBannerAdsArray = sortedArray;
    ad = [sortedArray objectAtIndex:0];
   
    self.genericAd = ad;
    self.genericAd.delegate = self;
    if ([ad respondsToSelector:@selector(showBannerAdAtTop)]) {
        [ad showBannerAdAtTop];
    }
    
    self.currentBannerAdIndex = 0;
}
- (void) giveMeFullScreenAd{
    
    DebugLog(@"%s", __PRETTY_FUNCTION__);
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    GenericAd *ad;
    NSMutableArray *fullScreenAdsInBucket;
   
    fullScreenAdsInBucket = [self.currentAdsBucketArray mutableCopy];
    NSPredicate *predicate = [NSPredicate predicateWithFormat:@"adType == %d", kFullScreenAd];
    [fullScreenAdsInBucket filterUsingPredicate:predicate];
    
    NSSortDescriptor *sortDescriptor;
    sortDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"adPriority"
                                                  ascending:NO] autorelease];
    NSArray *sortDescriptors = [NSArray arrayWithObject:sortDescriptor];
    NSArray *sortedArray;
    sortedArray = [fullScreenAdsInBucket sortedArrayUsingDescriptors:sortDescriptors];
  
    self.sortedFullScreenAdsArray = sortedArray;
    ad = [self.sortedFullScreenAdsArray objectAtIndex:0];
    if ([ad respondsToSelector:@selector(showFullScreenAd)]) {
        ad.delegate = self;
        [ad showFullScreenAd];
    }
    self.currentSortedFullScreenAdIndex = 0;
}
- (void) giveMeLinkAd{
//    NSString *idfaString = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
//    NSString *link = [NSString stringWithFormat:@"http://jmplead.com/applnk/%@?devad_id=%@", kLeadBoltLinkAdID,idfaString];
//    DebugLog(@"%@", link);
//    NSURL *url = [NSURL URLWithString:link];
//    NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
//    
//    UIWebView *appWebView = [[UIWebView alloc] init];
//    [appWebView setScalesPageToFit:YES];
//    [appWebView loadRequest:requestObj];
    [[RevMobAds session] openAdLinkWithDelegate:nil];
    
    //[[UIApplication sharedApplication] openURL: [NSURL URLWithString:link]];
}

- (void)giveMeGameOverAd{
    [self giveMeFullScreenAd];
   // [self giveMeFullScreenRevMobAd];
}

- (void)giveMeThirdGameOverAd{

    gameOverCount++;
    NSLog(@"game over counter %d", gameOverCount);
    if (gameOverCount %3 == 0){
        [self giveMeFullScreenAd];
       // [self giveMeFullScreenRevMobAd];
    }
}

- (void)giveMeBootUpAd{
    [self giveMeFullScreenAd];
   // [self giveMeFullScreenRevMobAd];
}
- (void)giveMeWillEnterForegroundAd{
    [[PHPublisherOpenRequest requestForApp:kPlayHavenAppToken secret:kPlayHavenSecret] send];
    [self giveMeFullScreenAd];
  //  [self giveMeFullScreenRevMobAd];
}
-(void) giveMePaidFullScreenAd{
    [self giveMeFullScreenChartBoostAd];
}

- (void) hideBannerAd{

    [self.genericAd hideBannerAppLovinAd];//AppLovin hide Ad
}

#pragma mark -
#pragma mark Charboost Ads
- (void) giveMeFullScreenChartBoostAd{
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        
        
        [Chartboost showInterstitial:CBLocationHomeScreen];
//        self.chartBoost.delegate = nil;
//        [self.chartBoost showInterstitial];
    });
    
}
- (void)giveMeMoreAppsAd{
    [self giveMeFullScreenChartBoostAd];
//    [self giveMeLinkAd];
}
- (void) giveMeMoreAppsChartBoostAd{
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    
    [Chartboost showMoreApps:CBLocationHomeScreen];
    //[self.chartBoost showMoreApps];
}
#pragma mark Delegate Methods


#pragma mark -
#pragma mark AppLovin delegates
- (void)giveMeFullScreenAppLovinAd{
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    /*GenericAd *genericAd = [[GenericAd alloc] initWithAdNetworkType:kAppLovin andAdType:kFullScreenAd];
    [genericAd showFullScreenAd];
    [genericAd release];*/
    dispatch_async(dispatch_get_main_queue(), ^{
        [ALInterstitialAd showOver:[UIApplication sharedApplication].keyWindow];
        [ALInterstitialAd shared].adLoadDelegate = nil;
        [ALInterstitialAd shared].adDisplayDelegate = nil;
    });
   
}

- (void)giveMeBannerAppLovinAd{
    if (!self.haveAdNetworksInitialized) {
        [self addAdRequestToQueue:_cmd];
        return;
    }
    GenericAd *genericAd = [[GenericAd alloc] initWithAdNetworkType:kAppLovin andAdType:kBannerAd];
    [genericAd showBannerAd];
    [genericAd release];
}


- (void)chartBoostFullScreenDidFailToLoad:(GenericAd *)ad{
    NSLog(@"%s", __PRETTY_FUNCTION__);
    [self loadFullscreenAdWithLowerPriorityThanPreviousAd:ad];
    if ([self.delegate respondsToSelector:@selector(chartBoostFullScreenDidFailToLoad)]) {
        [self.delegate chartBoostFullScreenDidFailToLoad];
    }
}

- (void)revMobFullScreenDidLoadAd:(GenericAd *)ad{
    if ([self.delegate respondsToSelector:@selector(fullScreenAdDidLoad)]) {
        [self.delegate fullScreenAdDidLoad];
    }
}

- (void)chartBoostFullScreenDidLoadAd:(GenericAd *)ad{
    if ([self.delegate respondsToSelector:@selector(fullScreenAdDidLoad)]) {
        [self.delegate fullScreenAdDidLoad];
    }
}


- (void)playHavenFullScreenDidLoadAd:(GenericAd *)ad{
    if ([self.delegate respondsToSelector:@selector(playHavenFullScreenDidLoad)]) {
        [self.delegate playHavenFullScreenDidLoad];
    }

}
- (void)playHavenFullScreenDidFailToLoad:(GenericAd *)ad{
    [self loadFullscreenAdWithLowerPriorityThanPreviousAd:ad];
    if ([self.delegate respondsToSelector:@selector(playHavenFullScreenDidFailToLoad)]) {
        [self.delegate playHavenFullScreenDidFailToLoad];
    }
}

- (void)appLovinFullScreenDidLoadAd:(GenericAd *)ad{
    if ([self.delegate respondsToSelector:@selector(fullScreenAdDidLoad)]) {
        [self.delegate fullScreenAdDidLoad];
    }
}
- (void)appLovinBannerDidLoadAd:(GenericAd *)ad{
    if ([self.delegate respondsToSelector:@selector(bannerAdDidLoad)]) {
        [self.delegate bannerAdDidLoad];
    }
}
- (void)appLovinBannerDidFailToLoad:(GenericAd *)ad{
    [self loadBannerAdWithLowerPriorityhanPreviousAd:ad];
    if ([self.delegate respondsToSelector:@selector(appLovinBannerAdDidFailToLoad)]) {
        [self.delegate appLovinBannerAdDidFailToLoad];
    }
}
- (void)appLovinFullScreenDidFailToLoad:(GenericAd *)ad{
    [self loadFullscreenAdWithLowerPriorityThanPreviousAd:ad];
    if ([self.delegate respondsToSelector:@selector(appLovinFullScreenAdDidFailToLoad)]) {
        [self.delegate appLovinFullScreenAdDidFailToLoad];
    }
}
@end
