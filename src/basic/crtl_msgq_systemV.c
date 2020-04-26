#include "crtl/bits/crtl_msgq_systemV.h"
#include "crtl/crtl_log.h"
#include "crtl/crtl_assert.h"

#include "crtl/bits/crtl_types_basic.h"

#include "crtl/easy/macro.h"


//TODO:::::::::::::::::::



key_t ftok(const char *pathname, int proj_id);
/* Get messages queue.  */
extern int msgget(key_t __key, int __msgflg);

crtl_msqid_t crtl_msgget(const char *name, int proj_id, int __msgflg);


/* Message queue control operation.  */
extern int msgctl(int __msqid, int __cmd, struct msqid_ds *__buf);
//__cmd:
//    IPC_STAT
//    IPC_SET
//    IPC_RMID
//    IPC_INFO (Linux-specific)
//    MSG_INFO (Linux-specific)
//    MSG_STAT (Linux-specific)

