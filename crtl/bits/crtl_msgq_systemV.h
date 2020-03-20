#ifndef __CRTL_MSGQ_SYSV_H
#define __CRTL_MSGQ_SYSV_H 1

//System V
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>



typedef int crtl_msqid_t;


//TODO:::::::::::::::::::



#ifndef _SYS_MSG_H


/* The following System V style IPC functions implement a message queue
   system.  The definition is found in XPG2.  */

#ifdef __USE_GNU
/* Template for struct to be used as argument for `msgsnd' and `msgrcv'.  */
struct msgbuf
  {
    __syscall_slong_t mtype;	/* type of received/sent message */
    char mtext[1];		/* text of the message */
  };
#endif

key_t ftok(const char *pathname, int proj_id);

__BEGIN_DECLS
struct msqid_ds {
    struct ipc_perm msg_perm;     /* Ownership and permissions */
    time_t          msg_stime;    /* Time of last msgsnd(2) */
    time_t          msg_rtime;    /* Time of last msgrcv(2) */
    time_t          msg_ctime;    /* Time of last change */
    unsigned long   __msg_cbytes; /* Current number of bytes in
                            queue (nonstandard) */
    msgqnum_t       msg_qnum;     /* Current number of messages
                            in queue */
    msglen_t        msg_qbytes;   /* Maximum number of bytes
                            allowed in queue */
    pid_t           msg_lspid;    /* PID of last msgsnd(2) */
    pid_t           msg_lrpid;    /* PID of last msgrcv(2) */
};
struct ipc_perm {
    key_t          __key;       /* Key supplied to msgget(2) */
    uid_t          uid;         /* Effective UID of owner */
    gid_t          gid;         /* Effective GID of owner */
    uid_t          cuid;        /* Effective UID of creator */
    gid_t          cgid;        /* Effective GID of creator */
    unsigned short mode;        /* Permissions */
    unsigned short __seq;       /* Sequence number */
};
struct msginfo {
    int msgpool; /* Size in kibibytes of buffer pool
                 used to hold message data;
                 unused within kernel */
    int msgmap;  /* Maximum number of entries in message
                 map; unused within kernel */
    int msgmax;  /* Maximum number of bytes that can be
                 written in a single message */
    int msgmnb;  /* Maximum number of bytes that can be
                 written to queue; used to initialize
                 msg_qbytes during queue creation
                 (msgget(2)) */
    int msgmni;  /* Maximum number of message queues */
    int msgssz;  /* Message segment size;
                 unused within kernel */
    int msgtql;  /* Maximum number of messages on all queues
                 in system; unused within kernel */
    unsigned short int msgseg;
              /* Maximum number of segments;
                 unused within kernel */
};



/* Message queue control operation.  */
extern int msgctl(int __msqid, int __cmd, struct msqid_ds *__buf);
//__cmd:
//    IPC_STAT
//    IPC_SET
//    IPC_RMID
//    IPC_INFO (Linux-specific)
//    MSG_INFO (Linux-specific)
//    MSG_STAT (Linux-specific)

/* Get messages queue.  */
extern int msgget(key_t __key, int __msgflg);

/* Receive message from message queue.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern ssize_t msgrcv(int __msqid, void *__msgp, size_t __msgsz,
		       long int __msgtyp, int __msgflg);

/* Send message to message queue.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int msgsnd(int __msqid, const void *__msgp, size_t __msgsz,
		   int __msgflg);

__END_DECLS


#endif //_SYS_MSG_H

#endif /*<__CRTL_MSGQ_SYSV_H>*/


