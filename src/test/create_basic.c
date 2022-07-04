#include <unistd.h>
#include <fcntl.h>
#include "helpers.h"

const char * DIR_PATH = "./dir";
const char * FILE_PATH = "./dir/file";

int main() {
    int fd;
    pid_t child;

    mkdir_expect(DIR_PATH);
    fd = open_expect(DIR_PATH);

    child = spawn_notify_expect(fd, NOTIFY_CREATE);
    sleep(1);
    assert_lives(child);

    create_expect(FILE_PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
