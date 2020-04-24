
#ifndef __CRTL_BITS_VERSION_H
#define __CRTL_BITS_VERSION_H

 /*
 * Versions with the same major number are ABI stable. API is allowed to
 * evolve between minor releases, but only in a backwards compatible way.
 * Make sure you update the -soname directives in configure.ac
 * whenever you bump UV_VERSION_MAJOR or CRTL_VERSION_MINOR (but
 * not CRTL_VERSION_PATCH.)
 */

#define CRTL_VERSION_MAJOR 1
#define CRTL_VERSION_MINOR 0
#define CRTL_VERSION_PATCH 0

#define CRTL_VERSION_IS_RELEASE 1

#define CRTL_VERSION_SUFFIX ""

#define CRTL_VERSION_HEX  ((CRTL_VERSION_MAJOR << 16) | \
                         (CRTL_VERSION_MINOR <<  8) | \
                         (CRTL_VERSION_PATCH))


unsigned int crtl_version(void);
const char* crtl_version_string(void);




#endif /* __CRTL_BITS_VERSION_H */

