//
//  WindLayer.m
//  Wind
//
//  Created by yangjie on 3/29/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "effects/WindLayer.h"

NS_CC_BEGIN
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

//#define screenSize CCDirector::sharedDirector()->getWinSize()

#define arc4random() ((int)(CCRANDOM_0_1()*1000000)) //((int)(CCRANDOM_0_1()*0x100000000))

static inline float InvAbs(float x) { return (x < 0) ? -x : x; }
static inline float InvSinf(float x)
{
	// fast sin function; maximum error is 0.001
	const float P = 0.225;
	x = x * M_PI;
	int k = (int)(x+0.5);
	x = x - k;
	float y = (4 - 4 * InvAbs(x)) * x;
	y = P * (y * InvAbs(y) - y) + y;
	return (k & 1)?-y:y;
}

static inline float InvSqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int *)&x; // get bits for floating value
	i = 0x5f3759df - (i >> 1); // gives initial guess y0 (what's the fuck!?)
	x = *(float *)&i; // convert bits back to float
	x = x*(1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
	return 1/x;
}


CCWindLayer* CCWindLayer::create()
{
	CCWindLayer * pRet = new CCWindLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
	return pRet;
}

	CCWindLayer::CCWindLayer()
{
	//isRelativeAnchorPoint = true;
	screenSize = CCDirector::sharedDirector()->getWinSize();
		setAnchorPoint(ccp(0,0));
}

CCWindLayer::~CCWindLayer()
{

}

void CCWindLayer::instanceWindSetting(int num)
{
	m_windSetting[num][0] = arc4random() % mchu; // 初相，反映在坐标系上就是整体的左移或者右移(+-)
	m_windSetting[num][1] = arc4random() % mpian + mpianb; // 偏距，反映在坐标系上就是整体的上移或下移
	m_windSetting[num][2] = arc4random() % mw + mwb; // 波幅，反映再坐标系上就是波的高度 arc4random() % 50 + 30;
	m_windSetting[num][3] = mdis; // 波距，反映在坐标系上就是波峰之间的距离
	m_windSetting[num][4] = arc4random() % mnumd + mnumb; // 组成风的粒子数量 arc4random() % 60 + 30;

}


void CCWindLayer::initWithWindCount(int count,int chu,int pian,int pianb,int w,int wb,float dis,int numd,int numb,int r,int g,int b,int a)
{
	m_windCount = count;
	mchu=chu;
	mpian=pian;
	mpianb=pianb;
	mw=w;
	mwb=wb;
	mdis=dis;
	mnumd=numd;
	mnumb=numb;
	mr=r;
	mg=g;
	mb=b;
	ma=a;
	for (int i = 0; i < count; i++)
	{
		std::vector<CCPoint> tmp;
		m_windList.push_back( tmp  );
		//随机初始化风的参数
		instanceWindSetting(i);



		m_x[i] = -screenSize.width - (arc4random() % 100 + 50);
	}

	this->schedule(schedule_selector(CCWindLayer::tick));
}



void CCWindLayer::tick(float dt)
{
	for (int i = 0; i < m_windCount; i++)
	{
		constructWind(i);
	}
}

void CCWindLayer::constructWind(int num)
{
	//CGPoint start = ccp(m_x[num], m_y[num]);
	std::vector< CCPoint >& pointList = m_windList[num];
	// 实时循环生成风点
	if (m_x[num] > screenSize.width + 100)
	{
		m_x[num] = -screenSize.width - (arc4random() % 100 + 50);
		pointList.clear();
		instanceWindSetting(num);
		// 重置风的设置
	}
	m_x[num] += 5;//random() % 6 + 3;
	m_y[num] = m_windSetting[num][2] * InvSinf(m_windSetting[num][3] * m_x[num] + m_windSetting[num][0]) + m_windSetting[num][1];
	//CGPoint end = ccp(m_x[num], m_y[num]);
	// 处理如果两点之间的距离过大那么自动填充
	//CGFloat distance = (int)ccpDistance(start, end);
	//	if ( distance > 1 ) {
	//		float difx = end.x - start.x;
	//		float dify = end.y - start.y;
	//		float delta;
	//		CGPoint p;
	//		for (int i = 0; i < distance; i++) {
	//			delta = (float)(i / distance);
	//			p.x = start.x + (difx * delta);
	//			p.y = start.y + (dify * delta);
	//			[pointList addObject:NSStringFromCGPoint(p)];
	//		}
	//	}
	pointList.push_back(ccp(m_x[num], m_y[num]));

	//为了使线段不过长
	while (pointList.size() > 0 && pointList.size() > m_windSetting[num][4]) {
		pointList.erase(pointList.begin());
	}
}



void CCWindLayer::draw()
{
	glClearColor(0, 0, 0, 255);
	float linewidth = 1.0f;
	//CGFloat linewidthinc = 0.10f;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	for (int j = 0; j < m_windCount; j++) {
		std::vector< CCPoint >& windArray = m_windList[j];
		int pointlistsize = windArray.size();

		//上一个点位置
		CCPoint pointper = ccp(0, 0);
		float lineWidthAddValue = linewidth*20/(pointlistsize);
		float drawPersent = pointlistsize * 0.3f;

		for (int i = pointlistsize - 1;i > 0;i--) {
			//NSLog(@"point:%@", [m_pointList objectAtIndex:i]);
			CCPoint pt = windArray[i];
			//NSLog(@"point:%f -- %f", pt.x, pt.y);

			//控制线段的粗细，使达到两头细中间粗的效果
			if (i > pointlistsize - drawPersent) {
				linewidth += lineWidthAddValue;
			} else if (i < drawPersent) {
				linewidth -= lineWidthAddValue;
			}

			//红晕效果
			glColor4ub(mr,mg,mb,ma);

			//白色
			//glColor4ub(128,128,128,32 );

			//颜色效果
			//glColor4ub(64,64,64,32 );
			//int ccr=random()%255;
			//int ccg=random()%255;
			//int ccb=random()%255;
			//int cca=random()%255;
			//glColor4ub(ccr,ccr,ccr,32 );

			//if (pointper.x > 1 && pointper.y > 1 )
			//{
			//画线段，也可以使用点
			if (!(pointper.x == 0 && pointper.y == 0)) {
				glLineWidth( linewidth );
				ccDrawLine(pointper, pt);
			}
			//glColor4ub(255,255,255,10 + 10* initlw );

			//使用点，发现效果更好一点,性能也更差一点⋯⋯
			//glPointSize(linewidth);
			//ccDrawPoint(pt);
			//}
			pointper = pt;
		}
	}
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
}



NS_CC_END
