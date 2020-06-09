#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "crtl/random.h"
#include "crtl/log.h"
#include "crtl/assert.h"

#include "crypto/attribute.h"

#define HAVE_DEV_RANDOM 1

static const char *dev_random_file = "/dev/urandom";


#define CRTL_RANDOM_SEED_DEV    1
#define CRTL_RANDOM_SEED_TIME    2


static int _unused __crtl_random_int_device(int seed, int min, int max)
{
    if(CRTL_RANDOM_SEED_DEV != seed)
    {
        crtl_print_err("wrong seed, need CRTL_RANDOM_SEED_DEV(%d)\n", CRTL_RANDOM_SEED_DEV);
        return -1;
    }
    if(max < min)
    {
        crtl_print_err("min must smaller than max. min(%d), max(%d)\n", min, max);
        return -1;
    }
    
    /* Store a file descriptor opened to /dev/random in a static
     * variable. That way, we don't need to open the file every time
     * this function is called. */
    static int dev_random_fd = -1;
    char* next_random_byte;
    int bytes_to_read;
    unsigned random_value;

    /* Make sure MAX is greater than MIN. */
    crtl_assert (max > min);
    
    /* If this is the first time this function is called, open a file
     * descriptor to /dev/random. */
    if (dev_random_fd == -1) {
        dev_random_fd = open ("/dev/random", O_RDONLY);
        crtl_assert (dev_random_fd != -1);
    }
    /* Read enough random bytes to fill an integer variable. */
    next_random_byte = (char*) &random_value;
    bytes_to_read = sizeof (random_value);
    /* Loop until we've read enough bytes. Because /dev/random is filled
     * from user-generated actions, the read may block and may only
     * return a single random byte at a time. */
    do {
        int bytes_read;
        bytes_read = read (dev_random_fd, next_random_byte, bytes_to_read);
        bytes_to_read -= bytes_read;
        next_random_byte += bytes_read;
    } while (bytes_to_read > 0);
    /* Compute a random number in the correct range. */
    return min + (random_value % (max - min + 1));
}


int crtl_random_int(int min, int max)
{
    return __crtl_random_int_device(CRTL_RANDOM_SEED_DEV, min, max);
}




/*
 * random_seed.c
 *
 * Copyright (c) 2013 Metaparadigm Pte. Ltd.
 * Michael Clark <michael@metaparadigm.com>
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See COPYING for details.
 *
 */
#if 0


#if defined ENABLE_RDRAND

/* cpuid */

#if defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#define HAS_X86_CPUID 1

static void do_cpuid(int regs[], int h)
{
    __asm__ __volatile__(
#if defined __x86_64__
                         "pushq %%rbx;\n"
#else
                         "pushl %%ebx;\n"
#endif
                         "cpuid;\n"
#if defined __x86_64__
                         "popq %%rbx;\n"
#else
                         "popl %%ebx;\n"
#endif
                         : "=a"(regs[0]), [ebx] "=r"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
                         : "a"(h));
}

#endif

/* has_rdrand */

#if HAS_X86_CPUID

static int has_rdrand(void)
{
    // CPUID.01H:ECX.RDRAND[bit 30] == 1
    int regs[4];
    do_cpuid(regs, 1);
    return (regs[2] & (1 << 30)) != 0;
}

#endif

/* get_rdrand_seed - GCC x86 and X64 */

#if defined __GNUC__ && (defined __i386__ || defined __x86_64__)

#define HAVE_RDRAND 1

static int get_rdrand_seed(void)
{
    int _eax;
    // rdrand eax
    __asm__ __volatile__("1: .byte 0x0F\n"
                         "   .byte 0xC7\n"
                         "   .byte 0xF0\n"
                         "   jnc 1b;\n"
                         : "=a" (_eax));
    return _eax;
}

#endif
#endif /* defined ENABLE_RDRAND */

#endif 


/* has_dev_urandom */

static int has_dev_urandom(void)
{
    struct stat buf;
    if (stat(dev_random_file, &buf)) {
        return 0;
    }
    return ((buf.st_mode & S_IFCHR) != 0);
}


/* get_dev_random_seed */

static int get_dev_random_seed(void)
{

    int fd = open(dev_random_file, O_RDONLY);
    if (fd < 0) {
        crtl_print_err("error opening %s: %s", dev_random_file, strerror(errno));
        crtl_assert(1);
    }

    int r;
    ssize_t nread = read(fd, &r, sizeof(r));
    if (nread != sizeof(r)) {
        crtl_print_err("error short read %s: %s", dev_random_file, strerror(errno));
        crtl_assert(1);
    }

    close(fd);
    return r;
}



/* get_time_seed */
static int get_time_seed(void)
{
    return (int)time(NULL) * 433494437;
}


/* json_c_get_random_seed */

int json_c_get_random_seed(void)
{
#if defined HAVE_RDRAND && HAVE_RDRAND
    if (has_rdrand()) return get_rdrand_seed();
#endif
#if defined HAVE_DEV_RANDOM && HAVE_DEV_RANDOM
    if (has_dev_urandom()) return get_dev_random_seed();
#endif
    return get_time_seed();
}

