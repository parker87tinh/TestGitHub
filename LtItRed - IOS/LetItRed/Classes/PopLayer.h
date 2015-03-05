//
//  PopLayer.h
//  LetItRide
//
//  Created by lion on 2/18/14.
//
//

#ifndef __LetItRide__PopLayer__
#define __LetItRide__PopLayer__

#include "cocos2d.h"

USING_NS_CC;

class PopLayer : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    virtual ~PopLayer();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    CCSprite* spPop;
    CCSprite* spPop1;
    CCSprite* spPop2;
    CCSprite* spPop3;
    CCSprite* spPop4;
    CCSprite* spPop5;
    CCMenuItemImage* itemPop1;
    CCMenuItemImage* itemPop2;
    CCMenuItemImage* itemPop3;
    CCMenuItemImage* itemPop4;
    CCMenuItemImage* itemPop5;
    CCMenuItemImage* itemPopBuy;
    CCMenuItemImage* itemPopClose;
    CCMenu* menuPop;
    void onClickPop1(CCObject* sender);
    void onClickPop2(CCObject* sender);
    void onClickPop3(CCObject* sender);
    void onClickPop4(CCObject* sender);
    void onClickPop5(CCObject* sender);
    void onClickPopBuy(CCObject* sender);
    void onClickPopClose(CCObject* sender);
    
    CCSize winSize;
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PopLayer);
};

#endif /* defined(__LetItRide__PopLayer__) */
