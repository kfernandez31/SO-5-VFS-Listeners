#include <unistd.h>
#include <fcntl.h>
#include "helpers.h"

const char * DIR_PATH = "./dir";
const char * SRC_PATH = "./dir2";
const char * DST_PATH = "./dir/dir2";

int main() {
    int fd;
    pid_t child;

    mkdir_expect(SRC_PATH);
    mkdir_expect(DIR_PATH);
    fd = open_expect(DIR_PATH);

    child = spawn_notify_expect(fd, NOTIFY_MOVE);
    sleep(1);
    assert_lives(child);

    rename_expect(SRC_PATH, DST_PATH);
    wait_exited_expect(child, 0, 1);

    return 0;
}
