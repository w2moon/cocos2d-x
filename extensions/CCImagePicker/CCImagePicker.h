//
//  ImagePicker.h
//  CCImagePicker
//
//  Created by w2moon on 13-4-8.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#ifndef __CCImagePicker__
#define __CCImagePicker__

namespace cocos2d { namespace extension {
    
    typedef void (*picker_callback)(int width,int height,char* data);

class CCImagePicker
{
public:
    static bool canUseCamera();
    static bool canUsePhotoLibrary();
    static void useCamera(void* obj,picker_callback callback,bool edit = false);
    static void usePhotoLibrary(void* obj,picker_callback callback,bool edit = false);
};
}}

#endif /* defined__CCImagePicker__) */
