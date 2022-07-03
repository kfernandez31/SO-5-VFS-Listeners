#include <errno.h>
#include <fcntl.h>
#include "helpers.h"

int main() {
    pid_t child;

    child = spawn_notify_expect(99, NOTIFY_OPEN);
    wait_exited_expect(child, EBADF, 1);

    return 0;
}
