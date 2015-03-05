//
//  MainMenuLayer.cpp
//  LetItRide
//
//  Created by lion on 2/17/14.
//
//

#include "MainMenuLayer.h"
#include "SimpleAudioEngine.h"

#include "Tools.h"
#include "Extern.h"

#include "GameSetting.h"
#include "PlayLayer.h"

#include "LetItRideSetting.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MainMenuLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenuLayer *layer = MainMenuLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

MainMenuLayer::~MainMenuLayer()
{
    stopAllActions();
    removeAllChildren();
}

// on "init" you need to initialize your instance
bool MainMenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    this->setTouchEnabled(true);
    this->initVariable();
    this->initImages();
    this->initItems();
    this->initLabels();
    
    bool bMusic = GameSetting::shared()->isEnableMusic();
    GameSetting::shared()->saveMusic(bMusic);
    if ( bMusic )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic( "main_music.caf" , true);
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
    
    return true;
}

void MainMenuLayer::initVariable()
{
    winSize = CCDirector::sharedDirector()->getWinSize();
}

void MainMenuLayer::initImages()
{
    CCString* strBk = Tools::imageName("main_bg");
    if ( g_fIsIPhoneFive )
    {
        strBk = CCString::create("main_bg-1136.png");
    }
    CCSprite* spBk = CCSprite::create(strBk->getCString());
    spBk->setPosition(ccp( winSize.width/2, winSize.height/2 ));
    this->addChild(spBk, 0);
}

void MainMenuLayer::initItems()
{
    itemPlay = CCMenuItemImage::create(Tools::imageName("main_play")->getCString(), Tools::imageName("main_play_1")->getCString(), this, menu_selector(MainMenuLayer::onClickPlay));
    itemPlay->setPosition(ccp( winSize.width/2 - 170*g_rX, winSize.height/2 - 50* g_rY));
    
    
    itemFreeGames = CCMenuItemImage::create(Tools::imageName("main_Free_Game")->getCString(), Tools::imageName("main_Free_Game_1")->getCString(), this, menu_selector(MainMenuLayer::onClickFreeGames));
    itemFreeGames->setPosition(ccp(winSize.width/2 - 340*g_rX, winSize.height/2 - 180*g_rY));
    
    
    itemMoreGames = CCMenuItemImage::create(Tools::imageName("main_More_Games")->getCString(), Tools::imageName("main_More_Games_1")->getCString(), this, menu_selector(MainMenuLayer::onClickMoreGames));
    itemMoreGames->setPosition(ccp(winSize.width/2 - 340*g_rX, winSize.height/2 - 300*g_rY));
    
    itemRemoveAds = CCMenuItemImage::create(Tools::imageName("main_Remove_Ads")->getCString(), Tools::imageName("main_Remove_Ads_1")->getCString(), this, menu_selector(MainMenuLayer::onClickRemoveAds));
    itemRemoveAds->setPosition(ccp( winSize.width/2, winSize.height/2 - 180*g_rY ));
    
    itemRestore = CCMenuItemImage::create(Tools::imageName("main_Restore")->getCString(), Tools::imageName("main_Restore_1")->getCString(), this, menu_selector(MainMenuLayer::onClickRestore));
    itemRestore->setPosition(ccp( winSize.width/2, winSize.height/2 - 300*g_rY ));
    itemHighScore = CCMenuItemImage::create(Tools::imageName("main_High_Scores")->getCString(), Tools::imageName("main_High_Scores_1")->getCString(), this, menu_selector(MainMenuLayer::onClickHighScore));
    itemHighScore->setPosition(ccp( winSize.width/2 + 340*g_rX, winSize.height/2 - 300*g_rY));
    
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();

#ifdef PaidApp       
    itemRemoveAds->setVisible(false);
        itemRestore->setVisible(false);
        itemMoreGames->setPosition(itemRemoveAds->getPosition());
        itemHighScore->setPosition(ccp( itemPlay->getPosition().x, itemHighScore->getPosition().y ));
#endif

    
#ifdef FreeApp
    if ( GameSetting::shared()->getRemoveAd() )
    {
        itemRemoveAds->setVisible(false);
        itemRestore->setVisible(false);
        itemMoreGames->setPosition(itemRemoveAds->getPosition());
        itemHighScore->setPosition(ccp( itemPlay->getPosition().x, itemHighScore->getPosition().y ));

        
    }
#endif
    
    bool bMusic = GameSetting::shared()->isEnableMusic();
    if ( bMusic )
    {
        itemMusic = CCMenuItemToggle::createWithTarget(this,
                                                       menu_selector(MainMenuLayer::onClickMusic),
                                                       CCMenuItemImage::create(Tools::imageName("main_music")->getCString(), Tools::imageName("main_music")->getCString()),
                                                       CCMenuItemImage::create(Tools::imageName("main_music_2")->getCString(), Tools::imageName("main_music_2")->getCString()), NULL);
        
    }
    else
    {
        itemMusic = CCMenuItemToggle::createWithTarget(this,
                                                       menu_selector(MainMenuLayer::onClickMusic),
                                                       CCMenuItemImage::create(Tools::imageName("main_music_2")->getCString(), Tools::imageName("main_music_2")->getCString()),
                                                       CCMenuItemImage::create(Tools::imageName("main_music")->getCString(), Tools::imageName("main_music")->getCString()), NULL);
    }
    
    itemMusic->setPosition(ccp( winSize.width - 80*g_rX, winSize.height - 80*g_rY));
    
    bool bSound = GameSetting::shared()->isEnableSound();
    if ( bSound )
    {
        itemSound = CCMenuItemToggle::createWithTarget(this,
                                                       menu_selector(MainMenuLayer::onClickSound),
                                                       CCMenuItemImage::create(Tools::imageName("main_sound")->getCString(), Tools::imageName("main_sound")->getCString()),
                                                       CCMenuItemImage::create(Tools::imageName("main_sound_2")->getCString(), Tools::imageName("main_sound_2")->getCString()), NULL);
    }
    else
    {
        itemSound = CCMenuItemToggle::createWithTarget(this,
                                                       menu_selector(MainMenuLayer::onClickSound),
                                                       CCMenuItemImage::create(Tools::imageName("main_sound_2")->getCString(), Tools::imageName("main_sound_2")->getCString()),
                                                       CCMenuItemImage::create(Tools::imageName("main_sound")->getCString(), Tools::imageName("main_sound")->getCString()), NULL);
    }
    
    itemSound->setPosition(ccp(winSize.width - 80*g_rX, winSize.height - 180*g_rY));
    
    CCMenu* menu = CCMenu::create(itemPlay, itemFreeGames, itemMoreGames, itemRemoveAds, itemRestore, itemHighScore, itemMusic, itemSound, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);
}

