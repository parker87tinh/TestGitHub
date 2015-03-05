//
//  Extern.h
//  Black Jack
//
//  Created by lion on 12/19/13.
//
//

#ifndef Black_Jack_Extern_h
#define Black_Jack_Extern_h

extern bool g_fIsRetinaDisable;
extern bool g_fIsIPhoneFive;
extern float g_rX;
extern float g_rY;
extern int g_nRXForIFive;
extern bool m_fHighRes;

extern float g_rScale;
extern bool isIpad;

extern int g_nYourCoin;

extern bool g_bPause;
extern bool g_bBuy;
extern bool g_bMainMenu;

typedef enum {

    card_non,
    card_royal_flush,
    card_straight_flush,
    card_4_kind,
    card_full_house,
    card_flush,
    card_straight,
    card_3_kind,
    card_tow_pair,
    card_one_pair
    
}CARD_RESULT;

#endif
