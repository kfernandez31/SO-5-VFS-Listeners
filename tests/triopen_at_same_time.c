#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "helpers.h"

const char * PATH = "/root/.profile";

int main() {
    int fd;
    pid_t child;

    fd = open_expect(PATH);
    if (close(fd)) {
        fprintf(stderr, "Closing file %s failed. Errno: %d.\n", PATH, errno);
        exit(1);
    }

    fd = open_expect(PATH);

    child = spawn_notify_expect(fd, NOTIFY_TRIOPEN);
    sleep(1);
    assert_lives(child);

    open_expect(PATH);
    sleep(1);
    assert_lives(child);

    open_expect(PATH);
    sleep(1);
    wait_exited_expect(child, 0, 1);

    return 0;
}
