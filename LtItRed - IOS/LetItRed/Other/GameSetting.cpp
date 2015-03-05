//
//  GameSetting.cpp
//  Black Jack
//
//  Created by lion on 1/6/14.
//
//

#include "GameSetting.h"
#include "Extern.h"

USING_NS_CC;

//#define PZ_GET_FULLPATH( P ) CCFileUtils::sharedFileUtils()->fullPathFromRelativePath( P )
static GameSetting* gameSetting = NULL;

GameSetting::GameSetting()
{
    loaded = false;
    isRemoveAds = false;
    isSound = true;
    isMusic = true;
}

GameSetting::~GameSetting()
{
    
}

GameSetting* GameSetting::shared()
{
    if ( gameSetting == NULL )
    {
        gameSetting = new GameSetting();
    }
    return gameSetting;
}

void GameSetting::load()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    bool bLoad = userDefault->getBoolForKey("LetItRide2");
    if ( !bLoad )
    {
        g_nYourCoin = 2000;
        userDefault->setFloatForKey("LetItRide_Coins", g_nYourCoin);
        userDefault->setBoolForKey("LetItRide2", true);
        isRemoveAds = false;
        isSound = true;
        isMusic = true;
        userDefault->setBoolForKey("LetItRide_Music", isMusic);
        userDefault->setBoolForKey("LetItRide_Sound", isSound);
    }
    else
    {
        g_nYourCoin = userDefault->getFloatForKey("LetItRide_Coins");
        isRemoveAds = userDefault->getBoolForKey("LetItRide_removeAds");
        isMusic = userDefault->getBoolForKey("LetItRide_Music");
        isSound = userDefault->getBoolForKey("LetItRide_Sound");
    }
    
    
}

void GameSetting::saveCoins()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setFloatForKey("LetItRide_Coins", g_nYourCoin);
}

int GameSetting::getCoins()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    g_nYourCoin = userDefault->getFloatForKey("LetItRide_Coins");
    return g_nYourCoin;
}

void GameSetting::saveRemoveAd(bool b)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey("LetItRide_removeAds", b);
}

bool GameSetting::getRemoveAd()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    
    isRemoveAds = userDefault->getBoolForKey("LetItRide_removeAds",false);
    return isRemoveAds;
}

void GameSetting::saveMusic(bool b)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey("LetItRide_Music", b);
}
void GameSetting::saveSound(bool b)
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setBoolForKey("LetItRide_Sound", b);
}
bool GameSetting::isEnableMusic()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    isMusic = userDefault->getBoolForKey("LetItRide_Music");
    return isMusic;
}
bool GameSetting::isEnableSound()
{
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    isSound = userDefault->getBoolForKey("LetItRide_Sound");
    return isSound;
}