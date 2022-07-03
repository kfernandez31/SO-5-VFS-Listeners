#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "helpers.h"

const char * SRC_PATH = "./file1";
const char * DST_PATH = "./file2";

int main() {
    int fd;
    pid_t child;

    create_expect(SRC_PATH);
    fd = open_expect(".");

    child = spawn_notify_expect(fd, NOTIFY_MOVE);
    sleep(1);
    assert_lives(child);

    rename_expect(SRC_PATH, DST_PATH);
    sleep(1);
    assert_lives(child);

    kill(child, 9);
    wait_signaled_expect(child, 9, 1);

    return 0;
}
