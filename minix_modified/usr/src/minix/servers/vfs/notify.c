#include "fs.h"
#include "vnode.h"
#include "file.h"
#include <fcntl.h>
#include <sys/stat.h>

#define suspend_as_listener(why){\
    num_listeners++;\
    suspend(why);\
    num_listeners--;\
}\

// number of processes listening on a notify-event 
static int num_listeners = 0;

static int do_notify_open(struct vnode* vnode) {
    suspend_as_listener(FP_BLOCKED_ON_NOTIFY_OPEN);
    return(OK);
}

static int do_notify_triopen(struct vnode* vnode) {
    if (vnode->v_ref_count < THREE) {
        suspend_as_listener(FP_BLOCKED_ON_NOTIFY_TRIOPEN);
    }
    return(OK);
}

static int do_notify_create(struct vnode* vnode) {
    // this should be a directory
    if (!S_ISDIR(vnode->v_mode)) {
        return(ENOTDIR);
    }
    suspend_as_listener(FP_BLOCKED_ON_NOTIFY_CREATE);
    return(OK);
}

static int do_notify_move(struct vnode* vnode) {
    // this should be a directory
    if (!S_ISDIR(vnode->v_mode)) {
        return(ENOTDIR);
    }
    suspend_as_listener(FP_BLOCKED_ON_NOTIFY_MOVE);
    return(OK);
}

int do_notify(void) {
    int fd = job_m_in.m_lc_vfs_notify.fd;
    int event = job_m_in.m_lc_vfs_notify.event;

    struct filp* filp = get_filp(fd, VNODE_NONE); // no locking needed
    // invalid file descriptor
    if (filp == NULL) {
        return(EBADF);
    }
    
    struct vnode* vnode = filp->filp_vno;

    switch (event) {
        case NOTIFY_OPEN:
            if (num_listeners == NR_NOTIFY) 
                return(ENONOTIFY);
            return do_notify_open(vnode);
        case NOTIFY_TRIOPEN:
            if (num_listeners == NR_NOTIFY)
                return(ENONOTIFY);
            return do_notify_triopen(vnode);
        case NOTIFY_CREATE:
            if (num_listeners == NR_NOTIFY) 
                return(ENONOTIFY);
            return do_notify_create(vnode);
        case NOTIFY_MOVE:
            if (num_listeners == NR_NOTIFY) 
                return(ENONOTIFY);
            return do_notify_move(vnode);
        default: {
            // invalid event type
            return(EINVAL);
        }
    }
}
