//
//  ImagePicker.m
//  CCImagePicker
//
//  Created by w2moon on 13-4-7.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#import "CCImagePickerImplIOS.h"

static CCImagePickerImplIOS* s_picker = nil;

@implementation CCImagePickerImplIOS

@synthesize imagePicker;


+(BOOL)canUseCamera
{
    return [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
}

+(BOOL)canUsePhotoLibrary
{
    return [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary];
}

+(id)create:(UIImagePickerControllerSourceType)sourceType canEdit:(BOOL)edit
{
    CCImagePickerImplIOS* pIpc = [[CCImagePickerImplIOS alloc] init];

    if(s_picker != nil)
    {
        [s_picker closeView];
    }
    s_picker = pIpc;

    pIpc.imagePicker = [[UIImagePickerController alloc] init];
    pIpc.imagePicker.delegate = pIpc;
    if(edit)
    {
    [pIpc.imagePicker setAllowsEditing:edit];
    }
    [pIpc.view addSubview:pIpc.imagePicker.view];

    UIWindow *window = nil;
    NSArray *windows = [[UIApplication sharedApplication] windows];

    for (int i = 0; i < [windows count]; ++i)
    {
        window = [windows objectAtIndex:i];
        if([window isKeyWindow])
        {
            [window addSubview:pIpc.imagePicker.view];
            break;
        }
    }
    return pIpc;
}

-(void)closeView
{

    [self.imagePicker.view removeFromSuperview];

    s_picker = nil;
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [self closeView];
}


+(id)useCamera:(BOOL)edit
{
    return  [CCImagePickerImplIOS create:UIImagePickerControllerSourceTypeCamera canEdit:edit];

}

+(id)usePhotoLibrary:(BOOL)edit
{
    return [CCImagePickerImplIOS create:UIImagePickerControllerSourceTypePhotoLibrary canEdit:edit];
}


#pragma mark    imagePicker委托方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    //[self dismissModalViewControllerAnimated:YES];
	//[popoverController dismissPopoverAnimated:YES];
   // [self dismissViewControllerAnimated:YES completion:nil];
    [self closeView];

    UIImage *image = [info valueForKey:UIImagePickerControllerOriginalImage];//原图
    if([picker allowsEditing])
    {
        float w = image.size.width;
        float h = image.size.height;
        if (w < 320 && h < 480) {//如果图片  宽、高都分别小于320、480  那么，直接用截好的图,因为组件会自动把图片放大
            image = [info valueForKey:UIImagePickerControllerEditedImage];
        }
        else
        {//否则 就选择较短的(宽和高比较)作为标准 以中心为基准截取正方形
            image = [info valueForKey:UIImagePickerControllerEditedImage];
            w = image.size.width;
            h = image.size.height;
            float s = 0;
            s = MIN(w, h);
            image = [self image:image centerInSize:CGSizeMake(s, s)];
        }
    }
    NSData *imageData = UIImagePNGRepresentation(image);
   // hiddenView.image = image;
   // image = [self imageFromView:hiddenView];//为了发送给服务器

  //  photoView.image = image;

    //下面就应该变为nsdata然后传给服务器了...........以下省略N个字
}


//生成图片
- (UIImage *) imageFromView: (UIView *) theView
{
//	UIGraphicsBeginImageContext(theView.frame.size);
//	CGContextRef context = UIGraphicsGetCurrentContext();
//	[theView.layer renderInContext:context];
//	UIImage *theImage = UIGraphicsGetImageFromCurrentImageContext();
//	UIGraphicsEndImageContext();
//	return theImage;
    return NULL;
}

//改变图片大小
- (UIImage *) image: (UIImage *) image centerInSize: (CGSize) viewsize
{
	CGSize size = image.size;

	UIGraphicsBeginImageContext(viewsize);
	float dwidth = (viewsize.width - size.width) / 2.0f;
	float dheight = (viewsize.height - size.height) / 2.0f;

	CGRect rect = CGRectMake(dwidth, dheight, size.width, size.height);
	[image drawInRect:rect];

    UIImage *newimg = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();

    return newimg;
}

@end

#include "CCImagePicker.h"

namespace cocos2d { namespace extension {

bool CCImagePicker::canUseCamera()
{
    return [CCImagePickerImplIOS canUseCamera];
}

bool CCImagePicker::canUsePhotoLibrary()
{
    return [CCImagePickerImplIOS canUsePhotoLibrary];
}

void CCImagePicker::useCamera(void* obj,picker_callback func,bool edit)
{
    [CCImagePickerImplIOS useCamera:edit];
}

void CCImagePicker::usePhotoLibrary(void* obj,picker_callback func,bool edit)
{
    [CCImagePickerImplIOS usePhotoLibrary:edit];
}
}}
