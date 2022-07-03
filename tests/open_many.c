#include <unistd.h>
#include <fcntl.h>
#include "helpers.h"

const char * PATH = "/root/.profile";

int main() {
    int fd;
    pid_t children[3];
    int i;

    fd = open_expect(PATH);

    for (i = 0; i < 3; ++i) {
        children[i] = spawn_notify_expect(fd, NOTIFY_OPEN);
    }
    sleep(1);
    for (i = 0; i < 3; ++i) {
        assert_lives(children[i]);
    }

    open_expect(PATH);
    for (i = 0; i < 3; ++i) {
        wait_exited_expect(children[i], 0, 1);
    }

    return 0;
}
