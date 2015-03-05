//
//  PlayLayer.cpp
//  LetItRide
//
//  Created by lion on 1/28/14.
//
//

#include "PlayLayer.h"
#include "SimpleAudioEngine.h"

#include "Tools.h"
#include "Extern.h"

#include "LetItRideSetting.h"
#include "MainMenuLayer.h"

#include "PopLayer.h"
#include "GameSetting.h"


CCPoint posCoinMark[] = { ccp( 300*g_rX, 300*g_rY), ccp( 500*g_rX, 300*g_rY), ccp( 700*g_rX, 300*g_rY )};

CCScene* PlayLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    PlayLayer *layer = PlayLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

PlayLayer::~PlayLayer()
{
    stopAllActions();
    
    removeAllChildren();
    if ( pPause != NULL )
    {
        this->removeChild(pPause, true);
        pPause = NULL;
    }
    if ( pPop != NULL )
    {
        this->removeChild(pPop, true);
        pPop = NULL;
    }
}

// on "init" you need to initialize your instance
bool PlayLayer::init()
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
    
    this->scheduleUpdate();
    
    bool bMusic = GameSetting::shared()->isEnableMusic();
    GameSetting::shared()->saveMusic(bMusic);
    if ( bMusic )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic( "play_music.mp3" , true);
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
    
    return true;
}

void PlayLayer::initVariable()
{
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    GameSetting::shared()->getCoins();
    mType = game_bet;
//    g_nYourCoin = 2000;
    nCurBet = 0;
    nIncreaseBet = 0;
    bDeal = false;
    nCardIndex = -1;
    for ( int i = 0; i < 52; i++ )
    {
        bCard[i] = false;
    }
    for ( int i = 0; i < 5; i++ )
    {
        nCardNum[i] = 0;
        nCardMark[i] = 0;
    }
    nPlayNum = 0;
    nDropTouchNum = 0;
    nTime = 1;
    
    nTempBet = 0;
    
    for ( int i = 0; i < 13; i++ )
    {
        m_nCNum[i] = 0;
    }
    card_results = card_non;
    
    g_bPause = false;
    g_bBuy = false;
    g_bMainMenu = false;
    
    pPause = PauseLayer::create();
    this->addChild(pPause, 30);
    pPause->setVisible(false);
    
    pPop = PopLayer::create();
    this->addChild(pPop, 30);
    pPop->setVisible(false);
}

