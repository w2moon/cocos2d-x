//
//  WindLayer.h
//  Wind
//
//  Created by yangjie on 3/29/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CCWINDLAYER_H
#define CCWINDLAYER_H

#include "cocos2d.h"
#include "CCStdC.h"
#include "math.h"
#include "stdio.h"
NS_CC_BEGIN


class CC_DLL  CCWindLayer : public CCLayer
{
public:
	//CCArray *			m_windList;
	std::vector< std::vector< CCPoint > > m_windList;
	int							m_windCount;
	float						m_x[10];
	float						m_y[10];
	float						m_windSetting[10][5];

	int mchu;
	int mpian;
	int mpianb;
	int mw;
	int mwb;
	int mdis;
	int mnumd;
	int mnumb;
	int mr;
	int mg;
	int mb;
	int ma;
	static CCWindLayer* create();

	CCSize screenSize;
	CCWindLayer();
	virtual ~CCWindLayer();
	void initWithWindCount(int count=3,int chu=15,int pian=30,int pianb=10,int w=5,int wb=2,int dis=0.01,int numd=60,int numb=30,int r=64,int g=32,int b=32,int a=64);
	void instanceWindSetting(int num);
	void constructWind(int num);
	void tick(float dt);

	virtual void draw();
};


NS_CC_END

#endif
