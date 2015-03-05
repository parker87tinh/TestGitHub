//
//  MainMenuLayer.h
//  LetItRide
//
//  Created by lion on 2/17/14.
//
//

#ifndef __LetItRide__MainMenuLayer__
#define __LetItRide__MainMenuLayer__

#include "cocos2d.h"

USING_NS_CC;

class MainMenuLayer : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    virtual ~MainMenuLayer();
    virtual void update(float dt);
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void initVariable();
    void initImages();
    void initItems();
    void initLabels();
    
    CCMenuItemImage* itemPlay;
    CCMenuItemImage* itemFreeGames;
    CCMenuItemImage* itemMoreGames;
    CCMenuItemImage* itemRemoveAds;
    CCMenuItemImage* itemRestore;
    CCMenuItemImage* itemHighScore;
    
    CCMenuItemToggle* itemMusic;
    CCMenuItemToggle* itemSound;
    
    void onClickPlay(CCObject* sender);
    void onClickFreeGames(CCObject* sender);
    void onClickMoreGames(CCObject* sender);
    void onClickRemoveAds(CCObject* sender);
    void onClickRestore(CCObject* sender);
    void onClickHighScore(CCObject* sender);
    
    void onClickMusic(CCObject* sender);
    void onClickSound(CCObject* sender);
    
    CCSize winSize;
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainMenuLayer);
};

#endif /* defined(__LetItRide__MainMenuLayer__) */
