#ifndef __CRTL_BITS_VERSION_H
#define __CRTL_BITS_VERSION_H



/**
 * Get LIBCrtl Version Integer
 * @param void
 * @return version
 */
unsigned int crtl_version(void);

/**
 * Get LIBCrtl Version String
 * @param void
 * @return string of version
 */
const char* crtl_version_string(void);

/**
 * Get LIBCrtl Author String
 * @param void
 * @return Author of version
 */
const char *crtl_author(void);



#endif /* __CRTL_BITS_VERSION_H */

