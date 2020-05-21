#include "crtl/bits/byteswap.h"
#include "crypto/attribute.h"




void crtl_swapshort(short *tni2)
{
    *tni2=(((*tni2>>8)&0xff) | ((*tni2&0xff)<<8));  
}

void crtl_swapushort(unsigned short *tni2)
{
    *tni2=(((*tni2>>8)&0xff) | ((*tni2&0xff)<<8));  
}

void crtl_swapint(int *tni4)
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));  
}

void crtl_swapuint(unsigned int *tni4)
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));  
}

void crtl_swaplong(long *tni4)
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));  
}

void crtl_swapulong(unsigned long *tni4)
{
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));  
}

void crtl_swapfloat(float *tnf4)
{
    int *tni4=(int *)tnf4;
    *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));  
}

void crtl_swapdouble(double *tndd8)
{
    char *tnd8=(char *)tndd8;
    char tnc;

    tnc= *tnd8;
    *tnd8= *(tnd8+7);
    *(tnd8+7)=tnc;

    tnc= *(tnd8+1);
    *(tnd8+1)= *(tnd8+6);
    *(tnd8+6)=tnc;

    tnc= *(tnd8+2);
    *(tnd8+2)= *(tnd8+5);
    *(tnd8+5)=tnc;

    tnc= *(tnd8+3);
    *(tnd8+3)= *(tnd8+4);
    *(tnd8+4)=tnc;
}

