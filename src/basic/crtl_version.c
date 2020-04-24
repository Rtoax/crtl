#include "crtl/bits/crtl_version.h"


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
