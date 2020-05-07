#ifndef __CRTL_BITS_CORE_H
#define __CRTL_BITS_CORE_H 1


#if defined(__PASE__)
/* On IBM i PASE, the highest process priority is -10 */
# define CRTL_PRIORITY_LOW 39            // RUNPTY(99)
# define CRTL_PRIORITY_BELOW_NORMAL 15   // RUNPTY(50)
# define CRTL_PRIORITY_NORMAL 0          // RUNPTY(20)
# define CRTL_PRIORITY_ABOVE_NORMAL -4   // RUNTY(12)
# define CRTL_PRIORITY_HIGH -7           // RUNPTY(6)
# define CRTL_PRIORITY_HIGHEST -10       // RUNPTY(1)
#else
# define CRTL_PRIORITY_LOW 19
# define CRTL_PRIORITY_BELOW_NORMAL 10
# define CRTL_PRIORITY_NORMAL 0
# define CRTL_PRIORITY_ABOVE_NORMAL -7
# define CRTL_PRIORITY_HIGH -14
# define CRTL_PRIORITY_HIGHEST -20
#endif


typedef struct crtl_utsname_struct {
  char sysname[256];
  char release[256];
  char version[256];
  char machine[256];
  /* This struct does not contain the nodename and domainname fields present in
     the utsname type. domainname is a GNU extension. Both fields are referred
     to as meaningless in the docs. */
}crtl_utsname_t;
  





int crtl_getpid(void);
int crtl_getppid(void);
int crtl_getpriority(int pid, int* priority);
int crtl_setpriority(int pid, int priority); //CRTL_PRIORITY_XXX

int crtl_uname(crtl_utsname_t* buffer);





#endif /*<__CRTL_BITS_CORE_H>*/
