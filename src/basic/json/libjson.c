
/* dummy source file for compatibility purposes */

#if defined(HAVE_CDEFS_H)
#include <sys/cdefs.h>
#endif

#ifndef __warn_references

#if 0

#define __warn_references(sym,msg)                  \
  __asm__(".section .gnu" #sym ",\n\t.ascii \"" msg "\"\n\t.text");
adfadfadsfad adsf adf adf
#else
#define __warn_references(sym,msg)    /* nothing */
#endif

#endif

#include "crypto/json/json_object.h"

__warn_references(json_object_get, "Warning: please link against libjson-c instead of libjson");

/*        __asm__(".section .gnu.warning." __STRING(sym)  \
            " ; .ascii \"" msg "\" ; .text") */
