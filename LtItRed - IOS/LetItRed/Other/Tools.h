//
//  Tools.h
//  Black Jack
//
//  Created by lion on 12/19/13.
//
//

#ifndef __Black_Jack__Tools__
#define __Black_Jack__Tools__

#include "cocos2d.h"

USING_NS_CC;

class Tools
{
public:
    static CCString* imageName(const char* name);
    static bool isHighRes();
};

#endif /* defined(__Black_Jack__Tools__) */
