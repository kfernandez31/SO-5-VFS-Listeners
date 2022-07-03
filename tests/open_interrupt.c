#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>
#include "helpers.h"

char const * FILE_BASE = "./file";

int main() {
    int fd[NR_NOTIFY];
    pid_t children[NR_NOTIFY];
    int i;

    for (i = 0; i < NR_NOTIFY; ++i) {
        char buff[16];
        sprintf(buff, "%s-%d", FILE_BASE, i);
        fd[i] = create_expect(buff);
    }

    children[0] = spawn_notify_expect(fd[0], NOTIFY_OPEN);
    sleep(1);
    assert_lives(children[0]);
    kill(children[0], 9);
    wait_signaled_expect(children[0], 9, 1);

    for (i = 0; i < NR_NOTIFY; ++i) {
        children[i] = spawn_notify_expect(fd[i], NOTIFY_OPEN);
    }
    sleep(1);
    for (i = 0; i < NR_NOTIFY; ++i) {
        assert_lives(children[i]);
    }

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
