
#include <crtl/bits/ctype.h>


extern const char __crtl_hex_asc[];
#define __crtl_hex_asc_lo(x)	__crtl_hex_asc[((x) & 0x0f)]
#define __crtl_hex_asc_hi(x)	__crtl_hex_asc[((x) & 0xf0) >> 4]

static inline char *__crtl_hex_byte_pack(char *buf, uint8_t byte)
{
	*buf++ = __crtl_hex_asc_hi(byte);
	*buf++ = __crtl_hex_asc_lo(byte);
	return buf;
}

extern const char hex_asc_upper[];
#define hex_asc_upper_lo(x)	hex_asc_upper[((x) & 0x0f)]
#define hex_asc_upper_hi(x)	hex_asc_upper[((x) & 0xf0) >> 4]

static inline char *hex_byte_pack_upper(char *buf, uint8_t byte)
{
	*buf++ = hex_asc_upper_hi(byte);
	*buf++ = hex_asc_upper_lo(byte);
	return buf;
}



/**
 * hex_to_bin - convert a hex digit to its real value
 * @ch: ascii character represents hex digit
 *
 * hex_to_bin() converts one hex digit to its actual value or -1 in case of bad
 * input.
 */
int crtl_hex_to_bin(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - '0';
	ch = crtl_tolower(ch);
	if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	return -1;
}



/**
 * hex2bin - convert an ascii hexadecimal string to its binary representation
 * @dst: binary result
 * @src: ascii hexadecimal string
 * @count: result length
 *
 * Return 0 on success, -EINVAL in case of bad input.
 */
int crtl_hex2bin(uint8_t *dst, const char *src, int count)
{
	while (count--) {
		int hi = crtl_hex_to_bin(*src++);
		int lo = crtl_hex_to_bin(*src++);

		if ((hi < 0) || (lo < 0))
			return -1;

		*dst++ = (hi << 4) | lo;
	}
	return 0;
}

/**
 * bin2hex - convert binary data to an ascii hexadecimal string
 * @dst: ascii hexadecimal result
 * @src: binary data
 * @count: binary data length
 */
char *crtl_bin2hex(char *dst, const void *src, int count)
{
	const unsigned char *_src = src;

	while (count--)
		dst = __crtl_hex_byte_pack(dst, *_src++);
	return dst;
}



