#include "crtl/bits/pixel.h"
#include "crtl/log.h"

void demo_pixel_test1()
{
    int i, ipixel;
    for(i='0';i<='9';i++) {
        const unsigned char *pixel = crtl_pixel_8x8(i);
        for(ipixel=0;ipixel<8;ipixel++) {
            printf("%02x ", pixel[ipixel]);
        }
        printf("\n");
    }
}

int main()
{
    demo_pixel_test1();

    return 0;
}