void MainMenuLayer::initLabels()
{
    
}

#pragma mark click functions
void MainMenuLayer::onClickPlay(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    CCDirector::sharedDirector()->replaceScene(PlayLayer::scene());
}

void MainMenuLayer::onClickFreeGames(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
//    if ( !GameSetting::shared()->getRemoveAd() )
//    {
//        /if ( !LetItRide::LetItRideSetting::shared()->isPaidVersion() )
//        {
            LetItRide::LetItRideSetting::shared()->giveMeLinkAd();
//        }
//    }
}

void MainMenuLayer::onClickMoreGames(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    
//    if ( !GameSetting::shared()->getRemoveAd() )
//    {
//        if ( !LetItRide::LetItRideSetting::shared()->isPaidVersion() )
//        {
            LetItRide::LetItRideSetting::shared()->giveMeMoreGamesAd();
//        }
//    }
}
void MainMenuLayer::update(float dt)
{
    if ( GameSetting::shared()->getRemoveAd() )
    {
        itemRemoveAds->setVisible(false);
        itemRestore->setVisible(false);
        itemMoreGames->setPosition(itemRemoveAds->getPosition());
        itemHighScore->setPosition(ccp( itemPlay->getPosition().x, itemHighScore->getPosition().y ));
        
        this->unscheduleUpdate();
    }
}
void MainMenuLayer::onClickRemoveAds(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
//    if ( bj->isPaidVersion() )
//    {
//        return;
//    }
    
    bj->onRemoveAds();//m_bRemoveAds
    this->unscheduleUpdate();
    this->scheduleUpdate();
}

void MainMenuLayer::onClickRestore(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
    
    bj->onRestore();
        this->unscheduleUpdate();
     this->scheduleUpdate();
    
}

void MainMenuLayer::onClickHighScore(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    
    LetItRide::LetItRideSetting *gc = LetItRide::LetItRideSetting::shared();
    
    if (gc->isEnabled){
        gc->submitScore( g_nYourCoin );
    }
    if (gc->isEnabled) {
        gc->showLeaderboard();
    }
    else{
        if( gc->isAvailable() ){
            gc->playerLogin();
        }
    }
}

void MainMenuLayer::onClickMusic(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    bool bMusic = GameSetting::shared()->isEnableMusic();
    bMusic = !bMusic;
    GameSetting::shared()->saveMusic(bMusic);
    if ( bMusic )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic( "main_music.caf" , true);
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
}

void MainMenuLayer::onClickSound(cocos2d::CCObject *sender)
{
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    bool bSound = GameSetting::shared()->isEnableSound();
    bSound = !bSound;
    GameSetting::shared()->saveSound(bSound);
}


