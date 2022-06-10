#include "fs.h"
#include "notify.h"
#include "vnode.h"
#include "file.h"
#include <fcntl.h>

#include <stdio.h>

#define is_valid_event(event) (event == NOTIFY_OPEN || event == NOTIFY_TRIOPEN || event == NOTIFY_CREATE || event == NOTIFY_MOVE)

static struct listener* get_free_entry(int event, struct vnode* vnode) {
    for (int i = 0; i < NR_NOTIFY; i++) {
        struct listener* listener = &listeners[i];
        if (listener->l_fproc == NULL) {
            // slot is ours to use
            listener->l_vnode = vnode;
            listener->l_fproc = fp;
            listener->l_event = event;
            return listener;
        }
    }
    return NULL;
}

static int suspend_listener(int event, struct vnode* vnode) {
    struct listener* listener = get_free_entry(event, vnode);
    return (listener == NULL)? (ENONOTIFY) : (SUSPEND); 
}

static int do_notify_open(struct vnode* vnode) {
    return suspend_listener(NOTIFY_OPEN, vnode);
}

static int do_notify_triopen(struct vnode* vnode) {
    if (vnode->v_ref_count < 3) 
        return suspend_listener(NOTIFY_TRIOPEN, vnode);
    return(OK); // proceed without hanging
}

static int do_notify_create(struct vnode* vnode) {
    // file should be a directory
    if (!S_ISDIR(vnode->v_mode)) 
        return(ENOTDIR);
    return suspend_listener(NOTIFY_CREATE, vnode);
}

static int do_notify_move(struct vnode* vnode) {
    // file should be a directory
    if (!S_ISDIR(vnode->v_mode))
        return(ENOTDIR);
    return suspend_listener(NOTIFY_MOVE, vnode);
}

void wake_listeners(int event, struct vnode *vnode) {
    if (!is_valid_event(event))
        return;

    for (int i = 0; i < NR_NOTIFY; i++) {
        struct listener* listener = &listeners[i];
        if (listener->l_fproc != NULL && listener->l_vnode == vnode && listener->l_event == event) {
            listener->l_fproc = NULL; // free up the slot
            listener->l_fproc->fp_blocked_on = FP_BLOCKED_ON_NONE;
            replycode(listener->l_fproc->fp_endpoint, OK); // wake the listener
        }
    }
}

int do_notify(void) {
    int fd = job_m_in.m_lc_vfs_notify.fd;
    int event = job_m_in.m_lc_vfs_notify.event;

    struct filp* filp = get_filp(fd, VNODE_NONE); // no locking needed
    if (filp == NULL) {
        return(EBADF); // invalid file descriptor
    }
    struct vnode* vnode = filp->filp_vno;

    switch (event) {
        case NOTIFY_OPEN:
            return do_notify_open(vnode);
        case NOTIFY_TRIOPEN:
            return do_notify_triopen(vnode);
        case NOTIFY_CREATE:
            return do_notify_create(vnode);
        case NOTIFY_MOVE:
            return do_notify_move(vnode);
    }
    return(EINVAL); // invalid event type
}
