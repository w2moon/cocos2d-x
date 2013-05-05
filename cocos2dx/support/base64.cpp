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

#include <stdio.h>
#include <stdlib.h>
#include "base64.h"

namespace cocos2d {

unsigned char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int _base64Decode(const unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len );

int _base64Decode(const unsigned char *input, unsigned int input_len, unsigned char *output, unsigned int *output_len )
{
    static char inalphabet[256], decoder[256];
    int i, bits, c = 0, char_count, errors = 0;
    unsigned int input_idx = 0;
    unsigned int output_idx = 0;

    for (i = (sizeof alphabet) - 1; i >= 0 ; i--) {
        inalphabet[alphabet[i]] = 1;
        decoder[alphabet[i]] = i;
    }

    char_count = 0;
    bits = 0;
    for( input_idx=0; input_idx < input_len ; input_idx++ ) {
        c = input[ input_idx ];
        if (c == '=')
            break;
        if (c > 255 || ! inalphabet[c])
            continue;
        bits += decoder[c];
        char_count++;
        if (char_count == 4) {
            output[ output_idx++ ] = (bits >> 16);
            output[ output_idx++ ] = ((bits >> 8) & 0xff);
            output[ output_idx++ ] = ( bits & 0xff);
            bits = 0;
            char_count = 0;
        } else {
            bits <<= 6;
        }
    }
    
    if( c == '=' ) {
        switch (char_count) {
            case 1:
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
                std::fprintf(stderr, "base64Decode: encoding incomplete: at least 2 bits missing");
#endif
                errors++;
                break;
            case 2:
                output[ output_idx++ ] = ( bits >> 10 );
                break;
            case 3:
                output[ output_idx++ ] = ( bits >> 16 );
                output[ output_idx++ ] = (( bits >> 8 ) & 0xff);
                break;
            }
    } else if ( input_idx < input_len ) {
        if (char_count) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
            std::fprintf(stderr, "base64 encoding incomplete: at least %d bits truncated",
                    ((4 - char_count) * 6));
#endif
            errors++;
        }
    }
    
    *output_len = output_idx;
    return errors;
}



int base64Decode(const unsigned char *in, unsigned int inLength, unsigned char **out)
{
    unsigned int outLength = 0;
    
    //should be enough to store 6-bit buffers in 8-bit buffers
    *out = new unsigned char[(size_t)(inLength * 3.0f / 4.0f + 1)];
    if( *out ) {
        int ret = _base64Decode(in, inLength, *out, &outLength);
        
        if (ret > 0 )
        {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
            printf("Base64Utils: error decoding");
#endif
            delete [] *out;
            *out = NULL;            
            outLength = 0;
        }
    }
    return outLength;
}

static const char sm_base64digits[65] =
	   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char sm_base64val[128] = {
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1, 62, -1,-1,-1, 63,
		 52, 53, 54, 55,  56, 57, 58, 59,  60, 61,-1,-1, -1,-1,-1,-1,
		-1,  0,  1,  2,   3,  4,  5,  6,   7,  8,  9, 10,  11, 12, 13, 14,
		 15, 16, 17, 18,  19, 20, 21, 22,  23, 24, 25,-1, -1,-1,-1,-1,
		-1, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 36,  37, 38, 39, 40,
		 41, 42, 43, 44,  45, 46, 47, 48,  49, 50, 51,-1, -1,-1,-1,-1
	};


	void base64Encode(const unsigned char *in, int inLength,unsigned char **pout)
	{
		int size = (inLength / 3 + (inLength % 3 ? 1 : 0)) * 4 + 1;
		*pout = new unsigned char[size];
		unsigned char* out = *pout;
		for ( ; inLength >= 3; inLength -= 3, in += 3) {
			*out++ = sm_base64digits[in[0] >> 2];
			*out++ = sm_base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
			*out++ = sm_base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
			*out++ = sm_base64digits[in[2] & 0x3f];
		}
		if (inLength > 0) {
			unsigned char fragment;
	        
			*out++ = sm_base64digits[in[0] >> 2];
			fragment = (in[0] << 4) & 0x30;
			if (inLength > 1)
				fragment |= in[1] >> 4;
			*out++ = sm_base64digits[fragment];
			*out++ = (inLength < 2) ? '=' : sm_base64digits[(in[1] << 2) & 0x3c];
			*out++ = '=';
		}
		*out = '\0';
	}

}//namespace   cocos2d 
