//
//  ImagePicker.h
//  CCImagePicker
//
//  Created by w2moon on 13-4-7.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CCImagePickerImplIOS : UIViewController<UIImagePickerControllerDelegate,UINavigationControllerDelegate>
{
    UIImageView *photoView;
    UIImagePickerController *imagePicker;
}
@property(nonatomic,strong)UIImagePickerController *imagePicker;

+(BOOL)canUseCamera;
+(BOOL)canUsePhotoLibrary;


+(id)useCamera:(BOOL)edit;
+(id)usePhotoLibrary:(BOOL)edit;

@end
