#include <stdio.h>
#include <stdlib.h>
#include <net/if.h>
#include <malloc.h>
#include <string.h>

#include <arpa/inet.h> //inet_xxxx

#include <netinet/in.h>
#include <net/if.h>

#include "crypto/endian.h"
#include "crtl/bits/byteswap.h"
#include "crypto/attribute.h"
#include "crypto/once.h"
#include "crypto/type/check.h"

#include "crtl/log.h"
#include "crtl/string.h"

#include "crtl/bits/types_basic.h"
#include "crtl/network/inet.h"

#include "crypto/align/array.h"


/**
 *	Show all if_nameindex in terminal
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_showallnameindexs(void)
{
	struct if_nameindex *head, *ifni;
	ifni = if_nameindex();
    head = ifni;

	if (head == NULL) 
	{
        perror("t_if_getallnameindexs()");
        return CRTL_ERROR;
    }

	while (ifni->if_index != 0) 
	{
        printf("Interface %d : %s\n", ifni->if_index, ifni->if_name);
        ifni++;
    } 
	
	if_freenameindex(head);
    head = NULL;
    ifni = NULL;
	return CRTL_SUCCESS;
}

/**
 *	is if_index exist
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_indexexist(unsigned int if_index)
{
	char *if_name = (char*)malloc(IFNAMSIZ*sizeof(char));
	char *name = if_indextoname(if_index, if_name);
	
    if (name == NULL) 
	{
        perror("if_indextoname()");
		free(if_name);
        return CRTL_ERROR;
    }
	else
	{
		crtl_print_err("\033[31m%d:%s\033[0m\n", if_index, if_name);
		free(if_name);
		return CRTL_SUCCESS;
	}
}


/**
 *	is if_name exist
 *	Author: Rong Tao
 *	Time:	2018.11.14
 */
int crtl_if_nameexist(const char *if_name)
{
	if(if_name == NULL)
	{
		return CRTL_ERROR;
	}

	unsigned int if_index;
	struct if_nameindex *head, *ifni;
	ifni = if_nameindex();
    head = ifni;

	if (head == NULL) 
	{
        perror("t_if_nameexist()");
        return CRTL_ERROR;
    }

	while (ifni->if_index != 0) 
	{
        if(strcmp(if_name, ifni->if_name) == 0)
    	{
    		if_index = ifni->if_index;
			if_freenameindex(head);
		    head = NULL;
		    ifni = NULL;
			return if_index;
    	}
        ifni++;
    } 
	
	if_freenameindex(head);
    head = NULL;
    ifni = NULL;
	return CRTL_ERROR;
}


#define UV__INET_ADDRSTRLEN         16
#define UV__INET6_ADDRSTRLEN        46



int crtl_inet_ntop4(const unsigned char *src, char *dst, size_t size) 
{
  static const char fmt[] = "%u.%u.%u.%u";
  char tmp[UV__INET_ADDRSTRLEN];
  int l;

  l = snprintf(tmp, sizeof(tmp), fmt, src[0], src[1], src[2], src[3]);
  if (l <= 0 || (size_t) l >= size) {
    return CRTL_ERROR;
  }
  crtl_strscpy(dst, tmp, size);
  return CRTL_SUCCESS;
}


int crtl_inet_ntop6(const unsigned char *src, char *dst, size_t size)
{
  /*
   * Note that int32_t and int16_t need only be "at least" large enough
   * to contain a value of the specified size.  On some systems, like
   * Crays, there is no such thing as an integer variable with 16 bits.
   * Keep this in mind if you think this function should have been coded
   * to use pointer overlays.  All the world's not a VAX.
   */
  char tmp[UV__INET6_ADDRSTRLEN], *tp;
  struct { int base, len; } best, cur;
  unsigned int words[sizeof(struct in6_addr) / sizeof(uint16_t)];
  int i;

  /*
   * Preprocess:
   *  Copy the input (bytewise) array into a wordwise array.
   *  Find the longest run of 0x00's in src[] for :: shorthanding.
   */
  memset(words, '\0', sizeof words);
  for (i = 0; i < (int) sizeof(struct in6_addr); i++)
    words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
  best.base = -1;
  best.len = 0;
  cur.base = -1;
  cur.len = 0;
  for (i = 0; i < (int) ARRAY_SIZE(words); i++) {
    if (words[i] == 0) {
      if (cur.base == -1)
        cur.base = i, cur.len = 1;
      else
        cur.len++;
    } else {
      if (cur.base != -1) {
        if (best.base == -1 || cur.len > best.len)
          best = cur;
        cur.base = -1;
      }
    }
  }
  if (cur.base != -1) {
    if (best.base == -1 || cur.len > best.len)
      best = cur;
  }
  if (best.base != -1 && best.len < 2)
    best.base = -1;

  /*
   * Format the result.
   */
  tp = tmp;
  for (i = 0; i < (int) ARRAY_SIZE(words); i++) {
    /* Are we inside the best run of 0x00's? */
    if (best.base != -1 && i >= best.base &&
        i < (best.base + best.len)) {
      if (i == best.base)
        *tp++ = ':';
      continue;
    }
    /* Are we following an initial run of 0x00s or any real hex? */
    if (i != 0)
      *tp++ = ':';
    /* Is this address an encapsulated IPv4? */
    if (i == 6 && best.base == 0 && (best.len == 6 ||
        (best.len == 7 && words[7] != 0x0001) ||
        (best.len == 5 && words[5] == 0xffff))) {
      int err = crtl_inet_ntop4(src+12, tp, sizeof tmp - (tp - tmp));
      if (err)
        return err;
      tp += strlen(tp);
      break;
    }
    tp += sprintf(tp, "%x", words[i]);
  }
  /* Was it a trailing run of 0x00's? */
  if (best.base != -1 && (best.base + best.len) == ARRAY_SIZE(words))
    *tp++ = ':';
  *tp++ = '\0';
  if (CRTL_ERROR == crtl_strscpy(dst, tmp, size))
    return CRTL_ERROR;
  return CRTL_SUCCESS;
}



