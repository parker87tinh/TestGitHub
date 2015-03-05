//
//  Tools.cpp
//  Black Jack
//
//  Created by lion on 12/19/13.
//
//

#include "Tools.h"
#include "Extern.h"

using namespace cocos2d;


bool Tools::isHighRes()
{
    bool isB = false;
    if (m_fHighRes)
        isB = true;
    return isB;
}

CCString* Tools::imageName(const char* name)
{
    CCString* szName;
    
    
    if (isHighRes())
    {
        if (isIpad)
            szName = CCString::createWithFormat("%s-ipadhd.png", name);
        
        else
            szName = CCString::createWithFormat("%s-hd.png", name);
        
    }
    else
    {
        if (isIpad)
            szName = CCString::createWithFormat("%s-ipad.png", name);
        else
            szName = CCString::createWithFormat("%s.png", name);
    }
    
    
    return szName;
    
}