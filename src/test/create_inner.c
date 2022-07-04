#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "helpers.h"

const char * PATH_OUTER = "./dir";
const char * PATH_INNER = "./dir/dir";
const char * FILE = "./dir/dir/file";

int main() {
    int fd;
    pid_t child;

    mkdir_expect(PATH_OUTER);
    mkdir_expect(PATH_INNER);

    fd = open_expect(PATH_OUTER);

    child = spawn_notify_expect(fd, NOTIFY_CREATE);
    sleep(1);
    assert_lives(child);

    create_expect(FILE);
    sleep(1);
    assert_lives(child);

    kill(child, 9);
    wait_signaled_expect(child, 9, 1);

    return 0;
}
