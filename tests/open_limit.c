#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include "helpers.h"

const char * FILE_BASE = "./file";

int main() {
    int fd[NR_NOTIFY + 1];
    pid_t children[NR_NOTIFY + 1];
    int i;

    for (i = 0; i < NR_NOTIFY + 1; ++i) {
        char buff[16];
        sprintf(buff, "%s-%d", FILE_BASE, i);
        fd[i] = create_expect(buff);
    }

    for (i = 0; i < NR_NOTIFY; ++i) {
        children[i] = spawn_notify_expect(fd[i], NOTIFY_OPEN);
    }
    sleep(1);
    for (i = 0; i < NR_NOTIFY; ++i) {
        assert_lives(children[i]);
    }

    children[NR_NOTIFY] = spawn_notify_expect(fd[NR_NOTIFY], NOTIFY_OPEN);
    wait_exited_expect(children[NR_NOTIFY], ENONOTIFY, 1);

    for (i = 0; i < NR_NOTIFY; ++i) {
        char buff[16];
        sprintf(buff, "%s-%d", FILE_BASE, i);
        fd[i] = open_expect(buff);
    }
    for (i = 0; i < NR_NOTIFY; ++i) {
        wait_exited_expect(children[i], 0, 1);
    }

    return 0;
}
