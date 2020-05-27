#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>

#include "crtl/bits/plugin.h"
#include "crtl/bits/types_basic.h"





static int __crtl_dlerror(crtl_dl_t* dl_handle)
{
    const char* errmsg;

    errmsg = dlerror();

    if (errmsg) {
        dl_handle->errmsg = strdup(errmsg);
        return CRTL_ERROR;
    } else {
        dl_handle->errmsg = NULL;
        return CRTL_SUCCESS;
    }
}



int crtl_dlopen(const char* filename, crtl_dl_t* lib)
{
    dlerror(); /* Reset error status. */
    lib->errmsg = NULL;
    lib->handle = dlopen(filename, RTLD_LAZY);
    return lib->handle ? CRTL_SUCCESS : __crtl_dlerror(lib);
}


void crtl_dlclose(crtl_dl_t* lib)
{
    free(lib->errmsg);
    lib->errmsg = NULL;

    if (lib->handle) {
        /* Ignore errors. No good way to signal them without leaking memory. */
        dlclose(lib->handle);
        lib->handle = NULL;
    }
}


int crtl_dlsym(crtl_dl_t* lib, const char* name, void** ptr)
{
    dlerror(); /* Reset error status. */
    *ptr = dlsym(lib->handle, name);
    return __crtl_dlerror(lib);
}


const char* crtl_dlerror(const crtl_dl_t* lib)
{
    return lib->errmsg ? lib->errmsg : "no error";
}




