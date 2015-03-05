//
//  SettingsManager.h
//  Christmas Countdown
//
//  Created by Asad Khan on 25/10/2012.
//  Copyright (c) 2012 Semantic Notion. All rights reserved.
//

#import <Foundation/Foundation.h>
@class RootViewController;

@interface SettingsManager : NSObject

enum ratedStatus{
    kAppHasNotBeenRated,
    kAppHasBeenRated,
    kMaybeLater,
    kNever
};



//Device Check Boolean Variables
@property(nonatomic, assign)BOOL isIPhone5;
@property(nonatomic, assign)BOOL isIPhone;
@property(nonatomic, assign)BOOL isIPad;
@property(nonatomic, strong)RootViewController *rootViewController;
@property(nonatomic, assign)BOOL hasInAppPurchaseBeenMade;
@property(nonatomic, assign)NSInteger gameOverCounter;
@property(nonatomic, assign)NSInteger ammoAtHand;

+ (SettingsManager *)sharedManager;

@end
