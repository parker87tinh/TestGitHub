//
//  GameSetting.h
//  Black Jack
//
//  Created by lion on 1/6/14.
//
//

#ifndef __Black_Jack__GameSetting__
#define __Black_Jack__GameSetting__

#include "cocos2d.h"

USING_NS_CC;

class GameSetting
{
public:
    GameSetting();
    ~GameSetting();
    
    static GameSetting* shared();
    void load();
    
    static void saveCoins();
    static int getCoins();
    
    void saveRemoveAd(bool b);
    bool getRemoveAd();
    
    bool loaded;
    
    CCDictionary *gameDict;
    bool isRemoveAds;
    
    bool isSound;
    bool isMusic;
    
    void saveMusic(bool b);
    void saveSound(bool b);
    bool isEnableMusic();
    bool isEnableSound();
    
};

#endif /* defined(__Black_Jack__GameSetting__) */
