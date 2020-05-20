#include "config.h"
#include "crtl/bits/version.h"


 /*
 * Versions with the same major number are ABI stable. API is allowed to
 * evolve between minor releases, but only in a backwards compatible way.
 * Make sure you update the -soname directives in configure.ac
 * whenever you bump UV_VERSION_MAJOR or CRTL_VERSION_MINOR (but
 * not CRTL_VERSION_PATCH.)
 */
#ifdef LIBCRTL_VERSION_MAJOR
#define CRTL_VERSION_MAJOR LIBCRTL_VERSION_MAJOR
#else
#define CRTL_VERSION_MAJOR 1
#endif

#ifdef LIBCRTL_VERSION_MINOR
#define CRTL_VERSION_MINOR LIBCRTL_VERSION_MINOR
#else
#define CRTL_VERSION_MINOR 0
#endif

#ifdef LIBCRTL_VERSION_PATCH
#define CRTL_VERSION_PATCH LIBCRTL_VERSION_PATCH
#else
#define CRTL_VERSION_PATCH 0
#endif

#define CRTL_VERSION_IS_RELEASE 0

#ifdef LIBCRTL_NAME
#define CRTL_VERSION_PERFIX LIBCRTL_NAME
#else
#define CRTL_VERSION_PERFIX "libcrtl"
#endif

#ifdef LIBCRTL_AUTHOR
#define CRTL_VERSION_SUFFIX "Beta "LIBCRTL_AUTHOR
#else
#define CRTL_VERSION_SUFFIX "Beta "
#endif

#define CRTL_VERSION_HEX  ((CRTL_VERSION_MAJOR << 16) | \
                         (CRTL_VERSION_MINOR <<  8) | \
                         (CRTL_VERSION_PATCH))



#define CRTL_STRINGIFY(v) CRTL_STRINGIFY_HELPER(v)
#define CRTL_STRINGIFY_HELPER(v) #v


#define UV_VERSION_STRING_BASE  CRTL_STRINGIFY(CRTL_VERSION_MAJOR) "." \
                                CRTL_STRINGIFY(CRTL_VERSION_MINOR) "." \
                                CRTL_STRINGIFY(CRTL_VERSION_PATCH)

#if CRTL_VERSION_IS_RELEASE
# define CRTL_VERSION_STRING  CRTL_VERSION_PERFIX" "UV_VERSION_STRING_BASE
#else
# define CRTL_VERSION_STRING  CRTL_VERSION_PERFIX" "UV_VERSION_STRING_BASE "-" CRTL_VERSION_SUFFIX
#endif

/**
 * Get LIBCrtl Version Integer
 * @param void
 * @return version
 */
unsigned int crtl_version(void)
{
  return CRTL_VERSION_HEX;
}

/**
 * Get LIBCrtl Version String
 * @param void
 * @return string of version
 */
const char* crtl_version_string(void)
{
  return CRTL_VERSION_STRING;
}

/**
 * Get LIBCrtl Author String
 * @param void
 * @return Author of version
 */
const char *crtl_author(void)
{
#ifdef LIBCRTL_AUTHOR
    return LIBCRTL_AUTHOR;
#else
    return "unknown, maybe Rong Tao";
#endif
}
