//
//  PlayLayer.h
//  LetItRide
//
//  Created by lion on 1/28/14.
//
//

#ifndef __LetItRide__PlayLayer__
#define __LetItRide__PlayLayer__

#include "cocos2d.h"

#include "GameConfig.h"
#include "Extern.h"

#include "PauseLayer.h"
#include "PopLayer.h"

#define MIN_BET 1
#define MAX_BET 300

USING_NS_CC;

class PlayLayer : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    virtual ~PlayLayer();
    virtual void update(float dt);
    virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent);
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void initVariable();
    void initImages();
    void initItems();
    void initLabels();
    
    void onClick5Coin(CCObject* sender);
    void onClick10Coin(CCObject* sender);
    void onClick25Coin(CCObject* sender);
    void onClick50Coin(CCObject* sender);
    void onClick100Coin(CCObject* sender);
    void onClickX(CCObject* sender);
    
    void onClickDeal(CCObject* sender);
    void onClickKeep(CCObject* sender);
    void onClickDrop(CCObject* sender);
    
    void onClickPause(CCObject* sender);
    void onClickBuy(CCObject* sender);
    
    void drawCoinMark(const char* chName);
    void unVisibleCoinMark();
    void dropCoinMark(int nNum);
    
    void createCard();
    void drawBetLabel();
    void checkCard();
    
    void getCardNumMark();
    int getCardMark(int nI);
    
    void showResult();
    
    CCPoint getCardPosition(int nIndex);
    
    CCSize winSize;
    
    CCSprite* spCard[5];
    CCSprite* spCoinMark[3];
    
    CCMenuItemImage* item5Coin;
    CCMenuItemImage* item10Coin;
    CCMenuItemImage* item25Coin;
    CCMenuItemImage* item50Coin;
    CCMenuItemImage* item100Coin;
    CCMenuItemImage* itemX;
    
    CCMenuItemImage* itemDeal;
    CCMenuItemImage* itemKeep;
    CCMenuItemImage* itemDrop;
    
    CCMenu* menu;
    
    GAME_TYPE mType;
    
    CCLabelAtlas* labRemainBet;
    CCLabelAtlas* labCurBet;
    
    CCLabelAtlas* labGetBet;
    CCLabelTTF* labKKK;
    CCLabelTTF* labNNN;
    
    CCSprite* spBoard;
    CCSprite* spWinLoss;
    
    CCSprite* spLeftCoinMark;
    CCSprite* spCenterCoinMark;
    CCSprite* spRightCoinMark;
    
//    int nRemainBet;//number for display
    int nCurBet;//number for display
    int nTempBet;
    
    int nIncreaseBet;
    
    int nRandom;
    bool bDeal;
    int nCardIndex;
    int nCardNum[5];
    int nCardMark[5];
    bool bCard[52];
    
    int nPlayNum;
    int nDropTouchNum;
    
    int nTime;
    
    int m_nCNum[13];
    
    CARD_RESULT card_results;
    
    PauseLayer* pPause;
    PopLayer* pPop;
    
    int n_OnePair;
    int n_TwoPair1;
    int n_TwoPair2;
    int n_3Kind;
    int n_Straight1;
    int n_Straight2;
    int n_Straight3;
    int n_Straight4;
    int n_Straight5;
    int n_Flush1;
    int n_Flush2;
    int n_Flush3;
    int n_Flush4;
    int n_Flush5;
    int n_FullHouse1;
    int n_FullHouse2;
    int n_4Kind;
    int n_StrightFlush1;
    int n_StrightFlush2;
    int n_StrightFlush3;
    int n_StrightFlush4;
    int n_StrightFlush5;
    int n_RoyalFlush1;
    int n_RoyalFlush2;
    int n_RoyalFlush3;
    int n_RoyalFlush4;
    int n_RoyalFlush5;
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PlayLayer);
};

#endif /* defined(__LetItRide__PlayLayer__) */
