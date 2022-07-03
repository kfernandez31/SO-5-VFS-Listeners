#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

const char * DIR_PATH = "./dir";
const char * BASE_PATH = "/root/.profile";
const char * SRC_PATH = "./file";
const char * DST_PATH = "./dir/file";

int main() {
    int fd;
    pid_t child;

    if (link(BASE_PATH, SRC_PATH)) {
        fprintf(stderr, "Creating a hard link from %s to %s failed. Errno: %d.\n", BASE_PATH, SRC_PATH, errno);
        exit(1);
    }

    mkdir_expect(DIR_PATH);
    fd = open_expect(DIR_PATH);

    child = spawn_notify_expect(fd, NOTIFY_MOVE);
    sleep(1);
    assert_lives(child);

    rename_expect(SRC_PATH, DST_PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
