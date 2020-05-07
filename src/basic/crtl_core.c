#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include <unistd.h>

#include "crtl/bits/crtl_core.h"
#include "crtl/bits/crtl_types_basic.h"
#include "crtl/crtl_string.h"





int crtl_getpid(void)
{
  return getpid();
}


int crtl_getppid(void)
{
  return getppid();
}

int crtl_getpriority(int pid, int* priority)
{
  int r;

  if (priority == NULL)
    return CRTL_ERROR;

  r = getpriority(PRIO_PROCESS, (int) pid);

  if (r == -1)
    return CRTL_ERROR;

  *priority = r;
  return CRTL_SUCCESS;
}



int crtl_setpriority(int pid, int priority)
{
  if (priority < CRTL_PRIORITY_HIGHEST || priority > CRTL_PRIORITY_LOW)
    return CRTL_ERROR;

  if (setpriority(PRIO_PROCESS, (int) pid, priority) != 0)
    return CRTL_ERROR;

  return CRTL_SUCCESS;
}





int crtl_uname(crtl_utsname_t* buffer)
{
  struct utsname buf;
  int r;

  if (buffer == NULL)
    return CRTL_ERROR;

  if (uname(&buf) == -1) {
    r = CRTL_ERROR;
    goto error;
  }

  r = crtl_strscpy(buffer->sysname, buf.sysname, sizeof(buffer->sysname));
  if (r == CRTL_ERROR)
    goto error;

  r = crtl_strscpy(buffer->release, buf.release, sizeof(buffer->release));
  if (r == CRTL_ERROR)
    goto error;

  r = crtl_strscpy(buffer->version, buf.version, sizeof(buffer->version));
  if (r == CRTL_ERROR)
    goto error;

  r = crtl_strscpy(buffer->machine, buf.machine, sizeof(buffer->machine));
  if (r == CRTL_ERROR)
    goto error;

  return 0;

error:
  buffer->sysname[0] = '\0';
  buffer->release[0] = '\0';
  buffer->version[0] = '\0';
  buffer->machine[0] = '\0';
  return r;
}


//int uv_getrusage(uv_rusage_t* rusage) {
//  struct rusage usage;
//
//  if (getrusage(RUSAGE_SELF, &usage))
//    return UV__ERR(errno);
//
//  rusage->ru_utime.tv_sec = usage.ru_utime.tv_sec;
//  rusage->ru_utime.tv_usec = usage.ru_utime.tv_usec;
//
//  rusage->ru_stime.tv_sec = usage.ru_stime.tv_sec;
//  rusage->ru_stime.tv_usec = usage.ru_stime.tv_usec;
//
//#if !defined(__MVS__) && !defined(__HAIKU__)
//  rusage->ru_maxrss = usage.ru_maxrss;
//  rusage->ru_ixrss = usage.ru_ixrss;
//  rusage->ru_idrss = usage.ru_idrss;
//  rusage->ru_isrss = usage.ru_isrss;
//  rusage->ru_minflt = usage.ru_minflt;
//  rusage->ru_majflt = usage.ru_majflt;
//  rusage->ru_nswap = usage.ru_nswap;
//  rusage->ru_inblock = usage.ru_inblock;
//  rusage->ru_oublock = usage.ru_oublock;
//  rusage->ru_msgsnd = usage.ru_msgsnd;
//  rusage->ru_msgrcv = usage.ru_msgrcv;
//  rusage->ru_nsignals = usage.ru_nsignals;
//  rusage->ru_nvcsw = usage.ru_nvcsw;
//  rusage->ru_nivcsw = usage.ru_nivcsw;
//#endif
//
//  return 0;
//}


