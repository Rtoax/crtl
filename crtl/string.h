#ifndef __CRTL_STRING_H
#define __CRTL_STRING_H 1


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>



/**
 * skip the space of string
 * @param str: string address
 * @return address of next char is not space
 */
char *crtl_skip_spaces(const char *str);


/**
 * strip a charector from a string
 * @param str: string address
 * @param ch: charector ready to strip
 * @return error number
 */
int crtl_strstripc(char *str, const char ch);

/**
 * count a charector from a string
 * @param str: string address
 * @param ch: charector ready to strip
 * @return error number
 */
int crtl_strccount (char ch, char *str);

/**
 * 
 * @param 
 * @param 
 * @return 
 */
void crtl_strchop(char *s, char *t);

/**
 * copy a string to another
 * @param d: destination
 * @param s: source
 * @param n: number of char to copy
 * @return size of copy
 */
ssize_t crtl_strscpy(char* d, const char* s, size_t n);

/**
 * string to short integer
 * @param s: source string
 * @return shrot integer
 */
short crtl_estrtoh(char *s);

/**
 * string to unsigned short integer
 * @param s: source string
 * @return unsigned short integer
 */
unsigned short crtl_estrtous(char *s);

/**
 * string to integer
 * @param s: source string
 * @return integer
 */
int crtl_estrtoi(char *s);

/**
 * string to unsigned integer
 * @param s: source string
 * @return unsigned integer
 */
unsigned int crtl_estrtoui(char *s);

/**
 * string to long integer
 * @param s: source string
 * @return long integer
 */
long crtl_estrtol(char *s);

/**
 * string to unsigned long integer
 * @param s: source string
 * @return unsigned long integer
 */
unsigned long crtl_estrtoul(char *s);


/**
 * string to float
 * @param s: source string
 * @return float
 */
float crtl_estrtof(char *s);

/**
 * string to double
 * @param s: source string
 * @return double
 */
double crtl_estrtod(char *s);


/**
 * string to boolean
 * @param s: source string
 * @param res: boolean 
 * @return error number
 *
 *  if s is start with "y,Y,1", res = true
 *  if s is start with "n,N,0", res = false
 *  if s is is "On,ON,oN,on", res = true
 *  if s is is "Of,OF,oF,of"-f, res = false
 *  
 */
int crtl_strtobool(const char *s, bool *res);

/**
 * Create byte array from hexadecimal string. 
 * @param str Pointer to the string.
 * @param buf Pointer to a buffer where the data should be stored. 
 * @param buflen Length for the data buffer.
 * @return -1 if string contained illegal charactes of if the buffer did not
 * contain enough room for data.
 */
int crtl_strtobyte(const char *str, uint8_t *buf, int buflen);


/**
 * joint a string with format like printf
 * @param dst: destination of string
 * @param fmt: format of string
 * @return address of destination string
 */
char *crtl_strjoint(char *dst, const char *fmt, ...);

/**
 * joint a string with format like printf, is safe than crtl_strjoint
 * @param dst: destination of string
 * @param size: size of string
 * @param fmt: format of string
 * @return address of destination string
 */
char *crtl_strnjoint(char *dst, size_t size, const char *fmt, ...);


/**
 * show memory
 * @param in: address
 * @param len: length of memory to show
 * @return -1 if failed, length of memory to show
 */
int crtl_memshow(const void *in, int len);


/**
 * memory swap
 * @param x: input memory address1
 * @param y: input memory address2
 * @param size: size to swap
 * @return void
 */
void crtl_memswap(unsigned char *x, unsigned char *y, size_t size);

/**
 * memory copy
 * @param to: memory ready to copy to
 * @param from: memory ready to copy from
 * @param size: size to copy
 * @return void
 */
void crtl_memcopy(void *to, void *from, size_t size);

    
    
/**
 * strcasecmp, strncasecmp - compare two strings ignoring case
 * @param s1: string to compare
 * @param s2: string to compare
 * @param len: size to compare
 * @return void
 */
int crtl_strncasecmp(const char *s1, const char *s2, size_t len);
int crtl_strcasecmp(const char *s1, const char *s2);


/* string to numeric conversion with error checking */
short crtl_eatoh(char *s);
unsigned short crtl_eatou(char *s);
int crtl_eatoi(char *s);
unsigned int crtl_eatop(char *s);
long crtl_eatol(char *s);
unsigned long crtl_eatov(char *s);
float crtl_eatof(char *s);
double crtl_eatod(char *s);


/**
 * glob_match - Shell-style pattern matching, like !fnmatch(pat, str, 0)
 * @param pat: Shell-style pattern to match, e.g. "*.[ch]".
 * @param str: String to match.  The pattern must match the entire string.
 *
 * Perform shell-style glob matching, returning true (1) if the match
 * succeeds, or false (0) if it fails.  Equivalent to !fnmatch(@pat, @str, 0).
 *
 * Pattern metacharacters are ?, *, [ and \.
 * (And, inside character classes, !, - and ].)
 *
 * This is small and simple implementation intended for device blacklists
 * where a string is matched against a number of patterns.  Thus, it
 * does not preprocess the patterns.  It is non-recursive, and run-time
 * is at most quadratic: strlen(@str)*strlen(@pat).
 *
 * An example of the worst case is glob_match("*aaaaa", "aaaaaaaaaa");
 * it takes 6 passes over the pattern before matching the string.
 *
 * Like !fnmatch(@pat, @str, 0) and unlike the shell, this does NOT
 * treat / or leading . specially; it isn't actually used for pathnames.
 *
 * Note that according to glob(7) (and unlike bash), character classes
 * are complemented by a leading !; this does not support the regex-style
 * [^a-z] syntax.
 *
 * An opening bracket without a matching close is matched literally.
 */
bool crtl_glob_match(char const *pat, char const *str);


#endif /*<__CRTL_STRING_H>*/


