#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

const char * DIR_PATH = "./dir";
const char * SRC_PATH = "/root/.profile";
const char * DST_PATH = "./dir/softlink";

int main() {
    int fd;
    pid_t child;

    mkdir_expect(DIR_PATH);
    fd = open_expect(DIR_PATH);

    child = spawn_notify_expect(fd, NOTIFY_CREATE);
    sleep(1);
    assert_lives(child);

    if (symlink(SRC_PATH, DST_PATH)) {
        fprintf(stderr, "Creating a soft link from %s to %s failed. Errno: %d.\n", SRC_PATH, DST_PATH, errno);
        exit(1);
    }
    wait_exited_expect(child, 0, 1);

    return 0;
}
