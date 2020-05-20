#ifndef __CRTL_BITS_PLUGIN_H
#define __CRTL_BITS_PLUGIN_H 1


typedef struct {
    void* handle;
    char* errmsg;
} crtl_dl_t;


int crtl_dlopen(const char* filename, crtl_dl_t* lib);
void crtl_dlclose(crtl_dl_t* lib);
int crtl_dlsym(crtl_dl_t* lib, const char* name, void** ptr);
const char* crtl_dlerror(const crtl_dl_t* lib);


#endif /*<__CRTL_BITS_PLUGIN_H>*/
