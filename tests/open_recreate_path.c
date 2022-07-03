#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include "helpers.h"

const char * PATH = "./recreate_path";

int main() {
    int fd;
    pid_t child;

    fd = create_expect(PATH);

    child = spawn_notify_expect(fd, NOTIFY_OPEN);
    sleep(1);
    assert_lives(child);

    if (unlink(PATH)) {
        fprintf(stderr, "Unlinking file %s failed. Errno: %d.\n", PATH, errno);
        exit(1);
    }
    sleep(1);
    assert_lives(child);

    create_expect(PATH);
    open_expect(PATH);
    sleep(1);
    assert_lives(child);

    kill(child, 9);
    wait_signaled_expect(child, 9, 1);

    return 0;
}
