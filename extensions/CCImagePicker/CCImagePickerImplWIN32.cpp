//
//  ImagePicker.cpp
//  CCImagePicker
//
//  Created by w2moon on 13-4-7.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#include "CCImagePicker.h"

namespace cocos2d { namespace extension {

static CCImagePicker *s_pImagePicker = NULL;

CCImagePicker* CCImagePicker::getInstance()
{
    if (s_pImagePicker == NULL) {
        s_pImagePicker = new CCImagePicker();
    }
    
    return s_pImagePicker;
}

void CCImagePicker::destroyInstance()
{
	s_pImagePicker->release();
}

CCImagePicker::CCImagePicker()
{
}

CCImagePicker::~CCImagePicker()
{
  s_pImagePicker = NULL;
}

bool CCImagePicker::canUseCamera()
{
    return false;
}

bool CCImagePicker::canUsePhotoLibrary()
{
    return false;
}

void CCImagePicker::dispatchCallbacks(float delta)
{
}

void CCImagePicker::useCamera(CCObject* pTarget, SEL_CallFuncO pSelector,int width,int height,bool edit)
{
	CCImage* image = new CCImage();
	image->autorelease();

	unsigned char* bytes = new unsigned char[4*width*height];
	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
		{
			int start = (i*width+j)*4;
			bytes[start] = 128;
			bytes[start+1] = 128;
			bytes[start+2] = 128;
			bytes[start+3] = 255;
		}
	image->initWithImageData(bytes,4*64,CCImage::kFmtRawData,width,height,4);
	delete[] bytes;
    
    (pTarget->*pSelector)(image);
}

void CCImagePicker::usePhotoLibrary(CCObject* pTarget, SEL_CallFuncO pSelector,int width,int height,bool edit)
{
	CCImage* image = new CCImage();
	image->autorelease();

	unsigned char* bytes = new unsigned char[4*width*height];
	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
		{
			int start = (i*width+j)*4;
			bytes[start] = 128;
			bytes[start+1] = 128;
			bytes[start+2] = 128;
			bytes[start+3] = 255;
		}
	image->initWithImageData(bytes,4*64,CCImage::kFmtRawData,width,height,4);
	delete[] bytes;
    
    (pTarget->*pSelector)(image);
}
}}