int crtl_inet_pton4(const char *src, unsigned char *dst)
{
  static const char digits[] = "0123456789";
  int saw_digit, octets, ch;
  unsigned char tmp[sizeof(struct in_addr)], *tp;

  saw_digit = 0;
  octets = 0;
  *(tp = tmp) = 0;
  while ((ch = *src++) != '\0') {
    const char *pch;

    if ((pch = strchr(digits, ch)) != NULL) {
      unsigned int nw = *tp * 10 + (pch - digits);

      if (saw_digit && *tp == 0)
        return CRTL_ERROR;
      if (nw > 255)
        return CRTL_ERROR;
      *tp = nw;
      if (!saw_digit) {
        if (++octets > 4)
          return CRTL_ERROR;
        saw_digit = 1;
      }
    } else if (ch == '.' && saw_digit) {
      if (octets == 4)
        return CRTL_ERROR;
      *++tp = 0;
      saw_digit = 0;
    } else
      return CRTL_ERROR;
  }
  if (octets < 4)
    return CRTL_ERROR;
  memcpy(dst, tmp, sizeof(struct in_addr));
  return CRTL_SUCCESS;
}


int crtl_inet_pton6(const char *src, unsigned char *dst)
{
  static const char xdigits_l[] = "0123456789abcdef",
                    xdigits_u[] = "0123456789ABCDEF";
  unsigned char tmp[sizeof(struct in6_addr)], *tp, *endp, *colonp;
  const char *xdigits, *curtok;
  int ch, seen_xdigits;
  unsigned int val;

  memset((tp = tmp), '\0', sizeof tmp);
  endp = tp + sizeof tmp;
  colonp = NULL;
  /* Leading :: requires some special handling. */
  if (*src == ':')
    if (*++src != ':')
      return CRTL_ERROR;
  curtok = src;
  seen_xdigits = 0;
  val = 0;
  while ((ch = *src++) != '\0') {
    const char *pch;

    if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
      pch = strchr((xdigits = xdigits_u), ch);
    if (pch != NULL) {
      val <<= 4;
      val |= (pch - xdigits);
      if (++seen_xdigits > 4)
        return CRTL_ERROR;
      continue;
    }
    if (ch == ':') {
      curtok = src;
      if (!seen_xdigits) {
        if (colonp)
          return CRTL_ERROR;
        colonp = tp;
        continue;
      } else if (*src == '\0') {
        return CRTL_ERROR;
      }
      if (tp + sizeof(uint16_t) > endp)
        return CRTL_ERROR;
      *tp++ = (unsigned char) (val >> 8) & 0xff;
      *tp++ = (unsigned char) val & 0xff;
      seen_xdigits = 0;
      val = 0;
      continue;
    }
    if (ch == '.' && ((tp + sizeof(struct in_addr)) <= endp)) {
      int err = crtl_inet_pton4(curtok, tp);
      if (err == 0) {
        tp += sizeof(struct in_addr);
        seen_xdigits = 0;
        break;  /*%< '\\0' was seen by inet_pton4(). */
      }
    }
    return CRTL_ERROR;
  }
  if (seen_xdigits) {
    if (tp + sizeof(uint16_t) > endp)
      return CRTL_ERROR;
    *tp++ = (unsigned char) (val >> 8) & 0xff;
    *tp++ = (unsigned char) val & 0xff;
  }
  if (colonp != NULL) {
    /*
     * Since some memmove()'s erroneously fail to handle
     * overlapping regions, we'll do the shift by hand.
     */
    const int n = tp - colonp;
    int i;

    if (tp == endp)
      return CRTL_ERROR;
    for (i = 1; i <= n; i++) {
      endp[- i] = colonp[n - i];
      colonp[n - i] = 0;
    }
    tp = endp;
  }
  if (tp != endp)
    return CRTL_ERROR;
  memcpy(dst, tmp, sizeof tmp);
  return CRTL_SUCCESS;
}


int crtl_inet_ntop(int af, const void* src, char* dst, size_t size)
{
  switch (af) {
  case AF_INET:
    return (crtl_inet_ntop4(src, dst, size));
  case AF_INET6:
    return (crtl_inet_ntop6(src, dst, size));
  default:
    return CRTL_ERROR;
  }
  /* NOTREACHED */
}




int crtl_inet_pton(int af, const char* src, void* dst)
{
  if (src == NULL || dst == NULL)
    return CRTL_ERROR;

  switch (af) {
  case AF_INET:
    return (crtl_inet_pton4(src, dst));
  case AF_INET6: {
    int len;
    char tmp[UV__INET6_ADDRSTRLEN], *s, *p;
    s = (char*) src;
    p = strchr(src, '%');
    if (p != NULL) {
      s = tmp;
      len = p - src;
      if (len > UV__INET6_ADDRSTRLEN-1)
        return CRTL_ERROR;
      memcpy(s, src, len);
      s[len] = '\0';
    }
    return crtl_inet_pton6(s, dst);
  }
  default:
    return CRTL_ERROR;
  }
  /* NOTREACHED */
}

