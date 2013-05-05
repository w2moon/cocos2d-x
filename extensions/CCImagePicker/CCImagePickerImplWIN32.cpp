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
#define PICKER_TEST 128
void CCImagePicker::useCamera(CCObject* pTarget, SEL_CallFuncO pSelector,bool edit)
{
	CCImage* image = new CCImage();
	image->autorelease();

	unsigned char* bytes = new unsigned char[4*PICKER_TEST*PICKER_TEST];
	for(int i=0;i<PICKER_TEST;++i)
		for(int j=0;j<PICKER_TEST;++j)
		{
			int start = (i*PICKER_TEST+j)*4;
			bytes[start] = 128;
			bytes[start+1] = 128;
			bytes[start+2] = 128;
			bytes[start+3] = 255;
		}
	image->initWithImageData(bytes,4*64,CCImage::kFmtRawData,PICKER_TEST,PICKER_TEST,4);
	delete[] bytes;
    
    (pTarget->*pSelector)(image);
}

void CCImagePicker::usePhotoLibrary(CCObject* pTarget, SEL_CallFuncO pSelector,bool edit)
{
	CCImage* image = new CCImage();
	image->autorelease();

	unsigned char* bytes = new unsigned char[4*PICKER_TEST*PICKER_TEST];
	for(int i=0;i<PICKER_TEST;++i)
		for(int j=0;j<PICKER_TEST;++j)
		{
			int start = (i*PICKER_TEST+j)*4;
			bytes[start] = 128;
			bytes[start+1] = 128;
			bytes[start+2] = 128;
			bytes[start+3] = 255;
		}
	image->initWithImageData(bytes,4*64,CCImage::kFmtRawData,PICKER_TEST,PICKER_TEST,4);
	delete[] bytes;
    
    (pTarget->*pSelector)(image);
}
}}
