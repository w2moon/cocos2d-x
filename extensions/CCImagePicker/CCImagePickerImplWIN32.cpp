//
//  ImagePicker.cpp
//  CCImagePicker
//
//  Created by w2moon on 13-4-7.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#include "CCImagePicker.h"

namespace cocos2d { namespace extension {

bool CCImagePicker::canUseCamera()
{
    return false;
}

bool CCImagePicker::canUsePhotoLibrary()
{
    return false;
}

void CCImagePicker::useCamera(bool edit)
{
}

void CCImagePicker::usePhotoLibrary(bool edit)
{
}
}}
