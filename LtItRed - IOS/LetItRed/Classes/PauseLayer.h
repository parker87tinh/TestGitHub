//
//  PauseLayer.h
//  LetItRide
//
//  Created by lion on 2/17/14.
//
//

#ifndef __LetItRide__PauseLayer__
#define __LetItRide__PauseLayer__

#include "cocos2d.h"

USING_NS_CC;

class PauseLayer : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    virtual ~PauseLayer();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void initVariable();
    void initImages();
    void initItems();
    void initLabels();
    
    CCMenuItemImage* itemResume;
    CCMenuItemImage* itemMainMenu;
    
    void onClickResume(CCObject* sender);
    void onClickMainMenu(CCObject* sender);
    
    
    CCSize winSize;
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PauseLayer);
};

#endif /* defined(__LetItRide__PauseLayer__) */
