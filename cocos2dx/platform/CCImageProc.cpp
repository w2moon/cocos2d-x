/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/



#include "CCImage.h"
#include "support/base64.h"
#include "CCFileUtils.h"
#include "support/zip_support/ZipUtils.h"

#include <cctype>
#include <iostream>
#include <iterator>
#include "opencv/cv.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/photo/photo.hpp"
#include "cocoa/CCArray.h"
#include "asmmodel.h"

NS_CC_BEGIN

CCImage* CCImage::create()
{
	CCImage * pRet = new CCImage();
    if (pRet)
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}


std::string CCImage::toBase64()
{
	unsigned char* zip = NULL;
	unsigned char* out = NULL;
	int length = m_nWidth*m_nHeight*4;
	int ziplength = ZipUtils::ccDeflateMemory(m_pData,length,&zip);
	base64Encode(zip,ziplength,&out);
	std::string ret = reinterpret_cast<char*>(out);
	delete[] zip;
	delete[] out;
	return ret;
}

bool CCImage::initWithBase64(const char * pStrData, 
                           int nWidth,
                           int nHeight)
{
	unsigned char *bytes = NULL;
	unsigned char* zip = NULL;
	int len = base64Decode(reinterpret_cast< const unsigned char* >(pStrData),strlen(pStrData)+1,&bytes);
	
	len = ZipUtils::ccInflateMemory(bytes,len,&zip);
	initWithImageData(zip,len,kFmtRawData,nWidth,nHeight,4);

	delete[] zip;
	delete[] bytes;
	return true;
}

void CCImage::resize(int nWidth,int nHeight)
{
	if(nWidth == m_nWidth && nHeight == m_nHeight){
		return;
	}
	int bytes = m_bHasAlpha ? 4 : 3;
	IplImage* src = cvCreateImageHeader( cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U, bytes); 
	IplImage* dst = cvCreateImage( cvSize(nWidth,nHeight), IPL_DEPTH_8U, bytes); 

	cvSetData(src,m_pData,m_nWidth*bytes);

	if(true)
	{
		if(nWidth*nHeight>m_nWidth*m_nHeight)
		{
			cvPyrUp (src, dst, CV_GAUSSIAN_5x5);
		}
		else
		{
				cvPyrDown (src, dst, CV_GAUSSIAN_5x5);
		}
	}
	else
	{
		cvResize(src,dst);
	}

	delete [] m_pData;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_pData = new unsigned char[dst->imageSize];
	unsigned char* pData = NULL;
	cvGetRawData(dst,&pData);
    memcpy(m_pData,pData,dst->imageSize);
	cvReleaseImageHeader(&src);
	cvReleaseImageHeader(&dst);
}

