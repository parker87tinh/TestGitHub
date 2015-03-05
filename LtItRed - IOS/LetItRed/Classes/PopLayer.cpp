//
//  PopLayer.cpp
//  LetItRide
//
//  Created by lion on 2/18/14.
//
//

#include "PopLayer.h"

#include "SimpleAudioEngine.h"

#include "Tools.h"
#include "Extern.h"

#include "PlayLayer.h"

#include "GameSetting.h"
#include "LetItRideSetting.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* PopLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    PopLayer *layer = PopLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

PopLayer::~PopLayer()
{
    this->removeAllChildren();
    
}

// on "init" you need to initialize your instance
bool PopLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    CCString* strBk = Tools::imageName("pop_bk_mask");
    if ( g_fIsIPhoneFive )
    {
        strBk = CCString::create("pop_bk_mask-1136.png");
    }
    CCSprite* spPopMask = CCSprite::create(strBk->getCString());
    spPopMask->setPosition(ccp( winSize.width/2, winSize.height/2 ));
    this->addChild(spPopMask, 100);
    
    spPop = CCSprite::create(Tools::imageName("pop_box")->getCString());
    spPop->setPosition(ccp( winSize.width/2, winSize.height/2 ));
    this->addChild(spPop, 100);
    
    CCSprite* spT = CCSprite::create(Tools::imageName("pop_text")->getCString());
    spT->setPosition(ccp( winSize.width/2, winSize.height/2 - 10*g_rY ));
    this->addChild(spT, 103);
    
    
    
    itemPop1 = CCMenuItemImage::create(Tools::imageName("pop_slection_bar")->getCString(), Tools::imageName("pop_slection_bar")->getCString(), this, menu_selector(PopLayer::onClickPop1));
    itemPop1->setPosition(ccp( winSize.width/2, winSize.height/2 + 80*g_rY ));
    if ( !isIpad )
    {
        itemPop1->setPosition(ccp( winSize.width/2, winSize.height/2 + 80*g_rY ));
        if ( g_fIsIPhoneFive )
        {
            itemPop1->setPosition(ccp( winSize.width/2, winSize.height/2 + 90*g_rY ));
        }
    }
    
    itemPop2 = CCMenuItemImage::create(Tools::imageName("pop_slection_bar")->getCString(), Tools::imageName("pop_slection_bar")->getCString(), this, menu_selector(PopLayer::onClickPop2));
    itemPop2->setPosition(ccp( itemPop1->getPosition().x, itemPop1->getPosition().y - itemPop2->getContentSize().height - 5*g_rY));
    
    itemPop3 = CCMenuItemImage::create(Tools::imageName("pop_slection_bar")->getCString(), Tools::imageName("pop_slection_bar")->getCString(), this, menu_selector(PopLayer::onClickPop3));
    itemPop3->setPosition(ccp( itemPop1->getPosition().x, itemPop2->getPosition().y - itemPop3->getContentSize().height - 5*g_rY ));
    
    itemPop4 = CCMenuItemImage::create(Tools::imageName("pop_slection_bar")->getCString(), Tools::imageName("pop_slection_bar")->getCString(), this, menu_selector(PopLayer::onClickPop4));
    itemPop4->setPosition(ccp( itemPop1->getPosition().x, itemPop3->getPosition().y - itemPop4->getContentSize().height - 5*g_rY ));
    
    itemPop5 = CCMenuItemImage::create(Tools::imageName("pop_slection_bar")->getCString(), Tools::imageName("pop_slection_bar")->getCString(), this, menu_selector(PopLayer::onClickPop5));
    itemPop5->setPosition(ccp( itemPop1->getPosition().x, itemPop4->getPosition().y - itemPop5->getContentSize().height - 5*g_rY ));
    
    
    
    menuPop = CCMenu::create(itemPop1, itemPop2, itemPop3, itemPop4, itemPop5, NULL);
    menuPop->setPosition(CCPointZero);
    this->addChild(menuPop, 99);
    
    itemPopBuy = CCMenuItemImage::create(Tools::imageName("pop_Buy")->getCString(), Tools::imageName("pop_Buy_1")->getCString(), this, menu_selector(PopLayer::onClickPopBuy));
    itemPopBuy->setPosition(ccp( winSize.width/2, winSize.height/2 - 170*g_rY));
    if ( g_fIsIPhoneFive )
    {
        itemPopBuy->setPosition(ccp( winSize.width/2, winSize.height/2 - 190*g_rY));
    }
    
    itemPopClose = CCMenuItemImage::create(Tools::imageName("pop_X")->getCString(), Tools::imageName("pop_X_1")->getCString(), this, menu_selector(PopLayer::onClickPopClose));
    itemPopClose->setPosition(ccp( winSize.width/2 + spPop->getContentSize().width/2 - 30*g_rX, winSize.height/2 + 140*g_rY));
    if (g_fIsIPhoneFive)
    {
        itemPopClose->setPosition(ccp( winSize.width/2 + spPop->getContentSize().width/2 - 15*g_rX, winSize.height/2 + 140*g_rY));
    }
    CCMenu* menuPop1 = CCMenu::create(itemPopBuy, itemPopClose, NULL);
    menuPop1->setPosition(CCPointZero);
    this->addChild(menuPop1, 103);
    
    spPop1 = CCSprite::create(Tools::imageName("pop_slection_bar")->getCString());
    spPop1->setPosition( itemPop1->getPosition() );
    this->addChild(spPop1, 102);
    spPop1->setVisible(true);
    
    spPop2 = CCSprite::create(Tools::imageName("pop_slection_bar")->getCString());
    spPop2->setPosition( itemPop2->getPosition() );
    this->addChild(spPop2, 102);
    spPop2->setVisible(false);
    
    spPop3 = CCSprite::create(Tools::imageName("pop_slection_bar")->getCString());
    spPop3->setPosition( itemPop3->getPosition() );
    this->addChild(spPop3, 102);
    spPop3->setVisible(false);
    
    spPop4 = CCSprite::create(Tools::imageName("pop_slection_bar")->getCString());
    spPop4->setPosition( itemPop4->getPosition() );
    this->addChild(spPop4, 102);
    spPop4->setVisible(false);
    
    spPop5 = CCSprite::create(Tools::imageName("pop_slection_bar")->getCString());
    spPop5->setPosition( itemPop5->getPosition() );
    this->addChild(spPop5, 102);
    spPop5->setVisible(false);
    
    return true;
}

