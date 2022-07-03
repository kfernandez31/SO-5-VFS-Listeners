#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

const char * PATH = "./dir";
const char * PATH_DEV = "./dir/dev";

int create_dev() {
    pid_t pid;
    pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Forking failed. Errno: %d.\n", errno);
        exit(1);
    }

    if (pid == 0) {
        if (execlp("/sbin/mknod", "/sbin/mknod", PATH_DEV, "c", "17", "0", NULL)) {
            fprintf(stderr, "Creating device %s failed. Errno: %d.\n", PATH_DEV, errno);
            exit(1);
        }
    }

    return pid;
}

int main() {
    int fd;
    pid_t child;
    pid_t creator;

    mkdir_expect(PATH);
    fd = open_expect(PATH);

    child = spawn_notify_expect(fd, NOTIFY_CREATE);
    sleep(1);
    assert_lives(child);

    creator = create_dev();
    wait_exited_expect(child, 0, 1);
    wait_exited_expect(creator, 0, 1);

    return 0;
}
