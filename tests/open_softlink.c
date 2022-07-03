#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include "helpers.h"

const char * BASEPATH = "/root/.profile";
const char * LINKPATH = "./softlink";

int main() {
    int fd;
    pid_t child;

    if (symlink(BASEPATH, LINKPATH)) {
        fprintf(stderr, "Creating a soft link from %s to %s failed. Errno: %d.\n", BASEPATH, LINKPATH, errno);
        exit(1);
    }

    fd = open_expect(LINKPATH);
    child = spawn_notify_expect(fd, NOTIFY_OPEN);
    sleep(1);
    assert_lives(child);

    open_expect(LINKPATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