void PlayLayer::initImages()
{
    CCString* strBk = Tools::imageName("play_bg");
    if ( g_fIsIPhoneFive )
    {
        strBk = CCString::create("play_bg-1136.png");
    }
    CCSprite* spBk = CCSprite::create(strBk->getCString());
    spBk->setPosition(ccp( winSize.width/2, winSize.height/2 ));
    this->addChild(spBk, 0);
    
    spLeftCoinMark = CCSprite::create(Tools::imageName("play_s_d")->getCString());
    spLeftCoinMark->setPosition( ccp( 93*g_rX, winSize.height - 231*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spLeftCoinMark->setPosition( ccp( 180*g_rX, winSize.height - 231*g_rY ) );
    }
    this->addChild(spLeftCoinMark, 0);
    
    spCenterCoinMark = CCSprite::create(Tools::imageName("play_2_d")->getCString());
    spCenterCoinMark->setPosition( ccp( 193*g_rX, winSize.height - 231*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spCenterCoinMark->setPosition( ccp( 260*g_rX, winSize.height - 231*g_rY ) );
    }
    this->addChild(spCenterCoinMark, 0);
    
    spRightCoinMark = CCSprite::create(Tools::imageName("play_3_d")->getCString());
    spRightCoinMark->setPosition( ccp( 293*g_rX, winSize.height - 231*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spRightCoinMark->setPosition( ccp( 340*g_rX, winSize.height - 231*g_rY ) );
    }
    this->addChild(spRightCoinMark, 0);
    
    CCSprite* spBalance = CCSprite::create(Tools::imageName("play_BALANCE")->getCString());
    spBalance->setPosition( ccp( 88*g_rX, winSize.height - 29*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spBalance->setPosition( ccp( 164*g_rX, winSize.height - 29*g_rY ) );
    }
    this->addChild(spBalance, 0);
    
    CCSprite* spBet = CCSprite::create(Tools::imageName("play_CURRENT_BET")->getCString());
    spBet->setPosition( ccp( 88*g_rX, winSize.height - 80*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spBet->setPosition( ccp( 164*g_rX, winSize.height - 80*g_rY ) );
    }
    this->addChild(spBet, 0);
    
    CCSprite* spMin = CCSprite::create(Tools::imageName("play_5_MINIMUM")->getCString());
    spMin->setPosition( ccp( 894*g_rX, winSize.height - 26*g_rY ) );
    spMin->setAnchorPoint(ccp(1.0f, 0.5f ));
    this->addChild(spMin, 0);
    
    CCSprite* spMax = CCSprite::create(Tools::imageName("play_100_MAXIMUM")->getCString());
    spMax->setPosition( ccp( 894*g_rX, winSize.height - 50*g_rY ) );
    spMax->setAnchorPoint(ccp(1.0f, 0.5f ));
    this->addChild(spMax, 0);
}

void PlayLayer::initItems()
{
    item5Coin = CCMenuItemImage::create(Tools::imageName(CCString::create("play_5")->getCString())->getCString(), Tools::imageName(CCString::create("play_5_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClick5Coin));
    item5Coin->setPosition(ccp(476*g_rX, winSize.height - 710*g_rY));
    if ( g_fIsIPhoneFive )
    {
        item5Coin->setPosition(ccp(559*g_rX, winSize.height - 700*g_rY));
    }
    
    item10Coin = CCMenuItemImage::create(Tools::imageName(CCString::create("play_10")->getCString())->getCString(), Tools::imageName(CCString::create("play_10_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClick10Coin));
    item10Coin->setPosition(ccp(563*g_rX, winSize.height - 712*g_rY));
    if ( g_fIsIPhoneFive )
    {
        item10Coin->setPosition(ccp(633*g_rX, winSize.height - 698*g_rY));
    }
    
    item25Coin = CCMenuItemImage::create(Tools::imageName(CCString::create("play_25")->getCString())->getCString(), Tools::imageName(CCString::create("play_25_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClick25Coin));
    item25Coin->setPosition(ccp( 650*g_rX, winSize.height - 705*g_rY));
    if ( g_fIsIPhoneFive )
    {
        item25Coin->setPosition(ccp( 703*g_rX, winSize.height - 690*g_rY));
    }
    
    item50Coin = CCMenuItemImage::create(Tools::imageName(CCString::create("play_50")->getCString())->getCString(), Tools::imageName(CCString::create("play_50_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClick50Coin));
    item50Coin->setPosition(ccp( 730*g_rX, winSize.height - 694*g_rY));
    if ( g_fIsIPhoneFive )
    {
        item50Coin->setPosition(ccp( 770*g_rX, winSize.height - 674*g_rY));
    }
    
    item100Coin = CCMenuItemImage::create(Tools::imageName(CCString::create("play_100")->getCString())->getCString(), Tools::imageName(CCString::create("play_100_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClick100Coin));
    item100Coin->setPosition(ccp( 818*g_rX, winSize.height - 679*g_rY));
    if ( g_fIsIPhoneFive )
    {
        item100Coin->setPosition(ccp( 830*g_rX, winSize.height - 644*g_rY));
    }
    
    itemX = CCMenuItemImage::create(Tools::imageName(CCString::create("play_X")->getCString())->getCString(), Tools::imageName(CCString::create("play_X_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClickX));
    itemX->setPosition(ccp( 898*g_rX, winSize.height - 649*g_rY));
    if ( g_fIsIPhoneFive )
    {
        itemX->setPosition(ccp( 898*g_rX, winSize.height - 600*g_rY));
    }
    
    CCSprite* spDeal = CCSprite::create(Tools::imageName(CCString::create("play_Deal_1")->getCString())->getCString());
    spDeal->setPosition( ccp( 70*g_rX, winSize.height - 625*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spDeal->setPosition( ccp( 170*g_rX, winSize.height - 625*g_rY ) );
    }
    this->addChild(spDeal, 0);
    
    CCSprite* spKeep = CCSprite::create(Tools::imageName(CCString::create("play_Keep_1")->getCString())->getCString());
    spKeep->setPosition( ccp( 210*g_rX, winSize.height - 670*g_rY ) );
    if ( g_fIsIPhoneFive )
    {
        spKeep->setPosition( ccp( 280*g_rX, winSize.height - 670*g_rY ) );
    }
    this->addChild(spKeep, 0);
    
    CCSprite* spDrop = CCSprite::create(Tools::imageName(CCString::create("play_Drop_1")->getCString())->getCString());
    spDrop->setPosition( ccp( 338*g_rX, winSize.height - 700*g_rY) );
    if ( g_fIsIPhoneFive )
    {
        spDrop->setPosition( ccp( 390*g_rX, winSize.height - 690*g_rY) );
    }
    this->addChild(spDrop, 0);
    
    
    itemDeal = CCMenuItemImage::create(Tools::imageName(CCString::create("play_Deal")->getCString())->getCString(), Tools::imageName(CCString::create("play_Deal_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClickDeal));
    itemDeal->setPosition(spDeal->getPosition());
    
    itemKeep = CCMenuItemImage::create(Tools::imageName(CCString::create("play_Keep")->getCString())->getCString(), Tools::imageName(CCString::create("play_Keep_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClickKeep));
    itemKeep->setPosition(spKeep->getPosition());
    
    itemDrop = CCMenuItemImage::create(Tools::imageName(CCString::create("play_Drop")->getCString())->getCString(), Tools::imageName(CCString::create("play_Drop_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClickDrop));
    itemDrop->setPosition(spDrop->getPosition());
    
    CCMenuItemImage* itemPause = CCMenuItemImage::create(Tools::imageName(CCString::create("play_pause")->getCString())->getCString(), Tools::imageName(CCString::create("play_pause_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClickPause));
    itemPause->setPosition(ccp( 970*g_rX, winSize.height - 50*g_rY));
    
    CCMenuItemImage* itemBuy = CCMenuItemImage::create(Tools::imageName(CCString::create("play_buy")->getCString())->getCString(), Tools::imageName(CCString::create("play_buy_1")->getCString())->getCString(), this, menu_selector(PlayLayer::onClickBuy));
    itemBuy->setPosition(ccp( 285*g_rX, winSize.height - 47*g_rY));
    if(g_fIsIPhoneFive)
        itemBuy->setPosition(ccp( 305*g_rX, winSize.height - 47*g_rY));
    menu = CCMenu::create(item5Coin, item10Coin, item25Coin, item50Coin, item100Coin, itemX, itemDeal, itemDrop, itemKeep, itemPause, itemBuy, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 1);
    
    itemDeal->setVisible(false);
    itemKeep->setVisible(false);
    itemDrop->setVisible(false);
}

void PlayLayer::initLabels()
{
    float rW = 40;
    float rH = 0;
    if ( isIpad )
    {
        rW = 160;
        rH = 21;
//        if ( m_fHighRes )
//        {
//            rW = 330;
//            rH = 43;
//        }
        
    }
    else
    {
        rW = 70;
        rH = 9;
//        if ( m_fHighRes )
//        {
//            rW = 150;
//            rH = 20;
//        }
    }
    labRemainBet = CCLabelAtlas::create(CCString::createWithFormat("%d", g_nYourCoin)->getCString(), Tools::imageName("play_number")->getCString(), rW/10, rH, '0');
    //CCLabelTTF::create(CCString::createWithFormat("%d", g_nYourCoin)->getCString(), "Arial", rFont);
    labRemainBet->setPosition(ccp( 95*g_rX - labRemainBet->getContentSize().width/2, winSize.height - 54*g_rY ));
    this->addChild(labRemainBet, 3);
    
    labCurBet = CCLabelAtlas::create(CCString::createWithFormat("%d", nCurBet)->getCString(), Tools::imageName("play_number")->getCString(), rW/10, rH, '0');
    //CCLabelTTF::create(CCString::createWithFormat("%d", nCurBet)->getCString(), "Arial", rFont);
    labCurBet->setPosition(ccp( 95*g_rX - labRemainBet->getContentSize().width/2, winSize.height - 110*g_rY ));
    this->addChild(labCurBet, 3);
    
//    ccColor3B color = ccc3(80, 61, 16);
//    labRemainBet->setColor(color);
//    labCurBet->setColor(color);
    
}

#pragma mark click functions

void PlayLayer::onClickPause(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    g_bPause = true;
    pPause->setVisible(true);
}

void PlayLayer::onClickBuy(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( GameSetting::shared()->isEnableSound() )
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
    }
    g_bBuy = true;
    pPop->setVisible(true);
//    CCDirector::sharedDirector()->pushScene(PopLayer::scene());
}

void PlayLayer::onClick5Coin(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("collectcoin.caf", false);
        }
        nCurBet += 15;
        g_nYourCoin -= 15;
        if ( g_nYourCoin < 0 )
        {
            g_nYourCoin += 15;
            nCurBet -= 15;
            LetItRide::LetItRideSetting::shared()->alertDialog();
            return;
        }
        if ( nCurBet > MAX_BET )
        {
            g_nYourCoin += 15;
            nCurBet -= 15;
            
            return;
        }
        this->drawCoinMark("play_5_d");
        this->drawBetLabel();
    }
}

void PlayLayer::onClick10Coin(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("collectcoin.caf", false);
        }
        nCurBet += 30;
        g_nYourCoin -= 30;
        if ( nCurBet > MAX_BET )
        {
            g_nYourCoin += 30;
            nCurBet -= 30;
            return;
        }
        if ( g_nYourCoin < 0 )
        {
            g_nYourCoin += 30;
            nCurBet -= 30;
            LetItRide::LetItRideSetting::shared()->alertDialog();
            return;
        }
        this->drawCoinMark("play_10_d");
        this->drawBetLabel();
    }
}

void PlayLayer::onClick25Coin(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("collectcoin.caf", false);
        }
        nCurBet += 75;
        g_nYourCoin -= 75;
        if ( nCurBet > MAX_BET )
        {
            g_nYourCoin += 75;
            nCurBet -= 75;
            return;
        }
        if ( g_nYourCoin < 0 )
        {
            g_nYourCoin += 75;
            nCurBet -= 75;
            LetItRide::LetItRideSetting::shared()->alertDialog();
            return;
        }
        this->drawCoinMark("play_25_d");
        this->drawBetLabel();
    }
}

void PlayLayer::onClick50Coin(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("collectcoin.caf", false);
        }
        nCurBet += 150;
        g_nYourCoin -= 150;
        if ( nCurBet > MAX_BET )
        {
            g_nYourCoin += 150;
            nCurBet -= 150;
            return;
        }
        if ( g_nYourCoin < 0 )
        {
            g_nYourCoin += 150;
            nCurBet -= 150;
            LetItRide::LetItRideSetting::shared()->alertDialog();
            return;
        }
        this->drawCoinMark("play_50_d");
        this->drawBetLabel();
    }
}

void PlayLayer::onClick100Coin(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("collectcoin.caf", false);
        }
        nCurBet += 300;
        g_nYourCoin -= 300;
        if ( nCurBet > MAX_BET )
        {
            g_nYourCoin += 300;
            nCurBet -= 300;
            return;
        }
        if ( g_nYourCoin < 0 )
        {
            g_nYourCoin += 300;
            nCurBet -= 300;
            LetItRide::LetItRideSetting::shared()->alertDialog();
            return;
        }
        this->drawCoinMark("play_100_d");
        this->drawBetLabel();
    }
}

void PlayLayer::onClickX(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sound4.wav", false);
        }
        g_nYourCoin += nCurBet;
        nCurBet = 0;
        this->drawBetLabel();
        this->unVisibleCoinMark();
    }
}

void PlayLayer::onClickDeal(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        if ( bDeal )
        {
            return;
        }
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("suffle.wav", false);
        }
        bDeal = true;
        nPlayNum = 0;
        for ( int i = 0; i < 52; i++ )
        {
            bCard[i] = false;
        }
        
        this->drawBetLabel();
        mType = game_deal;
    }
    
}

void PlayLayer::onClickKeep(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_play )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
        }
        nPlayNum += 1;
        this->createCard();
        if ( nPlayNum == 2 )
        {
            mType = game_end;
            nPlayNum = 0;
        }
    }
}

void PlayLayer::onClickDrop(cocos2d::CCObject *sender)
{
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_play )
    {
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("click.caf", false);
        }
        nPlayNum += 1;
        nDropTouchNum += 1;
        int nTmpCurBet = nCurBet/3;
        if ( nDropTouchNum == 2 )
        {
            nTmpCurBet = nCurBet/2;
        }
        nCurBet -= nTmpCurBet;
        g_nYourCoin += nTmpCurBet;
        this->dropCoinMark(3-nDropTouchNum);
        this->drawBetLabel();
        this->createCard();
        if ( nPlayNum == 2 )
        {
            mType = game_end;
            nPlayNum = 0;
        }
    }
}

#pragma mark touch functions
void PlayLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
//    CCTouch* touch = (CCTouch*)pTouches->anyObject();
//    CCPoint location = touch->getLocationInView();
//    CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
    if ( g_bPause || g_bBuy )
    {
        return;
    }
    if ( mType == game_bet )
    {
        LetItRide::LetItRideSetting::shared()->alertBetDialog();
    }
    if ( mType == game_end )
    {
        for ( int i = 0; i < 3; i++ )
        {
            if ( spCoinMark[i] != NULL )
            {
                this->removeChild(spCoinMark[i], true);
                spCoinMark[i] = NULL;
            }
        }
        for ( int i = 0; i < 5; i++ )
        {
            if ( spCard[i] != NULL )
            {
                this->removeChild(spCard[i], true);
                spCard[i] = NULL;
            }
        }
        nCurBet = 0;
        for ( int i = 0; i < 52; i++ )
        {
            bCard[i] = false;
        }
        this->drawBetLabel();
        nPlayNum = 0;
        nDropTouchNum = 0;
        mType = game_bet;
        
        if ( spBoard != NULL )
        {
            this->removeChild(spBoard, true);
            spBoard = NULL;
        }
        if ( spWinLoss != NULL )
        {
            this->removeChild(spWinLoss, true);
            spWinLoss = NULL;
        }
        
        if ( labGetBet != NULL )
        {
            this->removeChild(labGetBet, true);
            labGetBet = NULL;
        }
        
        if ( labKKK != NULL )
        {
            this->removeChild(labKKK, true);
            labKKK = NULL;
        }
        if ( labNNN != NULL )
        {
            this->removeChild(labNNN, true);
            labNNN = NULL;
        }
    }
}

#pragma mark redraw functions

void PlayLayer::update(float dt)
{
    if ( g_bPause || g_bBuy )
    {
        this->drawBetLabel();
        return;
    }
    GameSetting::shared()->saveCoins();
    if ( g_bMainMenu )
    {
        CCDirector::sharedDirector()->replaceScene(MainMenuLayer::scene());
        g_bMainMenu = false;
        return;
    }
    nRandom = abs(arc4random());
    if ( g_nYourCoin < 0 )
    {
        g_nYourCoin = 0;
    }
    nTime++;
    if ( nTime > 1000 )
    {
        nTime = 1;
    }
    if ( nTime % 4 == 0 )
    {
        
    }
    if ( mType == game_bet )
    {
        itemDeal->setVisible(false);
        itemDrop->setVisible(false);
        itemKeep->setVisible(false);
        if ( spCoinMark[0] != NULL && spCoinMark[1] != NULL && spCoinMark[2] != NULL )
        {
            if ( spCoinMark[0]->isVisible() || spCoinMark[1]->isVisible() || spCoinMark[2]->isVisible() )
            {
                itemDeal->setVisible(true);
            }
        }
        nPlayNum = 0;
        
    }
    
    if ( mType == game_deal )
    {
        itemDeal->setVisible(false);
        itemDrop->setVisible(true);
        itemKeep->setVisible(true);
        if ( bDeal )
        {
            nCardIndex += 1;
            
            if ( nCardIndex >= 5 )
            {
                bDeal = false;
//                this->createCard();
                nCardIndex =-1;
                mType = game_play;
                return;
            }
            this->getCardNumMark();
            this->createCard();
            
        }
    }//mType == game_deal
    
    if ( mType == game_play )
    {
        
        
    }
    
    if ( mType == game_end )
    {
        itemDeal->setVisible(false);
        itemDrop->setVisible(false);
        itemKeep->setVisible(false);
        nPlayNum = 0;
        for ( int i = 0; i < 52; i++ )
        {
            bCard[i] = false;
        }
    }
    
    labRemainBet->setPosition(ccp( 90*g_rX - labRemainBet->getContentSize().width/2, winSize.height - 54*g_rY  - labRemainBet->getContentSize().height/2));
    labCurBet->setPosition(ccp( 90*g_rX - labCurBet->getContentSize().width/2, winSize.height - 110*g_rY  - labRemainBet->getContentSize().height/2));
    if ( g_fIsIPhoneFive )
    {
        labRemainBet->setPosition(ccp( 168*g_rX - labRemainBet->getContentSize().width/2, winSize.height - 54*g_rY  - labRemainBet->getContentSize().height/2));
        labCurBet->setPosition(ccp( 168*g_rX - labCurBet->getContentSize().width/2, winSize.height - 110*g_rY  - labRemainBet->getContentSize().height/2));
    }
}


#pragma mark other
void PlayLayer::drawCoinMark(const char *chName)
{
    for ( int i = 0; i < 3; i++ )
    {
        if ( spCoinMark[i] != NULL )
        {
            this->removeChild(spCoinMark[i], true);
            spCoinMark[i] = NULL;
        }
        spCoinMark[i] = CCSprite::create(Tools::imageName(chName)->getCString());
        if ( i == 0 )
        {
            spCoinMark[i]->setPosition(spLeftCoinMark->getPosition());
        }
        if ( i == 1 )
        {
            spCoinMark[i]->setPosition(spCenterCoinMark->getPosition());
        }
        if ( i == 2 )
        {
            spCoinMark[i]->setPosition(spRightCoinMark->getPosition());
        }
        this->addChild(spCoinMark[i], 2);
        
    }
}

void PlayLayer::unVisibleCoinMark()
{
    for ( int i = 0; i < 3; i++ )
    {
        if ( spCoinMark[i] != NULL )
        {
            spCoinMark[i]->setVisible(false);
        }
    }
}

void PlayLayer::dropCoinMark(int nNum)
{
    for ( int i = 0; i < 3; i++ )
    {
        if ( i >= nNum )
        {
            if ( spCoinMark[i] != NULL )
            {
                spCoinMark[i]->setVisible(false);
            }
        }
        
    }
}

void PlayLayer::drawBetLabel()
{
    if ( labRemainBet != NULL )
    {
        labRemainBet->setString(CCString::createWithFormat("%d", g_nYourCoin)->getCString());
    }
    if ( labCurBet != NULL )
    {
        labCurBet->setString(CCString::createWithFormat("%d", nCurBet)->getCString());
    }
    
}

void PlayLayer::createCard()
{
    if ( nPlayNum == 0 )
    {
        CCString* strCardFile = NULL;
//        for ( int i = 0; i < 5; i++ )
//        {
            if ( nCardIndex >= 3 && nCardIndex <= 4 )
            {
                strCardFile = CCString::create("poker_bg");
            }
            else
            {
                strCardFile = CCString::createWithFormat("%d-%d", nCardNum[nCardIndex], nCardMark[nCardIndex] );
            }
            if ( spCard[nCardIndex] != NULL )
            {
                this->removeChild(spCard[nCardIndex], true);
                spCard[nCardIndex] = NULL;
            }
            spCard[nCardIndex] = CCSprite::create(Tools::imageName(strCardFile->getCString())->getCString());
            spCard[nCardIndex]->setPosition(this->getCardPosition(nCardIndex));
            this->addChild(spCard[nCardIndex], 3);
//        }
        
    }
    
    if ( nPlayNum == 1 )
    {
        if ( spCard[3] != NULL )
        {
            this->removeChild(spCard[3], true);
            spCard[3] = NULL;
        }
        CCString* strCardFile = NULL;
        strCardFile = CCString::createWithFormat("%d-%d", nCardNum[3], nCardMark[3] );
        spCard[3] = CCSprite::create(Tools::imageName(strCardFile->getCString())->getCString());
        spCard[3]->setPosition(this->getCardPosition(3));
        this->addChild(spCard[3], 3);
        spCard[3]->setScaleX(0.1f);
        
        CCScaleTo* act1 = CCScaleTo::create(0.1f, 1.0f, 1.0f);
        CCSequence* act = CCSequence::create(act1, NULL);
        spCard[3]->runAction(act);
        
    }
    
    if ( nPlayNum == 2 )
    {
        if ( spCard[4] != NULL )
        {
            this->removeChild(spCard[4], true);
            spCard[4] = NULL;
        }
        CCString* strCardFile = NULL;
        strCardFile = CCString::createWithFormat("%d-%d", nCardNum[4], nCardMark[4] );
        spCard[4] = CCSprite::create(Tools::imageName(strCardFile->getCString())->getCString());
        spCard[4]->setPosition(this->getCardPosition(4));
        this->addChild(spCard[4], 3);
        spCard[4]->setScaleX(0.1f);
        
        CCScaleTo* act1 = CCScaleTo::create(0.1f, 1.0f, 1.0f);
        CCSequence* act = CCSequence::create(act1, NULL);
        spCard[4]->runAction(act);
        
        this->checkCard();
    }
}

CCPoint PlayLayer::getCardPosition(int nIndex)
{
    CCPoint pos = CCPointZero;
    switch (nIndex) {
        case 0:
            pos = ccp( winSize.width/2 - 30*g_rX, winSize.height/2 );
            break;
        case 1:
            pos = ccp( winSize.width/2 + 130*g_rX, winSize.height/2 );
            break;
        case 2:
            pos = ccp( winSize.width/2 + 290*g_rX, winSize.height/2 );
            break;
        case 3:
            pos = ccp( winSize.width/2 + 50*g_rX, winSize.height/2 + 220*g_rY );
            break;
        case 4:
            pos = ccp( winSize.width/2 + 210*g_rX, winSize.height/2 + 220*g_rY );
            break;
        default:
            break;
    }
    
    return pos;
}

#pragma mark make Card
void PlayLayer::getCardNumMark()
{
    int nB = (nRandom%52);
    if ( bCard[nB] == true )
    {
        while ( bCard[nB] == true )
        {
            nB++;
            if ( nB >= 52 )
            {
                nB = 0;
            }
            if ( bCard[nB] == false )
            {
                nCardNum[nCardIndex] = (nB % 13) + 1;
                nCardMark[nCardIndex] = getCardMark(nB);
                bCard[nB] = true;
                break;
            }
        }
        
    }
    
    if ( bCard[nB] == false )
    {
        nCardNum[nCardIndex] = (nB % 13) + 1;
        nCardMark[nCardIndex] = getCardMark(nB);
        bCard[nB] = true;
        
    }
    
}

int PlayLayer::getCardMark(int nI)
{
    int n = 0;
    if ( nI >= 0 && nI < 13 )
    {
        n = 1;
    }
    if ( nI >= 13 && nI < 26 )
    {
        n = 2;
    }
    if ( nI >= 26 && nI < 39 )
    {
        n = 3;
    }
    if ( nI >= 39 && nI < 52 )
    {
        n = 4;
    }
    return n;
}

#pragma mark search 
void PlayLayer::checkCard()
{
    //Royal Flush
    //Straight Flush
    //4 of a Kind
    //Full House
    //Flush
    //Straight
    //3 of a Kind
    //Two Pair
    //Pair of 10's
    card_results = card_non;
    n_OnePair = 0;
    n_TwoPair1 = 0;
    n_TwoPair2 = 0;
    n_3Kind = 0;
    n_Straight1 = 0;
    n_Straight2 = 0;
    n_Straight3 = 0;
    n_Straight4 = 0;
    n_Straight5 = 0;
    n_Flush1 = 0;
    n_Flush2 = 0;
    n_Flush3 = 0;
    n_Flush4 = 0;
    n_Flush5 = 0;
    n_FullHouse1 = 0;
    n_FullHouse2 = 0;
    n_4Kind = 0;
    n_StrightFlush1 = 0;
    n_StrightFlush2 = 0;
    n_StrightFlush3 = 0;
    n_StrightFlush4 = 0;
    n_StrightFlush5 = 0;
    n_RoyalFlush1 = 0;
    n_RoyalFlush2 = 0;
    n_RoyalFlush3 = 0;
    n_RoyalFlush4 = 0;
    n_RoyalFlush5 = 0;
    
    for ( int i = 0; i < 13; i++ )
    {
        m_nCNum[i] = 0;
    }
    
    for ( int i = 0; i < 5; i++ )
    {
        m_nCNum[nCardNum[i]-1] += 1;
        
    }
    
    int m_nCMark = nCardMark[0];
    for ( int i = 0; i < 5; i++ )//Is flush
    {
        if ( m_nCMark == nCardMark[i] )
        {
            card_results = card_flush;
            n_Flush1 = nCardNum[0];
            n_Flush2 = nCardNum[1];
            n_Flush3 = nCardNum[2];
            n_Flush4 = nCardNum[3];
            n_Flush5 = nCardNum[4];
        }
        else
        {
            card_results = card_non;
            break;
        }
    }
    
    for ( int i = 0; i < 13; i++ )//Is Straight or Straight flush
    {
        bool bBreak = false;
        if ( m_nCNum[i] == 1 )
        {
            if ( i + 4 < 13 )
            {
                int nStright = 0;
                for ( int j = i; j <= i+4; j++ )
                {
                    if ( m_nCNum[j] == 1 )
                    {
                        nStright += 1;
                        if ( nStright == 5 )
                        {
                            if ( card_results == card_flush )
                            {
                                card_results = card_straight_flush;
                                n_StrightFlush1 = i+1;
                                n_StrightFlush2 = i+2;
                                n_StrightFlush3 = i+3;
                                n_StrightFlush4 = i+4;
                                n_StrightFlush5 = i+5;
                            }
                            else
                            {
                                card_results = card_straight;
                                n_Straight1 = i+1;
                                n_Straight2 = i+2;
                                n_Straight3 = i+3;
                                n_Straight4 = i+4;
                                n_Straight5 = i+5;
                            }
                            
                            bBreak = true;
                            break;
                        }
                        
                    }
                }
            }
            
        }
        if ( bBreak )
        {
            break;
        }
    }
    
    if ( m_nCNum[0] == 1 && m_nCNum[9] == 1 && m_nCNum[10] == 1 && m_nCNum[11] == 1 && m_nCNum[12] == 1 )
    {
        card_results = card_royal_flush;
        n_RoyalFlush1 = 1;
        n_RoyalFlush2 = 10;
        n_RoyalFlush3 = 11;
        n_RoyalFlush4 = 12;
        n_RoyalFlush5 = 13;
    }
    
    int nTwoPair = 0;
    bool bOnePair = false;
    for ( int i = 0; i < 13; i++ )
    {
        if ( m_nCNum[i] == 2 )
        {
            nTwoPair += 1;
            if ( card_results == card_3_kind )
            {
                card_results = card_full_house;
                n_FullHouse1 = n_3Kind;
                n_FullHouse2 = i+1;
                break;
            }
            else
            {
                if ( nTwoPair == 2 )
                {
                    card_results = card_tow_pair;
                    n_TwoPair1 = i+1;
                    n_TwoPair2 = n_OnePair;
                    break;
                }
                
                if ( nTwoPair == 1 )
                {
                    n_OnePair = i+1;
                    if ( i >= 9 )
                    {
                        bOnePair = true;
                        card_results = card_one_pair;
                        n_OnePair = i+1;
                    }
                }
                
            }
            
            
        }
        if ( m_nCNum[i] == 3 )
        {
            
            if ( nTwoPair == 1 )
            {
                card_results = card_full_house;
                n_FullHouse1 = n_OnePair;
                n_FullHouse2 = i+1;
            }
            else
            {
                card_results = card_3_kind;
                n_3Kind = i+1;
            }
        }
        
        if ( m_nCNum[i] == 4 )
        {
            card_results = card_4_kind;
            n_4Kind = i+1;
        }
    }
    this->showResult();
    
}

void PlayLayer::showResult()
{
    if ( spBoard != NULL )
    {
        this->removeChild(spBoard, true);
        spBoard = NULL;
    }
    spBoard = CCSprite::create(Tools::imageName("play_tex_tbox")->getCString());
    spBoard->setScale(3.0f);
    spBoard->setPosition( ccp( winSize.width/2, winSize.height/2 ) );
    this->addChild(spBoard, 15);
    
    CCString* strWinLost = NULL;
    CCString* strKKK = NULL;
    CCString* strNNN = NULL;
    
    if ( card_results == card_non )
    {
        strWinLost = Tools::imageName("play_you_lost");
        strKKK = CCString::create("You have Nothing");
        strNNN = CCString::create("");
        nTempBet = 0;
        
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("fail.mp3", false);
        }
    }
    else
    {
        
        if ( GameSetting::shared()->isEnableSound() )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("coin_collect.wav", false);
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("win.m4a", false);
        }
        
        strWinLost = Tools::imageName("play_you_win");
        
        if ( card_results == card_one_pair )
        {
            nTempBet = nCurBet;
            strKKK = CCString::create("One Pair");
            strNNN = CCString::createWithFormat("%d", n_OnePair);
            if ( n_OnePair > 10 )
            {
                if ( n_OnePair == 11 )
                {
                    strNNN = CCString::create("J");
                }
                if ( n_OnePair == 12 )
                {
                    strNNN = CCString::create("Q");
                }
                if ( n_OnePair == 13 )
                {
                    strNNN = CCString::create("K");
                }
            }
        }
        if ( card_results == card_tow_pair )
        {
            nTempBet = nCurBet*2;
            strKKK = CCString::create("Two Pair");
            strNNN = CCString::createWithFormat("%d", n_TwoPair1);
            CCString* strNNN1 = CCString::createWithFormat("%d", n_TwoPair2);
            if ( n_TwoPair1 == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_TwoPair1 == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_TwoPair1 == 13 )
            {
                strNNN = CCString::create("K");
            }
            if ( n_TwoPair1 == 1 )
            {
                strNNN = CCString::create("A");
            }
            if ( n_TwoPair2 == 11 )
            {
                strNNN1 = CCString::create("J");
            }
            if ( n_TwoPair2 == 12 )
            {
                strNNN1 = CCString::create("Q");
            }
            if ( n_TwoPair2 == 13 )
            {
                strNNN1 = CCString::create("K");
            }
            if ( n_TwoPair2 == 1 )
            {
                strNNN1 = CCString::create("A");
            }
            strNNN = CCString::createWithFormat("%s, %s", strNNN->getCString(), strNNN1->getCString());
        }
        
        if ( card_results == card_3_kind )
        {
            nTempBet = nCurBet*3;
            strKKK = CCString::create("3 of a Kind");
            strNNN = CCString::createWithFormat("%d", n_3Kind);
            if ( n_3Kind == 1 )
            {
                strNNN = CCString::create("A");
            }
            if ( n_3Kind == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_3Kind == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_3Kind == 13 )
            {
                strNNN = CCString::create("K");
            }
            
        }
        if ( card_results == card_4_kind )
        {
            nTempBet = nCurBet*50;
            strKKK = CCString::create("4 of a Kind");
            strNNN = CCString::createWithFormat("%d", n_4Kind);
            if ( n_4Kind == 1 )
            {
                strNNN = CCString::create("A");
            }
            if ( n_4Kind == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_4Kind == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_4Kind == 13 )
            {
                strNNN = CCString::create("K");
            }
        }
        if ( card_results == card_full_house )
        {
            nTempBet = nCurBet*11;
            strKKK = CCString::create("Full House");
            strNNN = CCString::createWithFormat("%d", n_FullHouse1 );
            CCString* strNNN1 = CCString::createWithFormat("%d", n_FullHouse2);
            if ( n_FullHouse1 == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_FullHouse1 == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_FullHouse1 == 13 )
            {
                strNNN = CCString::create("K");
            }
            if ( n_FullHouse1 == 1 )
            {
                strNNN = CCString::create("A");
            }
            if ( n_FullHouse2 == 11 )
            {
                strNNN1 = CCString::create("J");
            }
            if ( n_FullHouse2 == 12 )
            {
                strNNN1 = CCString::create("Q");
            }
            if ( n_FullHouse2 == 13 )
            {
                strNNN1 = CCString::create("K");
            }
            if ( n_FullHouse2 == 1 )
            {
                strNNN1 = CCString::create("A");
            }
            strNNN = CCString::createWithFormat("%s, %s", strNNN->getCString(), strNNN1->getCString());
        }
        if ( card_results == card_flush )
        {
            nTempBet = nCurBet*8;
            strKKK = CCString::create("Flush");
            strNNN = CCString::createWithFormat("%d", n_Flush1);
            CCString* strNNN1 = CCString::createWithFormat("%d", n_Flush2);
            CCString* strNNN2 = CCString::createWithFormat("%d", n_Flush3);
            CCString* strNNN3 = CCString::createWithFormat("%d", n_Flush4);
            CCString* strNNN4 = CCString::createWithFormat("%d", n_Flush5);
            if ( n_Flush1 == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_Flush1 == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_Flush1 == 13 )
            {
                strNNN = CCString::create("K");
            }
            if ( n_Flush1 == 1 )
            {
                strNNN = CCString::create("A");
            }
            //
            if ( n_Flush2 == 11 )
            {
                strNNN1 = CCString::create("J");
            }
            if ( n_Flush2 == 12 )
            {
                strNNN1 = CCString::create("Q");
            }
            if ( n_Flush2 == 13 )
            {
                strNNN1 = CCString::create("K");
            }
            if ( n_Flush2 == 1 )
            {
                strNNN1 = CCString::create("A");
            }
            //
            if ( n_Flush3 == 11 )
            {
                strNNN2 = CCString::create("J");
            }
            if ( n_Flush3 == 12 )
            {
                strNNN2 = CCString::create("Q");
            }
            if ( n_Flush3 == 13 )
            {
                strNNN2 = CCString::create("K");
            }
            if ( n_Flush3 == 1 )
            {
                strNNN2 = CCString::create("A");
            }
            //
            if ( n_Flush4 == 11 )
            {
                strNNN3 = CCString::create("J");
            }
            if ( n_Flush4 == 12 )
            {
                strNNN3 = CCString::create("Q");
            }
            if ( n_Flush4 == 13 )
            {
                strNNN3 = CCString::create("K");
            }
            if ( n_Flush4 == 1 )
            {
                strNNN3 = CCString::create("A");
            }
            //
            if ( n_Flush5 == 11 )
            {
                strNNN4 = CCString::create("J");
            }
            if ( n_Flush5 == 12 )
            {
                strNNN4 = CCString::create("Q");
            }
            if ( n_Flush5 == 13 )
            {
                strNNN4 = CCString::create("K");
            }
            if ( n_Flush5 == 1 )
            {
                strNNN4 = CCString::create("A");
            }
            strNNN = CCString::createWithFormat("%s, %s, %s, %s, %s", strNNN->getCString(), strNNN1->getCString(), strNNN2->getCString(), strNNN3->getCString(), strNNN4->getCString());
            
        }
        
        if ( card_results == card_straight )
        {
            nTempBet = nCurBet*5;
            strKKK = CCString::create("Straight");
            strNNN = CCString::createWithFormat("%d", n_Straight1 );
            CCString* strNNN1 = CCString::createWithFormat("%d", n_Straight2);
            CCString* strNNN2 = CCString::createWithFormat("%d", n_Straight3);
            CCString* strNNN3 = CCString::createWithFormat("%d", n_Straight4);
            CCString* strNNN4 = CCString::createWithFormat("%d", n_Straight5);
            if ( n_Straight1 == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_Straight1 == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_Straight1 == 13 )
            {
                strNNN = CCString::create("K");
            }
            if ( n_Straight1 == 1 )
            {
                strNNN = CCString::create("A");
            }
            //
            if ( n_Straight2 == 11 )
            {
                strNNN1 = CCString::create("J");
            }
            if ( n_Straight2 == 12 )
            {
                strNNN1 = CCString::create("Q");
            }
            if ( n_Straight2 == 13 )
            {
                strNNN1 = CCString::create("K");
            }
            if ( n_Straight2 == 1 )
            {
                strNNN1 = CCString::create("A");
            }
            //
            if ( n_Straight3 == 11 )
            {
                strNNN2 = CCString::create("J");
            }
            if ( n_Straight3 == 12 )
            {
                strNNN2 = CCString::create("Q");
            }
            if ( n_Straight3 == 13 )
            {
                strNNN2 = CCString::create("K");
            }
            if ( n_Straight3 == 1 )
            {
                strNNN2 = CCString::create("A");
            }
            //
            if ( n_Straight4 == 11 )
            {
                strNNN3 = CCString::create("J");
            }
            if ( n_Straight4 == 12 )
            {
                strNNN3 = CCString::create("Q");
            }
            if ( n_Straight4 == 13 )
            {
                strNNN3 = CCString::create("K");
            }
            if ( n_Straight4 == 1 )
            {
                strNNN3 = CCString::create("A");
            }
            //
            if ( n_Straight5 == 11 )
            {
                strNNN4 = CCString::create("J");
            }
            if ( n_Straight5 == 12 )
            {
                strNNN4 = CCString::create("Q");
            }
            if ( n_Straight5 == 13 )
            {
                strNNN4 = CCString::create("K");
            }
            if ( n_Straight5 == 1 )
            {
                strNNN4 = CCString::create("A");
            }
            strNNN = CCString::createWithFormat("%s, %s, %s, %s, %s", strNNN->getCString(), strNNN1->getCString(), strNNN2->getCString(), strNNN3->getCString(), strNNN4->getCString());
        }
        if ( card_results == card_straight_flush )
        {
            nTempBet = nCurBet*200;
            strKKK = CCString::create("Straight Flush");
            strNNN = CCString::createWithFormat("%d", n_StrightFlush1);
            CCString* strNNN1 = CCString::createWithFormat("%d", n_StrightFlush2);
            CCString* strNNN2 = CCString::createWithFormat("%d", n_StrightFlush3);
            CCString* strNNN3 = CCString::createWithFormat("%d", n_StrightFlush4);
            CCString* strNNN4 = CCString::createWithFormat("%d", n_StrightFlush5);
            if ( n_StrightFlush1 == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_StrightFlush1 == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_StrightFlush1 == 13 )
            {
                strNNN = CCString::create("K");
            }
            if ( n_StrightFlush1 == 1 )
            {
                strNNN = CCString::create("A");
            }
            //
            if ( n_StrightFlush2 == 11 )
            {
                strNNN1 = CCString::create("J");
            }
            if ( n_StrightFlush2 == 12 )
            {
                strNNN1 = CCString::create("Q");
            }
            if ( n_StrightFlush2 == 13 )
            {
                strNNN1 = CCString::create("K");
            }
            if ( n_StrightFlush2 == 1 )
            {
                strNNN1 = CCString::create("A");
            }
            //
            if ( n_StrightFlush3 == 11 )
            {
                strNNN2 = CCString::create("J");
            }
            if ( n_StrightFlush3 == 12 )
            {
                strNNN2 = CCString::create("Q");
            }
            if ( n_StrightFlush3 == 13 )
            {
                strNNN2 = CCString::create("K");
            }
            if ( n_StrightFlush3 == 1 )
            {
                strNNN2 = CCString::create("A");
            }
            //
            if ( n_StrightFlush4 == 11 )
            {
                strNNN3 = CCString::create("J");
            }
            if ( n_StrightFlush4 == 12 )
            {
                strNNN3 = CCString::create("Q");
            }
            if ( n_StrightFlush4 == 13 )
            {
                strNNN3 = CCString::create("K");
            }
            if ( n_StrightFlush4 == 1 )
            {
                strNNN3 = CCString::create("A");
            }
            //
            if ( n_StrightFlush5 == 11 )
            {
                strNNN4 = CCString::create("J");
            }
            if ( n_StrightFlush5 == 12 )
            {
                strNNN4 = CCString::create("Q");
            }
            if ( n_StrightFlush5 == 13 )
            {
                strNNN4 = CCString::create("K");
            }
            if ( n_StrightFlush5 == 1 )
            {
                strNNN4 = CCString::create("A");
            }
            strNNN = CCString::createWithFormat("%s, %s, %s, %s, %s", strNNN->getCString(), strNNN1->getCString(), strNNN2->getCString(), strNNN3->getCString(), strNNN4->getCString());
        }
        if ( card_results == card_royal_flush )
        {
            nTempBet = nCurBet*1000;
            strKKK = CCString::create("Royal Flush");
            strNNN = CCString::createWithFormat("%d", n_RoyalFlush1);
            CCString* strNNN1 = CCString::createWithFormat("%d", n_RoyalFlush2);
            CCString* strNNN2 = CCString::createWithFormat("%d", n_RoyalFlush3);
            CCString* strNNN3 = CCString::createWithFormat("%d", n_RoyalFlush4);
            CCString* strNNN4 = CCString::createWithFormat("%d", n_RoyalFlush5);
            if ( n_RoyalFlush1 == 11 )
            {
                strNNN = CCString::create("J");
            }
            if ( n_RoyalFlush1 == 12 )
            {
                strNNN = CCString::create("Q");
            }
            if ( n_RoyalFlush1 == 13 )
            {
                strNNN = CCString::create("K");
            }
            if ( n_RoyalFlush1 == 1 )
            {
                strNNN = CCString::create("A");
            }
            //
            if ( n_RoyalFlush2 == 11 )
            {
                strNNN1 = CCString::create("J");
            }
            if ( n_RoyalFlush2 == 12 )
            {
                strNNN1 = CCString::create("Q");
            }
            if ( n_RoyalFlush2 == 13 )
            {
                strNNN1 = CCString::create("K");
            }
            if ( n_RoyalFlush2 == 1 )
            {
                strNNN1 = CCString::create("A");
            }
            //
            if ( n_RoyalFlush3 == 11 )
            {
                strNNN2 = CCString::create("J");
            }
            if ( n_RoyalFlush3 == 12 )
            {
                strNNN2 = CCString::create("Q");
            }
            if ( n_RoyalFlush3 == 13 )
            {
                strNNN2 = CCString::create("K");
            }
            if ( n_RoyalFlush3 == 1 )
            {
                strNNN2 = CCString::create("A");
            }
            //
            if ( n_RoyalFlush4 == 11 )
            {
                strNNN3 = CCString::create("J");
            }
            if ( n_RoyalFlush4 == 12 )
            {
                strNNN3 = CCString::create("Q");
            }
            if ( n_RoyalFlush4 == 13 )
            {
                strNNN3 = CCString::create("K");
            }
            if ( n_RoyalFlush4 == 1 )
            {
                strNNN3 = CCString::create("A");
            }
            //
            if ( n_RoyalFlush5 == 11 )
            {
                strNNN4 = CCString::create("J");
            }
            if ( n_RoyalFlush5 == 12 )
            {
                strNNN4 = CCString::create("Q");
            }
            if ( n_RoyalFlush5 == 13 )
            {
                strNNN4 = CCString::create("K");
            }
            if ( n_RoyalFlush5 == 1 )
            {
                strNNN4 = CCString::create("A");
            }
            strNNN = CCString::createWithFormat("%s, %s, %s, %s, %s", strNNN->getCString(), strNNN1->getCString(), strNNN2->getCString(), strNNN3->getCString(), strNNN4->getCString());
        }
    }
    
    if ( spWinLoss != NULL )
    {
        this->removeChild(spWinLoss, true);
        spWinLoss = NULL;
    }
    spWinLoss = CCSprite::create(strWinLost->getCString());
    spWinLoss->setPosition( ccp( winSize.width/2, winSize.height / 2 + 100*g_rY ) );
    this->addChild(spWinLoss, 15);
    
    if ( labGetBet != NULL )
    {
        this->removeChild(labGetBet, true);
        labGetBet = NULL;
    }
    float rW = 40;
    float rH = 0;
    if ( isIpad )
    {
        rW = 530;
        rH = 69;
    }
    else
    {
        rW = 210;
        rH = 27;
    }
    labGetBet = CCLabelAtlas::create(CCString::createWithFormat("%d", nTempBet)->getCString(), Tools::imageName("play_number2")->getCString(), rW/10, rH, '0');
    labGetBet->setPosition(ccp( winSize.width/2 - labGetBet->getContentSize().width/2, winSize.height/2 - labGetBet->getContentSize().height/2 ));
    this->addChild(labGetBet, 15);
    
    if ( labKKK != NULL )
    {
        this->removeChild(labKKK, true);
        labKKK = NULL;
    }
    if ( labNNN != NULL )
    {
        this->removeChild(labNNN, true);
        labNNN = NULL;
    }
    
    float rFont = 60;
    if ( isIpad )
    {
        rFont = 60;
    }
    else
    {
        rFont = 30;
    }
    labKKK = CCLabelTTF::create(strKKK->getCString(), "Arial Rounded MT Bold", rFont);
    labKKK->setPosition(ccp( winSize.width/2, winSize.height/2 - 90*g_rY ));
    this->addChild(labKKK, 15);
    
    
    labNNN = CCLabelTTF::create(strNNN->getCString(), "Arial Rounded MT Bold", rFont);
    labNNN->setPosition(ccp( winSize.width/2, winSize.height/2 - 150*g_rY ));
    this->addChild(labNNN, 15);
    
    g_nYourCoin += nTempBet;
    
    LetItRide::LetItRideSetting* bj = LetItRide::LetItRideSetting::shared();
    
#ifdef FreeApp
    if (!GameSetting::shared()->getRemoveAd())
    {
        bj->giveMeThirdGameOverAd();
    }
#endif
    
}

