#include "crtl/easy/attribute.h"
#include "crtl/tcp-ip/crtl_tcp_ip_checksum.h"

/* get checksum value */
inline unsigned short crtl_tcp_ip_chksum1(void *pkg, int size)
{
    int             nleft   = size;
    int             sum     = 0;
    unsigned short  *w      = pkg;
    unsigned short  answer  = 0;

    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1)
    {
        * (unsigned char *) (&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum     = (sum >> 16) + (sum & 0xffff);
    sum     += (sum >> 16);
    answer  = ~sum;
    
    return answer;
}

