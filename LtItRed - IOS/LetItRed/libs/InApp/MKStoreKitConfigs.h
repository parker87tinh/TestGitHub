//
//  MKStoreKitConfigs.h
//  MKStoreKit (Version 4.2)
//
//  Created by Mugunth Kumar on 17-Nov-2010.
//  Copyright 2010 Steinlogic. All rights reserved.
//	File created using Singleton XCode Template by Mugunth Kumar (http://mugunthkumar.com
//  Permission granted to do anything, commercial/non-commercial with this file apart from removing the line/URL above
//  Read my blog post at http://mk.sg/1m on how to use this code

//  Licensing (Zlib)
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.

//  As a side note on using this code, you might consider giving some credit to me by
//	1) linking my website from your app's website
//	2) or crediting me inside the app's credits page
//	3) or a tweet mentioning @mugunthkumar
//	4) A paypal donation to mugunth.kumar@gmail.com


// To avoid making mistakes map plist entries to macros on this page.
// when you include MKStoreManager in your clss, these macros get defined there

#ifndef IN_APP_VAL
#define IN_APP_VAL

//1 SPIN TOKEN : 0.99$
//6 SPIN TOKEN : 4.99$
//15 SPIN TOKEN : 9.99$
//30 SPIN TOKEN + PLATINUM USER STATUES : 19.99$
//50 SPIN TOKEN + DIAMOND USER STATUES : 49.99$
//125 SPIN TOKEN + WHALE USER STATUES : 99.99$

//#ifdef PAID_ELE_VERSION
#ifdef PaidApp

#define strCoin1000            @"com.goldcoin.LetItRedPaid.1000Coins"
#define strCoin2500             @"com.goldcoin.LetItRedPaid.2500Coins"
#define strCoin6000            @"com.goldcoin.LetItRedPaid.6000Coins"
#define strCoin14000             @"com.goldcoin.LetItRedPaid.14000Coins"
#define strCoin30000            @"com.goldcoin.LetItRedPaid.30000Coins"
#define strRemoveAd             @""

#endif

#ifdef FreeApp

#define strCoin1000            @"com.goldcoin.LetItRed.1000Coins"
#define strCoin2500             @"com.goldcoin.LetItRed.2500Coins"
#define strCoin6000            @"com.goldcoin.LetItRed.6000Coins"
#define strCoin14000             @"com.goldcoin.LetItRed.14000Coins"
#define strCoin30000            @"com.goldcoin.LetItRed.30000Coins"
#define strRemoveAd             @"com.goldcoin.LetItRedFree.RemoveAds"

#endif



#define SERVER_PRODUCT_MODEL 0
#define OWN_SERVER 0
#define REVIEW_ALLOWED 0

#define kSharedSecret @"<FILL IN YOUR SHARED SECRET HERE>"

#endif