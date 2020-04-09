#include "crtl/crtl_random.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"

#include "crtl/easy/attribute.h"


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
    assert (max > min);
    
    /* If this is the first time this function is called, open a file
     * descriptor to /dev/random. */
    if (dev_random_fd == -1) {
        dev_random_fd = open ("/dev/random", O_RDONLY);
        assert (dev_random_fd != -1);
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

