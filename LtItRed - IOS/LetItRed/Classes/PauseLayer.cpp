//
//  PauseLayer.cpp
//  LetItRide
//
//  Created by lion on 2/17/14.
//
//

#include "PauseLayer.h"


#include "SimpleAudioEngine.h"

#include "Tools.h"
#include "Extern.h"

#include "GameSetting.h"
#include "PlayLayer.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* PauseLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    PauseLayer *layer = PauseLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

PauseLayer::~PauseLayer()
{
    stopAllActions();
    removeAllChildren();
}

// on "init" you need to initialize your instance
bool PauseLayer::init()
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
    
    return true;
}

void PauseLayer::initVariable()
{
    winSize = CCDirector::sharedDirector()->getWinSize();
}

void PauseLayer::initImages()
{
    CCString* strBk = Tools::imageName("pause_bk_mask");
    if ( g_fIsIPhoneFive )
    {
        strBk = CCString::create("pause_bk_mask-1136.png");
    }
    CCSprite* spBk = CCSprite::create(strBk->getCString());
    spBk->setPosition(ccp( winSize.width/2, winSize.height/2 ));
    this->addChild(spBk, 0);
}

void PauseLayer::initItems()
{
    itemResume = CCMenuItemImage::create(Tools::imageName("pause_Resume")->getCString(), Tools::imageName("pause_Resume_1")->getCString(), this, menu_selector(PauseLayer::onClickResume));
    itemResume->setPosition(ccp( winSize.width/2, winSize.height/2 + 70* g_rY));
    
    
    itemMainMenu = CCMenuItemImage::create(Tools::imageName("pause_Main_Menu")->getCString(), Tools::imageName("pause_Main_Menu_1")->getCString(), this, menu_selector(PauseLayer::onClickMainMenu));
    itemMainMenu->setPosition(ccp(winSize.width/2, winSize.height/2 - 70*g_rY));
    
    
    
    CCMenu* menu = CCMenu::create(itemResume, itemMainMenu, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);
}

void PauseLayer::initLabels()
{
    
}

#pragma mark click functions
void PauseLayer::onClickResume(cocos2d::CCObject *sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    g_bPause = false;
    this->setVisible(false);
}

void PauseLayer::onClickMainMenu(cocos2d::CCObject *sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    g_bMainMenu = true;
    g_bPause = false;
    this->setVisible(false);
}

