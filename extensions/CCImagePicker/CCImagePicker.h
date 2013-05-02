//
//  ImagePicker.h
//  CCImagePicker
//
//  Created by w2moon on 13-4-8.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#ifndef __CCImagePicker__
#define __CCImagePicker__

#include "cocos2d.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class CCImagePickerData : public CCObject
{
public:
    CCImagePickerData():width(0),height(0),data(NULL){}
    ~CCImagePickerData()
    {
        delete[] data;
    }
    int getWidth(){return width;}
    void setWidth(int w){width = w;}
    int getHeight(){return height;}
    void setHeight(int h){height = h;}
    unsigned char* getData(){return data;}
    void setData(unsigned char* d){data = d;}
private:
    int width;
    int height;
    unsigned char* data;
};

class CCImagePicker
{
public:
    static bool canUseCamera();
    static bool canUsePhotoLibrary();
    static void useCamera(CCObject* pTarget, SEL_CallFuncO pSelector,bool edit = false);
    static void usePhotoLibrary(CCObject* pTarget, SEL_CallFuncO pSelector,bool edit = false);
};
NS_CC_EXT_END

#endif /* defined__CCImagePicker__) */
