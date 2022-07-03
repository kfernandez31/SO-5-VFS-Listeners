#include <fcntl.h>
#include <unistd.h>
#include "helpers.h"

const char * PATH = "/root/.profile";

int main() {
    int fd;
    pid_t child;

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
