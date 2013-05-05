//
//  ImagePicker.h
//  CCImagePicker
//
//  Created by w2moon on 13-4-7.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "CCImagePicker.h"

using namespace cocos2d;

@interface CCImagePickerImplIOS : UIViewController<UIImagePickerControllerDelegate,UINavigationControllerDelegate>
{
    UIImageView *photoView;
    UIImagePickerController *imagePicker;
    
    CCObject* pTarget;
    SEL_CallFuncO pSelector;
    
    CCImage* pPickedImage;
    
    bool picked;
    int deswidth;
    int desheight;
    
}
@property(readwrite)bool picked;

@property(nonatomic,strong)UIImagePickerController *imagePicker;

@property(nonatomic,readwrite)CCImage* pPickedImage;
@property(nonatomic,readwrite)CCObject* pTarget;
@property(nonatomic,readwrite)SEL_CallFuncO pSelector;
@property(nonatomic,readwrite)int deswidth;
@property(nonatomic,readwrite)int desheight;

+(BOOL)canUseCamera;
+(BOOL)canUsePhotoLibrary;


+(id)useCamera:(BOOL)edit with:(CCObject*)obj func:(SEL_CallFuncO)cb  width:(int)width height:(int)height;
+(id)usePhotoLibrary:(BOOL)edit with:(CCObject*)obj func:(SEL_CallFuncO)cb  width:(int)width height:(int)height;

@end
