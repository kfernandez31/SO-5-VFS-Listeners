#include <unistd.h>
#include <errno.h>
#include "helpers.h"

int main() {
    int fd;
    pid_t child;

    fd = open_expect("/root/.profile");

    child = spawn_notify_expect(fd, 32);
    wait_exited_expect(child, EINVAL, 1);

    return 0;
}