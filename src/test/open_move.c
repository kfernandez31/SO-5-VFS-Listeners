#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include "helpers.h"

const char * PATH1 = "./move_1";
const char * PATH2 = "./move_2";

int main() {
    int fd;
    pid_t child;

    fd = create_expect(PATH1);

    child = spawn_notify_expect(fd, NOTIFY_OPEN);
    sleep(1);
    assert_lives(child);

    rename_expect(PATH1, PATH2);
    sleep(1);
    assert_lives(child);

    open_expect(PATH2);
    wait_exited_expect(child, 0, 1);

    return 0;
}
