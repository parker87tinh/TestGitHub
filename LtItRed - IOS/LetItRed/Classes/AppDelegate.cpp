//
//  LetItRideAppDelegate.cpp
//  LetItRide
//
//  Created by lion on 1/27/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "PlayLayer.h"

#include "GameSetting.h"

#include "MainMenuLayer.h"
#include "LetItRideSetting.h"

bool g_fIsRetinaDisable = false;
bool g_fIsIPhoneFive = false;
float g_rX = 1.0f;
float g_rY = 1.0f;
int g_nRXForIFive = 0;
bool m_fHighRes = false;
bool isIpad = false;

float g_rScale = 1.0f; // iPad, iphone Scale Rate

int g_nYourCoin;

bool g_bPause;
bool g_bBuy;
bool g_bMainMenu;

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    m_fHighRes = false;
    
    //	// Enables High Res mode (Retina Display) on iPhone 4 and maintains low res on all other devices
    g_fIsIPhoneFive = false;
    
    g_fIsRetinaDisable = false;
    
    cocos2d::TargetPlatform target = cocos2d::CCApplication::sharedApplication()->getTargetPlatform();
    if( target == cocos2d::kTargetIpad)
    {
        g_rX = 1.0f;
        g_rY = 1.0f;
        
        g_rScale = 1.0f;
        isIpad = true;
        CCSize result = pDirector->getWinSize();
        if ( result.width == 2048 )
        {
            m_fHighRes = true;
            pDirector->setContentScaleFactor(2);
            CCEGLView::sharedOpenGLView()->setContentScaleFactor(2);
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 768, kResolutionFixedWidth);
        }
        else
        {
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 768, kResolutionFixedWidth);
        }
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 768, kResolutionFixedWidth);
    }
    else
    {
        CCSize result = pDirector->getWinSize();
        isIpad = false;
        if(result.width == 1136)
        {
            g_fIsIPhoneFive = true;
            
            g_nRXForIFive = 44;
            
            g_rX = 568.0/1024.0;
            g_rY = 320.0/768.0;
            
            g_rScale = 1.0f;
        }
        else
        {
            g_rX = 480.0/1024.0;
            g_rY = 320.0/768.0;
            
            g_rScale = 1.0f;
        }
        
        if ( result.width < 960 )
        {
            g_fIsRetinaDisable = true;
            pDirector->setContentScaleFactor(1);
            CCEGLView::sharedOpenGLView()->setContentScaleFactor(1);
            CCLOG("Retina Display Not supported");
        }
        else //if( CC_CONTENT_SCALE_FACTOR() == 2 )
        {
            pDirector->setContentScaleFactor(2);
            CCEGLView::sharedOpenGLView()->setContentScaleFactor(2);
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(480, 320, kResolutionFixedHeight);
            
            m_fHighRes = true;
        }
        
    }
    
    GameSetting::shared()->load();
    
#ifdef FreeApp
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
    if (!GameSetting::shared()->getRemoveAd())
    {
        bj->giveMeBootUpAd();
    }
#endif
    
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainMenuLayer::scene();

    // run
    pDirector->runWithScene(pScene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
    bj->locallyNotifyWithText("Not seen you for a while. Let's play!!");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();

#ifdef FreeApp
    if (!GameSetting::shared()->getRemoveAd())
    {
        bj->giveMeWillEnterForegroundAd();
    }
#endif
    
    bj->RateMe();
}
