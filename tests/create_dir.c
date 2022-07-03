#include <unistd.h>
#include <fcntl.h>
#include "helpers.h"

const char * OUTER_PATH = "./dir";
const char * INNER_PATH = "./dir/dir";

int main() {
    int fd;
    pid_t child;

    mkdir_expect(OUTER_PATH);
    fd = open_expect(OUTER_PATH);

    child = spawn_notify_expect(fd, NOTIFY_CREATE);
    sleep(1);
    assert_lives(child);

    create_expect(INNER_PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
