#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>/* Obtain O_* constant definitions */
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

#include "crtl/bits/core.h"
#include "crtl/bits/types_basic.h"
#include "crtl/string.h"
#include "crtl/log.h"
#include "crtl/file.h"

#include "crypto/attribute.h"
#include "crypto/once.h"
#include "crypto/alias.h"
#include "crypto/type/check.h"


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


int crtl_fcntl_sockfdnonblk(int sockfd)
{  
	int opts;
	opts = fcntl(sockfd, F_GETFL);
	
	if(opts < 0)
	{
		perror("fcntl(sock, GETFL) error");
		return CRTL_ERROR;
	}
	opts = opts|O_NONBLOCK;
	if(fcntl(sockfd, F_SETFL, opts) < 0)
	{
		perror("fcntl(sock, SETFL, opts)");
		return CRTL_ERROR;
	}
    return CRTL_SUCCESS;
}

int crtl_fcntl_nonblk(int fd, int set)
{
  int flags;
  int r;

  do
    r = fcntl(fd, F_GETFL);
  while (r == -1);

  if (r == -1)
    return CRTL_ERROR;

  /* Bail out now if already set/clear. */
  if (!!(r & O_NONBLOCK) == !!set)
    return 0;

  if (set)
    flags = r | O_NONBLOCK;
  else
    flags = r & ~O_NONBLOCK;

  do
    r = fcntl(fd, F_SETFL, flags);
  while (r == -1);

  if (r)
    return CRTL_ERROR;

  return CRTL_SUCCESS;
}


int crtl_fcntl_cloexec(int fd, int set)
{
  int flags;
  int r;

  do
    r = fcntl(fd, F_GETFD);
  while (r == -1);

  if (r == -1)
    return CRTL_ERROR;

  /* Bail out now if already set/clear. */
  if (!!(r & FD_CLOEXEC) == !!set)
    return 0;

  if (set)
    flags = r | FD_CLOEXEC;
  else
    flags = r & ~FD_CLOEXEC;

  do
    r = fcntl(fd, F_SETFD, flags);
  while (r == -1);

  if (r)
    return CRTL_ERROR;

  return CRTL_SUCCESS;
}


int crtl_ioctl_nonblk(int fd, int set)
{
  int r;
  /* If ioctl(FIONBIO) reports ENOTTY, try fcntl(F_GETFL) + fcntl(F_SETFL).
   * Workaround for e.g. kqueue fds not supporting ioctls.
   */
  do
    r = ioctl(fd, FIONBIO, &set);
  while (r == -1);

  if (r)
    return CRTL_ERROR;

  return CRTL_SUCCESS;
}

int crtl_ioctl_cloexec(int fd, int set)
{
  int r;

  do
    r = ioctl(fd, set ? FIOCLEX : FIONCLEX);
  while (r == -1);

  if (r)
    return CRTL_ERROR;

  return CRTL_SUCCESS;
}


#if defined(_POSIX_PATH_MAX)
# define CRTL_PATH_MAX _POSIX_PATH_MAX
#elif defined(PATH_MAX)
# define CRTL_PATH_MAX PATH_MAX
#else
# define CRTL_PATH_MAX 8192
#endif


int crtl_cwd(char* buffer, size_t* size)
{
  char scratch[1 + CRTL_PATH_MAX];

  if (buffer == NULL || size == NULL)
    return CRTL_ERROR;

  /* Try to read directly into the user's buffer first... */
  if (getcwd(buffer, *size) != NULL) {
//    crtl_print_warning("getcwd(buffer, *size) error\n");
    goto fixup;
  }

  if (errno != ERANGE)
    return CRTL_ERROR;

  /* ...or into scratch space if the user's buffer is too small
   * so we can report how much space to provide on the next try.
   */
  if (getcwd(scratch, sizeof(scratch)) == NULL)
    return CRTL_ERROR;

  buffer = scratch;

fixup:

  *size = strlen(buffer);

  if (*size > 1 && buffer[*size - 1] == '/') {
    *size -= 1;
    buffer[*size] = '\0';
  }

  if (buffer == scratch) {
    *size += 1;
    return CRTL_ERROR;
  }

  return CRTL_SUCCESS;
}
weak_alias(crtl_cwd, crtl_current_dir);




int crtl_getrusage(crtl_rusage_t* rusage)
{
  struct rusage usage;

  if (getrusage(RUSAGE_SELF, &usage))
    return CRTL_ERROR;

  rusage->ru_utime.tv_sec = usage.ru_utime.tv_sec;
  rusage->ru_utime.tv_usec = usage.ru_utime.tv_usec;

  rusage->ru_stime.tv_sec = usage.ru_stime.tv_sec;
  rusage->ru_stime.tv_usec = usage.ru_stime.tv_usec;

#if !defined(__MVS__) && !defined(__HAIKU__)
  rusage->ru_maxrss = usage.ru_maxrss;
  rusage->ru_ixrss = usage.ru_ixrss;
  rusage->ru_idrss = usage.ru_idrss;
  rusage->ru_isrss = usage.ru_isrss;
  rusage->ru_minflt = usage.ru_minflt;
  rusage->ru_majflt = usage.ru_majflt;
  rusage->ru_nswap = usage.ru_nswap;
  rusage->ru_inblock = usage.ru_inblock;
  rusage->ru_oublock = usage.ru_oublock;
  rusage->ru_msgsnd = usage.ru_msgsnd;
  rusage->ru_msgrcv = usage.ru_msgrcv;
  rusage->ru_nsignals = usage.ru_nsignals;
  rusage->ru_nvcsw = usage.ru_nvcsw;
  rusage->ru_nivcsw = usage.ru_nivcsw;
#endif

  return CRTL_SUCCESS;
}