CCArray* CCImage::cut(const char* cascadeFile,const char* modelPath)
{
	int verboseL = 0;
	cv::CascadeClassifier objCascadeClassfifier; 
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(cascadeFile);
	if( !objCascadeClassfifier.load( fullPath ) )
	{
		return NULL;
	}
	StatModel::ASMModel asmModel;
	asmModel.loadFromFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(modelPath));
	  // Load image.
    int bytes = m_bHasAlpha ? 4 : 3;
	IplImage* src = cvCreateImageHeader( cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U, bytes);
	cvSetData(src,m_pData,m_nWidth*bytes);
	cv::Mat img = cv::cvarrToMat(src);
    if (img.empty()) {
        return NULL;
    }
	CCArray* arr = CCArray::create();

    // Face detection.
    std::vector< cv::Rect > faces;
    objCascadeClassfifier.detectMultiScale(
        img, faces,
        1.2, 2, CV_HAAR_SCALE_IMAGE, cv::Size(60, 60) );

    // Fit to ASM!
    std::vector < StatModel::ASMFitResult > fitResult = asmModel.fitAll(img, faces, verboseL);

	std::vector < StatModel::ASMFitResult >::iterator iterb = fitResult.begin();
	std::vector < StatModel::ASMFitResult >::iterator itere = fitResult.end();
	for(;iterb != itere;iterb++){
		//vector< Point_<int> > ptl;
	}
    

	//std::vector < Point_<int> >
	//fitResult.t
	
	return arr;

}
CCArray* CCImage::parse(const char* cascadeFile,double scale)
{
	cv::CascadeClassifier cascade;
	std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(cascadeFile);
	if( !cascade.load( fullPath ) )
	{
		return NULL;
	}
	int bytes = m_bHasAlpha ? 4 : 3;
	IplImage* src = cvCreateImageHeader( cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U, bytes);
	cvSetData(src,m_pData,m_nWidth*bytes);
	cv::Mat img = cv::cvarrToMat(src);
	CCArray* arr = CCArray::create();

	int i = 0;
    double t = 0;
	//double scale = 1.3;
	bool tryflip = false;
    std::vector<cv::Rect> faces, faces2;
    const static cv::Scalar colors[] =  { CV_RGB(0,0,255),
        CV_RGB(0,128,255),
        CV_RGB(0,255,255),
        CV_RGB(0,255,0),
        CV_RGB(255,128,0),
        CV_RGB(255,255,0),
        CV_RGB(255,0,0),
        CV_RGB(255,0,255)} ;
	
    cv::Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

	
		cvtColor( img, gray, m_bHasAlpha?CV_RGBA2GRAY:CV_RGB2GRAY );
	
    cv::resize( gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR );
    cv::equalizeHist( smallImg, smallImg );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        cv::Size(30, 30) );
    if( tryflip )
    {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale( smallImg, faces2,
                                 1.1, 2, 0
                                 //|CV_HAAR_FIND_BIGGEST_OBJECT
                                 //|CV_HAAR_DO_ROUGH_SEARCH
                                 |CV_HAAR_SCALE_IMAGE
                                 ,
                                 cv::Size(30, 30) );
        for( cv::vector<cv::Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++ )
        {
            faces.push_back(cv::Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
    }
    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    for( cv::vector<cv::Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
       
		CCRect tmp;
		tmp.origin.x = r->x*scale;
		tmp.origin.y = r->y*scale;
		tmp.size.width = r->width*scale;
		tmp.size.height = r->height*scale;
		CCImgObj* obj = new CCImgObj(tmp);
		obj->autorelease();
		arr->addObject(obj);
    }

	cvReleaseImageHeader(&src);

	return arr;
}



CCImage* CCImage::createSubImage(CCRect r)
{
	CCImage* img = CCImage::create();
	img->m_nWidth = r.size.width;
	img->m_nHeight = r.size.height;
	img->m_bHasAlpha = m_bHasAlpha;
	img->m_bPreMulti = m_bPreMulti;
	img->m_nBitsPerComponent = 8;
	int bytes = m_bHasAlpha ? 4 : 3;
	unsigned char* data = new unsigned char[(int)(r.size.width)*(int)(r.size.height)*bytes];
	for(int row = r.origin.y;row < (int)(r.origin.y)+(int)(r.size.height);++row)
	{
		memcpy(data+(row-(int)r.origin.y)*(int)(r.size.width)*bytes,m_pData+(row*m_nWidth+(int)(r.origin.x))*bytes,(int)(r.size.width)*bytes);
	}
	img->m_pData = data;
	return img;
}

CCImage* CCImage::effect(int type)
{
	int bytes = m_bHasAlpha ? 4 : 3;
	IplImage* src = cvCreateImageHeader( cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U, bytes);
	cvSetData(src,m_pData,m_nWidth*bytes);
	IplImage* dst = cvCloneImage (src);

	
	

	
	switch(type)
	{
	case IMAGE_EFFECT_THRESHOLD:
{
		IplImage* tmp1 = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		IplImage *gray = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		cvCvtColor (src, gray, m_bHasAlpha?CV_RGBA2GRAY :CV_RGB2GRAY);
		cvSmooth (gray, gray, CV_GAUSSIAN, 5);
		cvThreshold (gray, tmp1, 90, 255, CV_THRESH_BINARY);
		cvCvtColor (tmp1, dst, m_bHasAlpha?CV_GRAY2RGBA:CV_GRAY2RGB);
		cvReleaseImage(&gray);
		cvReleaseImage(&tmp1);
}
	break;
	case IMAGE_EFFECT_ADAPTIVE:
{
		IplImage* tmp1 = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		IplImage *gray = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		cvCvtColor (src, gray, m_bHasAlpha?CV_RGBA2GRAY :CV_RGB2GRAY);
		cvSmooth (gray, gray, CV_GAUSSIAN, 5);
		cvAdaptiveThreshold (gray, tmp1, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, 10);
		cvCvtColor (tmp1, dst, m_bHasAlpha?CV_GRAY2RGBA:CV_GRAY2RGB);
		cvReleaseImage(&gray);
		cvReleaseImage(&tmp1);
}
	break;
	case IMAGE_EFFECT_COMPOSITE:
{
		IplImage* tmp1 = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		IplImage* tmp2 = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		IplImage* tmp3 = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		IplImage *gray = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
		cvCvtColor (src, gray, m_bHasAlpha?CV_RGBA2GRAY :CV_RGB2GRAY);
		cvSmooth (gray, gray, CV_GAUSSIAN, 5);
		cvThreshold (gray, tmp1, 90, 255, CV_THRESH_BINARY);
		cvAdaptiveThreshold (gray, tmp2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, 10);
		cvAnd (tmp1, tmp2, tmp3);
		cvCvtColor (tmp3, dst, m_bHasAlpha?CV_GRAY2RGBA:CV_GRAY2RGB);

		cvSmooth (src, src, CV_GAUSSIAN, 11);
		cvAnd (dst, src, dst);
		cvReleaseImage(&gray);
		cvReleaseImage(&tmp1);
		cvReleaseImage(&tmp2);
		cvReleaseImage(&tmp3);
}
	break;
	case IMAGE_EFFECT_DAJIN:
{
	IplImage* tmp = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
	IplImage *gray = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 1);
	cvCvtColor (src, gray, m_bHasAlpha?CV_RGBA2GRAY :CV_RGB2GRAY);
	cvSmooth (gray, gray, CV_GAUSSIAN, 5);
	cvThreshold (gray, tmp, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvCvtColor (tmp, dst, m_bHasAlpha?CV_GRAY2RGBA:CV_GRAY2RGB);
	cvReleaseImage(&gray);
	cvReleaseImage(&tmp);
}
	break;
	case IMAGE_EFFECT_CARTOON:
		{
			//IplImage* tmp = cvCreateImage (cvGetSize (src), IPL_DEPTH_8U, 3);
			//cv::Mat bgr = cv::cvarrToMat(tmp);
			cv::Mat bgr,gray,edges,edgesBgr;
			//cvCvtColor(src, tmp, m_bHasAlpha?CV_RGBA2BGR:CV_RGB2BGR);
			cv::cvtColor(cv::cvarrToMat(src),bgr,m_bHasAlpha?CV_RGBA2BGR:CV_RGB2BGR);
			cv::pyrMeanShiftFiltering(bgr.clone(), bgr, 15, 40);

			cv::cvtColor(bgr, gray,CV_BGR2GRAY);
			cv::Canny(gray, edges, 150, 150);
  
		    cv::cvtColor(edges, edgesBgr, CV_GRAY2BGR);
  
			bgr = bgr - edgesBgr;
  
			cv::cvtColor(bgr, cv::cvarrToMat(dst), m_bHasAlpha?CV_BGR2RGBA:CV_BGR2RGB);
		}
		break;
	}

	CCImage* out = CCImage::create();
	out->m_nWidth = m_nWidth;
	out->m_nHeight = m_nHeight;
	out->m_bHasAlpha = m_bHasAlpha;
	out->m_bPreMulti = m_bPreMulti;
	out->m_nBitsPerComponent = m_nBitsPerComponent;
	int length = m_nWidth*m_nHeight*bytes;
	out->m_pData = new unsigned char[length];
	unsigned char* pData = NULL;
	cvGetRawData(dst,&pData);
	memcpy(out->m_pData,pData,length); 
	cvReleaseImageHeader(&src);
	cvReleaseImage(&dst);
	return out;
}

CCImage* CCImage::erase(CCImage* maskimg)
{
	int bytes = m_bHasAlpha ? 4 : 3;
	IplImage* src = cvCreateImageHeader( cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U, bytes);
	cvSetData(src,m_pData,m_nWidth*bytes);

	int mbytes = maskimg->m_bHasAlpha ? 4:3;
	IplImage* mask = cvCreateImageHeader( cvSize(maskimg->m_nWidth,maskimg->m_nHeight), IPL_DEPTH_8U, mbytes);
	cvSetData(mask,maskimg->m_pData,maskimg->m_nWidth*mbytes);

	IplImage *gray = cvCreateImage (cvGetSize (mask), IPL_DEPTH_8U, 1);
	cvCvtColor (mask, gray, m_bHasAlpha?CV_RGBA2GRAY :CV_RGB2GRAY);

	IplImage* dst = cvCloneImage (src);
	
	cv::inpaint (cv::cvarrToMat(src), cv::cvarrToMat(mask), cv::cvarrToMat(dst), 10.0, CV_INPAINT_NS);

	CCImage* out = CCImage::create();
	out->m_nWidth = m_nWidth;
	out->m_nHeight = m_nHeight;
	out->m_bHasAlpha = m_bHasAlpha;
	out->m_bPreMulti = m_bPreMulti;
	out->m_nBitsPerComponent = m_nBitsPerComponent;
	int length = m_nWidth*m_nHeight*bytes;
	out->m_pData = new unsigned char[length];
	unsigned char* pData = NULL;
	cvGetRawData(dst,&pData);
	memcpy(out->m_pData,pData,length); 
	cvReleaseImageHeader(&src);
	cvReleaseImageHeader(&mask);
	cvReleaseImage(&dst);

	return out;
}

CCImage* CCImage::mask(CCImage* maskimg)
{
	//if(!m_bHasAlpha)
	{
	//	return NULL;
	}
	int idx = 0,midx=0;
	int bytes = m_bHasAlpha ? 4 : 3;
	int mbytes = maskimg->m_bHasAlpha ? 4 : 3;

	CCImage* out = CCImage::create();
	out->m_nWidth = m_nWidth;
	out->m_nHeight = m_nHeight;
	out->m_bHasAlpha = true;
	out->m_bPreMulti = m_bPreMulti;
	out->m_nBitsPerComponent = m_nBitsPerComponent;
	int length = m_nWidth*m_nHeight*4;
	out->m_pData = new unsigned char[length];
	if(m_bHasAlpha)
	{
		memcpy(out->m_pData,m_pData,length);
	}
	else
	{
		for(int row=0;row<m_nHeight;++row)
		{
			for(int col=0;col<m_nWidth;++col)
			{
					idx = (row*m_nHeight+col)*3;
					midx = (row*m_nHeight+col)*4;
					memcpy(out->m_pData+midx,m_pData+idx,sizeof(unsigned char)*3);
					out->m_pData[midx+3] = 0xff;
			}
		}
	}
	
	for(int row=0;row < m_nHeight;++row)
	{
		for(int col=0;col<m_nWidth;++col)
		{
			idx = (row*m_nHeight+col)*4;
			if(row<maskimg->m_nHeight
			&& col<maskimg->m_nWidth)
			{
				midx=(row*maskimg->m_nHeight+col)*mbytes;
				out->m_pData[idx+3] = maskimg->m_pData[midx+3]; //a = a
			}
			else
			{
				out->m_pData[idx+0] = 0; //r
				out->m_pData[idx+1] = 0; //g
				out->m_pData[idx+2] = 0; //b
				out->m_pData[idx+3] = 0; //a
			}
		}
	}
	
	return out;
}

NS_CC_END

