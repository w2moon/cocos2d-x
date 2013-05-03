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
#include "ccUtils.h"

namespace cocos2d {

unsigned long ccNextPOT(unsigned long x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}
static CCUtil* s_util = NULL;
CCUtil* CCUtil::getInstance(){
	if(s_util == NULL)
	{
		s_util = new CCUtil();
	}
	return s_util;
}

CCUtil::CCUtil(){
}
	CCUtil::~CCUtil(){
		delete s_util;
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

typedef struct _ASTAT_
{
	    ADAPTER_STATUS adapt;
    NAME_BUFFER NameBuff [30];
} ASTAT, *PASTAT;

#pragma comment(lib,"netapi32.lib")
std::string  CCUtil::macAddress()
{
	char mac[32];
	ASTAT Adapter;
	    NCB Ncb;
	    UCHAR uRetCode;
	    LANA_ENUM lenum;
	    int i = 0;
	 
	    memset(&Ncb, 0, sizeof(Ncb));
	    Ncb.ncb_command = NCBENUM;
	    Ncb.ncb_buffer = (UCHAR *)&lenum;
	    Ncb.ncb_length = sizeof(lenum);
	 
	    uRetCode = Netbios( &Ncb );
	    CCLOG( "The NCBENUM return adapter number is: %d \n ", lenum.length);
	    for(i=0; i < lenum.length ; i++)
	    {
	        memset(&Ncb, 0, sizeof(Ncb));
	        Ncb.ncb_command = NCBRESET;
	        Ncb.ncb_lana_num = lenum.lana[i];
	        uRetCode = Netbios( &Ncb );
	 
	        memset(&Ncb, 0, sizeof(Ncb));
	        Ncb.ncb_command = NCBASTAT;
	        Ncb.ncb_lana_num = lenum.lana[i];
	        strcpy((char *)Ncb.ncb_callname, "* ");
	        Ncb.ncb_buffer = (unsigned char *) &Adapter;
	        Ncb.ncb_length = sizeof(Adapter);
	        uRetCode = Netbios( &Ncb );
	 
	        if (uRetCode == 0)
	        {
	            sprintf(mac, "%02x%02x%02x%02x%02x%02x ",
	            //sprintf(mac, "%02X%02X%02X%02X%02X%02X ",
	                    Adapter.adapt.adapter_address[0],
	                    Adapter.adapt.adapter_address[1],
	                    Adapter.adapt.adapter_address[2],
	                    Adapter.adapt.adapter_address[3],
	                    Adapter.adapt.adapter_address[4],
	                    Adapter.adapt.adapter_address[5]
	                   );
	            //printf( "The Ethernet Number on LANA %d is: %s\n ", lenum.lana[i], mac);
	        }
	    }
		return mac;
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
std::string  CCUtil::macAddress()
{
	int mib[6];
		size_t len;
		char mac[32];
		char* buf;
		unsigned char *ptr;
		struct if_msghdr *ifm;
		struct sockaddr_dl *sdl;

		mib[0] = CTL_NET;
		mib[1] = AF_ROUTE;
		mib[2] = 0;
		mib[3] = AF_LINK;
		mib[4] = NET_RT_IFLIST;

		if ((mib[5] = if_nametoindex("en0")) == 0) {
			CCLOG("Error: if_nametoindex error\n");
			return "";
		}

		if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
			CCLOG("Error:sys_id sysctl, take 1\n");
			return "";
		}

		if ((buf = malloc(len)) == NULL) {
			CCLOG("sys_id Could not allocate memory. error!\n");
			return "";
		}

		if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
			CCLOG("sys_id Error: sysctl, take 2");
			free(buf);
			return "";
		}

		ifm = (struct if_msghdr *)buf;
		sdl = (struct sockaddr_dl *)(ifm + 1);
		ptr = (unsigned char *)LLADDR(sdl);
		sprintf(mac,"%02X%02X%02X%02X%02X%02X",*ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
		free(buf);
		return mac;
}
#else
std::string  CCUtil::macAddress()
{
	return "";
}

#endif

}