void PopLayer::onClickPop1(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    spPop1->setVisible(true);
    spPop2->setVisible(false);
    spPop3->setVisible(false);
    spPop4->setVisible(false);
    spPop5->setVisible(false);
}

void PopLayer::onClickPop2(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    spPop1->setVisible(false);
    spPop2->setVisible(true);
    spPop3->setVisible(false);
    spPop4->setVisible(false);
    spPop5->setVisible(false);
}

void PopLayer::onClickPop3(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    spPop1->setVisible(false);
    spPop2->setVisible(false);
    spPop3->setVisible(true);
    spPop4->setVisible(false);
    spPop5->setVisible(false);
}

void PopLayer::onClickPop4(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    spPop1->setVisible(false);
    spPop2->setVisible(false);
    spPop3->setVisible(false);
    spPop4->setVisible(true);
    spPop5->setVisible(false);
}

void PopLayer::onClickPop5(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    spPop1->setVisible(false);
    spPop2->setVisible(false);
    spPop3->setVisible(false);
    spPop4->setVisible(false);
    spPop5->setVisible(true);
}

void PopLayer::onClickPopBuy(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    if ( spPop1->isVisible() )
    {
        LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
        bj->buyCoins1000();
//        if ( bj->isPaidVersion() )
//        {
//            bj->processIAP("com.goldcoin.LetItRedPaid.1000Coins");
//        }
//        else
//        {
//            bj->processIAP("com.goldcoin.LetItRed.1000Coins");
//        }
        
    }
    
    if ( spPop2->isVisible() )
    {
        LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
        bj->buyCoins2500();
//        if ( bj->isPaidVersion() )
//        {
//            bj->processIAP("com.goldcoin.LetItRedPaid.2500Coins");
//        }
//        else
//        {
//            bj->processIAP("com.goldcoin.LetItRed.2500Coins");
//        }
        
    }
    
    if ( spPop3->isVisible() )
    {
        LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
        bj->buyCoins6000();
//        if ( bj->isPaidVersion() )
//        {
//            bj->processIAP("com.goldcoin.LetItRedPaid.6000Coins");
//        }
//        else
//        {
//            bj->processIAP("com.goldcoin.LetItRed.6000Coins");
//        }
        
    }
    
    if ( spPop4->isVisible() )
    {
        LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
        bj->buyCoins14000();
        
//        if ( bj->isPaidVersion() )
//        {
//            bj->processIAP("com.goldcoin.LetItRedPaid.14000Coins");
//        }
//        else
//        {
//            bj->processIAP("com.goldcoin.LetItRed.14000Coins");
//        }
        
    }
    
    if ( spPop5->isVisible() )
    {
        LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
        bj->buyCoins30000();
//        if ( bj->isPaidVersion() )
//        {
//            bj->processIAP("com.goldcoin.LetItRedPaid.30000Coins");
//        }
//        else
//        {
//            bj->processIAP("com.goldcoin.LetItRed.30000Coins");
//        }
        
    }
}

void PopLayer::onClickPopClose(CCObject* sender)
{
    if ( !this->isVisible() )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    g_bBuy = false;
    this->setVisible(false);
//    CCDirector::sharedDirector()->popScene();
}