int crtl_tmpdir(char* buffer, size_t* size)
{
  const char* buf;
  size_t len;

  if (buffer == NULL || size == NULL || *size == 0)
    return CRTL_ERROR;

#define CHECK_ENV_VAR(name)                                                   \
  do {                                                                        \
    buf = getenv(name);                                                       \
    if (buf != NULL)                                                          \
      goto return_buffer;                                                     \
  }                                                                           \
  while (0)

  /* Check the TMPDIR, TMP, TEMP, and TEMPDIR environment variables in order */
  CHECK_ENV_VAR("TMPDIR");
  CHECK_ENV_VAR("TMP");
  CHECK_ENV_VAR("TEMP");
  CHECK_ENV_VAR("TEMPDIR");

#undef CHECK_ENV_VAR

  /* No temp environment variables defined */
    buf = "/tmp";

return_buffer:
  len = strlen(buf);

  if (len >= *size) {
    *size = len + 1;
    return CRTL_ERROR;
  }

  /* The returned directory should not have a trailing slash. */
  if (len > 1 && buf[len - 1] == '/') {
    len--;
  }

  memcpy(buffer, buf, len + 1);
  buffer[len] = '\0';
  *size = len;

  return CRTL_SUCCESS;
}



ssize_t crtl_recvmsg(int fd, struct msghdr* msg, int flags)
{
  struct cmsghdr* cmsg;
  ssize_t rc;
  int* pfd;
  int* end;

  static int no_msg_cmsg_cloexec;
  if (no_msg_cmsg_cloexec == 0) {
    rc = recvmsg(fd, msg, flags | 0x40000000);  /* MSG_CMSG_CLOEXEC */
    if (rc != -1)
      return rc;
//    if (errno != EINVAL)
//      return UV__ERR(errno);
    rc = recvmsg(fd, msg, flags);
    if (rc == -1)
      return CRTL_ERROR;
    no_msg_cmsg_cloexec = 1;
  } else {
    rc = recvmsg(fd, msg, flags);
  }
  
  if (rc == -1)
    return CRTL_ERROR;
  if (msg->msg_controllen == 0)
    return rc;
  for (cmsg = CMSG_FIRSTHDR(msg); cmsg != NULL; cmsg = CMSG_NXTHDR(msg, cmsg))
    if (cmsg->cmsg_type == SCM_RIGHTS)
      for (pfd = (int*) CMSG_DATA(cmsg),
           end = (int*) ((char*) cmsg + cmsg->cmsg_len);
           pfd < end;
           pfd += 1)
        crtl_cloexec(*pfd, 1);
  return rc;
}



int crtl_open_cloexec(const char* path, int flags)
{
#if defined(O_CLOEXEC)
  int fd;

  fd = open(path, flags | O_CLOEXEC);
  if (fd == -1)
    return CRTL_ERROR;

  return fd;
#else  /* O_CLOEXEC */
  int err;
  int fd;

  fd = open(path, flags);
  if (fd == -1)
    return CRTL_ERROR;

  err = crtl_cloexec(fd, 1);
  if (err) {
    close(fd);
    return err;
  }

  return fd;
#endif  /* O_CLOEXEC */
}


int crtl_dup2_cloexec(int oldfd, int newfd)
{
  int r;

  r = dup3(oldfd, newfd, O_CLOEXEC);
  if (r == -1)
    return CRTL_ERROR;

  return r;
}


/* close() on macos has the "interesting" quirk that it fails with EINTR
 * without closing the file descriptor when a thread is in the cancel state.
 * That's why libuv calls close$NOCANCEL() instead.
 *
 * glibc on linux has a similar issue: close() is a cancellation point and
 * will unwind the thread when it's in the cancel state. Work around that
 * by making the system call directly. Musl libc is unaffected.
 */
int crtl_close_nocancel(int fd)
{
#if defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdollar-in-identifier-extension"
#if defined(__LP64__) || TARGET_OS_IPHONE
  extern int close$NOCANCEL(int);
  return close$NOCANCEL(fd);
#else
  extern int close$NOCANCEL$UNIX2003(int);
  return close$NOCANCEL$UNIX2003(fd);
#endif
#pragma GCC diagnostic pop
#elif defined(__linux__)
  return syscall(SYS_close, fd);
#else
  return close(fd);
#endif
}

