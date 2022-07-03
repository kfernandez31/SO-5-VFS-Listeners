#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

const char * PATH = "/root/.profile";

int main() {
    int fd;
    pid_t child;

    fd = open_expect(PATH);

    child = spawn_notify_expect(fd, NOTIFY_OPEN);
    sleep(1);
    assert_lives(child);

    if (open(PATH, O_RDWR | O_CREAT | O_EXCL) > 0) {
        fprintf(stderr, "Creating the already existing file %s did not fail.\n", PATH);
        exit(1);
    }
    sleep(1);
    assert_lives(child);

    open_expect(PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
