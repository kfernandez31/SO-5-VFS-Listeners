#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "helpers.h"

const char * DIR_PATH = "./dir";
const char * SRC_PATH = "./dev";
const char * DST_PATH = "./dir/dev";

int create_dev() {
    pid_t pid;
    pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Forking failed. Errno: %d.\n", errno);
        exit(1);
    }

    if (pid == 0) {
        if (execlp("/sbin/mknod", "/sbin/mknod", SRC_PATH, "c", "17", "0", NULL)) {
            fprintf(stderr, "Creating device %s failed. Errno: %d.\n", SRC_PATH, errno);
            exit(1);
        }
    }

    return pid;
}

int main() {
    int fd;
    pid_t child;

    child = create_dev();
    wait_exited_expect(child, 0, 1);

    mkdir_expect(DIR_PATH);
    fd = open_expect(DIR_PATH);

    child = spawn_notify_expect(fd, NOTIFY_MOVE);
    sleep(1);
    assert_lives(child);

    rename_expect(SRC_PATH, DST_PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
