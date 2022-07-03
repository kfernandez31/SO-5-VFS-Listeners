#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "helpers.h"

const char * PATH = "/root/.profile";

int main() {
    int fd;
    pid_t child;

    fd = open_expect(PATH);

    child = spawn_notify_expect(fd, NOTIFY_CREATE);
    wait_exited_expect(child, ENOTDIR, 1);

    return 0;
}
