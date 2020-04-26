#include "crtl/bits/crtl_version.h"


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



#define CRTL_STRINGIFY(v) CRTL_STRINGIFY_HELPER(v)
#define CRTL_STRINGIFY_HELPER(v) #v

#define UV_VERSION_STRING_BASE  CRTL_STRINGIFY(CRTL_VERSION_MAJOR) "." \
                                CRTL_STRINGIFY(CRTL_VERSION_MINOR) "." \
                                CRTL_STRINGIFY(CRTL_VERSION_PATCH)

#if CRTL_VERSION_IS_RELEASE
# define CRTL_VERSION_STRING  UV_VERSION_STRING_BASE
#else
# define CRTL_VERSION_STRING  UV_VERSION_STRING_BASE "-" CRTL_VERSION_SUFFIX
#endif


unsigned int crtl_version(void)
{
  return CRTL_VERSION_HEX;
}


const char* crtl_version_string(void)
{
  return CRTL_VERSION_STRING;
}
