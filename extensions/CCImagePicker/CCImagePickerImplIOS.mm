//
//  ImagePicker.m
//  CCImagePicker
//
//  Created by w2moon on 13-4-7.
//  Copyright (c) 2013年 w2moon. All rights reserved.
//

#import "CCImagePickerImplIOS.h"

static CCImagePickerImplIOS* s_picker = nil;

USING_NS_CC_EXT;

@implementation CCImagePickerImplIOS

@synthesize pPickedImage;
@synthesize picked;
@synthesize imagePicker;
@synthesize pTarget,pSelector;
@synthesize deswidth,desheight;

+(BOOL)canUseCamera
{
    return [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera];
}

+(BOOL)canUsePhotoLibrary
{
    return [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary];
}

+(id)create:(UIImagePickerControllerSourceType)sourceType canEdit:(BOOL)edit with:(CCObject*)obj func:(SEL_CallFuncO)cb  width:(int)width height:(int)height
{
    CCImagePickerImplIOS* pIpc = [[CCImagePickerImplIOS alloc] init];

    if(s_picker != nil)
    {
        [s_picker closeView];
    }
    s_picker = pIpc;
    
  
        pIpc.deswidth = width;
        pIpc.desheight = height;
   
    pIpc.picked = false;
    obj->retain();
    pIpc.pTarget = obj;
    pIpc.pSelector = cb;

    pIpc.imagePicker = [[UIImagePickerController alloc] init];
    pIpc.imagePicker.sourceType = sourceType;
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

-(void)dealloc
{
    
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


+(id)useCamera:(BOOL)edit with:(CCObject*)obj func:(SEL_CallFuncO)cb  width:(int)width height:(int)height
{
    return  [CCImagePickerImplIOS create:UIImagePickerControllerSourceTypeCamera canEdit:edit with:obj func:cb  width:width height:height];

}

+(id)usePhotoLibrary:(BOOL)edit with:(CCObject*)obj func:(SEL_CallFuncO)cb  width:(int)width height:(int)height
{
    return [CCImagePickerImplIOS create:UIImagePickerControllerSourceTypePhotoLibrary canEdit:edit with:obj func:cb  width:width height:height];
}


#pragma mark    imagePicker委托方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    //[self dismissModalViewControllerAnimated:YES];
	//[popoverController dismissPopoverAnimated:YES];
   // [self dismissViewControllerAnimated:YES completion:nil];
    

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
   // NSData *imageData = UIImagePNGRepresentation(image);
    
    CGImageRef imageRef = [image CGImage];
    NSUInteger width = deswidth;//CGImageGetWidth(imageRef);
    NSUInteger height = desheight;//CGImageGetHeight(imageRef);
    int length = height * width * 4 * sizeof(unsigned char);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    unsigned char *rawData = new unsigned char[length];
    NSUInteger bytesPerPixel = 4;
    NSUInteger bytesPerRow = bytesPerPixel * width;
    NSUInteger bitsPerComponent = 8;
    CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                                                 bitsPerComponent, bytesPerRow, colorSpace,
                                                 kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(colorSpace);
    
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
    CGContextRelease(context);
   // hiddenView.image = image;
   // image = [self imageFromView:hiddenView];//为了发送给服务器

  //  photoView.image = image;

    //下面就应该变为nsdata然后传给服务器了...........以下省略N个字

    CCImage* pickdata= new CCImage();
    pickdata->autorelease();
    
    pickdata->initWithImageData(rawData,length,CCImage::kFmtRawData,width,height,8);
    
    delete[] rawData;
   // s_picker.picked = true;
  //  s_picker.pPickedImage = pickdata;
  //  pickdata->retain();
    
   (pTarget->*pSelector)(pickdata);
    pTarget->release();
    [s_picker closeView];
    
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



namespace cocos2d { namespace extension {


static CCImagePicker *s_pImagePicker = NULL;
    
    static CCImagePickerImplIOS* s_pImpl = NULL;

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
//    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
//                                                                   schedule_selector(CCImagePicker::dispatchCallbacks), this, 0, false);
//    CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
}

CCImagePicker::~CCImagePicker()
{
  s_pImagePicker = NULL;
}
    
void CCImagePicker::dispatchCallbacks(float delta)
{
    if(!s_picker.picked)
    {
        return;
    }
    s_picker.picked = false;
    CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);
    
    (s_picker.pTarget->*(s_picker.pSelector))(s_picker.pPickedImage);
    
    s_picker.pTarget->release();
    
    s_picker.pPickedImage->release();
    [s_picker closeView];
}

bool CCImagePicker::canUseCamera()
{
    return [CCImagePickerImplIOS canUseCamera];
}

bool CCImagePicker::canUsePhotoLibrary()
{
    return [CCImagePickerImplIOS canUsePhotoLibrary];
}

void CCImagePicker::useCamera(CCObject* pTarget, SEL_CallFuncO pSelector,int width,int height,bool edit)
{
    [CCImagePickerImplIOS useCamera:edit with:pTarget func:pSelector width:width height:height];
   // CCDirector::sharedDirector()->getScheduler()->resumeTarget(this);
}

void CCImagePicker::usePhotoLibrary(CCObject* pTarget, SEL_CallFuncO pSelector,int width,int height,bool edit)
{
   [CCImagePickerImplIOS usePhotoLibrary:edit with:pTarget func:pSelector width:width height:height];
  // CCDirector::sharedDirector()->getScheduler()->resumeTarget(this);
}
}}
