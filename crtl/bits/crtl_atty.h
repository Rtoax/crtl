#ifndef __CRTL_BITS_ATTY_H
#define __CRTL_BITS_ATTY_H 1

#include <termio.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "crtl/bits/crtl_lock_semaphore.h"
#include "crtl/bits/crtl_lock_mutex.h"
#include "crtl/bits/crtl_llist.h"


/* macros */
#define __CRTL_ATTY_BUFFER_PAGE	((((1UL<<12) - sizeof(struct atty_buffer)) / 2) & ~0xFF)
/* tty magic number */
#define __CRTL_ATTY_MAGIC        0x54f1

#define __CRTL_N_ATTY_BUF_SIZE   4096

#define __CRTL_ATTY_NAME_LEN     64

/*
 * When a break, frame error, or parity error happens, these codes are
 * stuffed into the flags buffer.
 */
#define __CRTL_ATTY_NORMAL	0
#define __CRTL_ATTY_BREAK	1
#define __CRTL_ATTY_FRAME	2
#define __CRTL_ATTY_PARITY	3 //parity
#define __CRTL_ATTY_OVERRUN	4


/* tty_buffer: linux-2.26 linux/tty.h */
struct crtl_atty_buffer {
    union {
		struct crtl_atty_buffer *next;
		struct llist_node free;
	};
    char                *char_buf_ptr;
    unsigned char       *flag_buf_ptr;
    int                 used;
    int                 size;
    int                 commit;
    int                 read;
	int                 flags;
    /* Data points here */
    unsigned long       data[0];
};


/* atty_bufhead: linux-2.26 linux/tty.h */
struct crtl_atty_bufhead {
	crtl_lock_sem_t            lock;
    int  priority;
	struct crtl_atty_buffer *head;	/* Queue head */
	struct crtl_atty_buffer sentinel;	
	struct crtl_atty_buffer *tail;	/* Active buffer */
	struct llist_head free;	/* Free queue head */
	int memory_used;		/* Buffer space used excluding
								free queue */
    int mem_limit;                                
};
//
//struct crtl_atty_port {/*     <rongtao 2019.11.11>*/
//	struct crtl_atty_bufhead	buf;		/* Locked internally */
//	struct crtl_atty_struct	*tty;		/* Back pointer */
//	struct crtl_atty_struct	*itty;		/* internal back ptr */
//	const struct crtl_atty_port_operations *ops;	/* Port operations */
//	const struct crtl_atty_port_client_operations *client_ops; /* Port client operations */
//	crtl_lock_sem_t		lock;		/* Lock protecting tty field */
//	int			blocked_open;	/* Waiting to open */
//	int			count;		/* Usage count */
//	unsigned long		flags;		/* User TTY flags ASYNC_ */
//	unsigned long		iflags;		/* Internal flags TTY_PORT_ */
//	unsigned char		console:1,	/* port is a console */
//				low_latency:1;	/* optional: tune for latency */
//	crtl_lock_mutex_t		mutex;		/* Locking */
//	crtl_lock_mutex_t		buf_mutex;	/* Buffer alloc lock */
//	unsigned char		*xmit_buf;	/* Optional buffer */
//	unsigned int		close_delay;	/* Close port delay */
//	unsigned int		closing_wait;	/* Delay for output */
//	int			drain_delay;	/* Set to zero if no pure time
//						   based drain is needed else
//						   set to size of fifo */
//	void 			*client_data;
//};



struct crtl_atty_struct;

struct file {
	struct file *next;
	struct file *parent;
	const char *name;
	int lineno;
	int flags;
};

                                
/* tty operations */
struct crtl_atty_operations {
	void (*shutdown)(struct crtl_atty_struct *atty);
	void (*cleanup)(struct crtl_atty_struct *atty);
	int  (*write)(struct crtl_atty_struct * atty, const unsigned char *buf, int count);
	int  (*put_char)(struct crtl_atty_struct *atty, unsigned char ch);
	void (*flush_chars)(struct crtl_atty_struct *atty);
	int  (*write_room)(struct crtl_atty_struct *atty);
	int  (*chars_in_buffer)(struct crtl_atty_struct *atty);
	int  (*ioctl)(struct crtl_atty_struct *atty, struct file * file, unsigned int cmd, unsigned long arg);
	long (*compat_ioctl)(struct crtl_atty_struct *atty, struct file * file, unsigned int cmd, unsigned long arg);
	void (*set_termios)(struct crtl_atty_struct *atty, struct termios * old);
	void (*stop)(struct crtl_atty_struct *atty);
	void (*start)(struct crtl_atty_struct *atty);
	void (*hangup)(struct crtl_atty_struct *atty);
	int  (*break_ctl)(struct crtl_atty_struct *atty, int state);
	void (*flush_buffer)(struct crtl_atty_struct *atty);
	void (*wait_until_sent)(struct crtl_atty_struct *atty, int timeout);
	void (*send_xchar)(struct crtl_atty_struct *atty, char ch);
	int  (*resize)(struct crtl_atty_struct *atty, struct winsize *ws);
};



/* atty structure */
struct crtl_atty_struct {
    int	                    magic;/*ATTY_MAGIC*/
    int                     index;
    char                    name[__CRTL_ATTY_NAME_LEN];
    
    struct termios               termios; //terminal io (posix)
    struct winsize               winsize;
    
    struct crtl_atty_bufhead     buf;
    struct crtl_atty_struct      *link;
    struct crtl_atty_operations  *ops;
    unsigned long           flags;
    int                     count;
    unsigned char           stopped:1, hw_stopped:1, flow_stopped:1, packet:1;
	unsigned char           low_latency:1, warned:1;
	unsigned char           ctrl_status;	/* ctrl_lock */
	unsigned int            receive_room;	/* Bytes free for queue */
    
    /*
	 * The following is data for the N_TTY line discipline.  For
	 * historical reasons, this is included in the atty structure.
	 * Mostly locked by the BKL.
	 */
	unsigned int            column;
	unsigned char           lnext:1, erasing:1, raw:1, real_raw:1, icanon:1;
	unsigned char           closing:1;
	unsigned char           echo_overrun:1;
	unsigned short          minimum_to_wake;
	unsigned long           overrun_time;
	int                     num_overrun;
	unsigned long           process_char_map[256/(8*sizeof(unsigned long))];
    int                     read_fd;
	char                    *read_buf;
	int                     read_head;
	int                     read_tail;
	int                     read_cnt;
	unsigned long           read_flags[__CRTL_N_ATTY_BUF_SIZE/(8*sizeof(unsigned long))];
    int                     echo_fd;
	unsigned char           *echo_buf;
	unsigned int            echo_pos;
	unsigned int            echo_cnt;
	int                     canon_data;
	unsigned long           canon_head;
	unsigned int            canon_column;
	crtl_lock_mutex_t       read_lock;
	crtl_lock_mutex_t       write_lock;
	crtl_lock_mutex_t       output_lock;
	crtl_lock_mutex_t       echo_lock;
    int                     write_fd;
	unsigned char           *write_buf;
	int                     write_cnt;
};


#endif /*<__CRTL_BITS_ATTY_H>*/
