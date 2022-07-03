#include <unistd.h>
#include <fcntl.h>
#include "helpers.h"

const char * PATH = "/root";

int main() {
    int fd;
    pid_t child;

    fd = open_expect(PATH);

    child = spawn_notify_expect(fd, NOTIFY_OPEN);
    sleep(1);
    assert_lives(child);

    open_expect(PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}