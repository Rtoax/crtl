#ifndef __CRTL_CRYPTO_SECTION_H
#define __CRTL_CRYPTO_SECTION_H 1


#ifdef __GNUC__
#define __section(StrName) __attribute__((section(StrName)))
#else 
#define __section(StrName)
#endif



#define _initfn		__section(".crtl.init.func")
#define _initdata	__section(".crtl.init.data")



#endif /*<__CRTL_CRYPTO_SECTION_H>*/
