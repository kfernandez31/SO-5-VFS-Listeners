#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <lib.h>
#include <minix/rs.h>
#include "helpers.h"

int open_expect(const char * path) {
    int fd;
    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "Opening %s failed. Errno %d.\n", path, errno);
        exit(1);
    }

    fprintf(stderr, "Opened file %s. File descriptor %d.\n", path, fd);
    return fd;
}

int create_expect(const char * path) {
    int fd;
    fd = open(path, O_RDWR | O_CREAT | O_EXCL);
    if (fd < 0) {
        fprintf(stderr, "Creating %s failed. Errno %d.\n", path, errno);
        exit(1);
    }

    fprintf(stderr, "Created file %s. File descriptor %d.\n", path, fd);
    return fd;
}

pid_t spawn_notify_expect(int file, int event) {
    pid_t pid;
    message m;
    endpoint_t vfs_ep;
    int res;

    pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Forking failed. Errno %d.\n", errno);
        exit(1);
    }
    if (pid > 0) {
        fprintf(stderr, "Spawned a child %d. File %d. Event %d.\n", pid, file, event);
        return pid;
    }

    if (minix_rs_lookup("vfs", &vfs_ep)) {
        fprintf(stderr, "Looking up vfs endpoint failed. Errno %d.\n", errno);
        exit(1);
    }

    m.m_lc_vfs_notify.fd = file;
    m.m_lc_vfs_notify.event = event;
    errno = 0;

    _syscall(vfs_ep, VFS_NOTIFY, &m);
    res = errno;

    exit(errno);
}

void wait_exited_expect(pid_t child, int status, int timeout) {
    pid_t pid;
    int res;

    while (1) {
        pid = waitpid(child, &res, WNOHANG);

        if (pid == -1) {
            fprintf(stderr, "Waiting for child %d failed. Errno %d.\n", child, errno);
            exit(1);
        }

        if (pid > 0) {
            if (!WIFEXITED(res)) {
                fprintf(stderr, "Child %d did not exit normally.\n", child);
                exit(1);
            }

            if (WEXITSTATUS(res) != status) {
                fprintf(stderr, "Child %d exited with an unexpected status %d. Expected %d.\n", child, WEXITSTATUS(res), status);
                exit(1);
            }

            return;
        }

        if (timeout < 1) {
            fprintf(stderr, "Waiting for child %d timeouted.\n", child);
            kill(child, 9);
            exit(1);
        }

        --timeout;
        sleep(1);
    }
}

void wait_signaled_expect(pid_t child, int sig, int timeout) {
    pid_t pid;
    int status;

    while (1) {
        pid = waitpid(child, &status, WNOHANG);

        if (pid == -1) {
            fprintf(stderr, "Waiting for child %d failed. Errno %d.\n", child, errno);
            exit(1);
        }

        if (pid > 0) {
            if (!WIFSIGNALED(status)) {
                fprintf(stderr, "Child %d did not die to a signal.\n", child);
                exit(1);
            }

            if (WTERMSIG(status) != sig) {
                fprintf(stderr, "Child %d died to an unexpected signal %d. Expected %d.\n", child, WTERMSIG(status), sig);
                exit(1);
            }

            return;
        }

        if (timeout < 1) {
            fprintf(stderr, "Waiting for child %d timeouted.\n", child);
            kill(child, 9);
            exit(1);
        }

        --timeout;
        sleep(1);
    }
}

void assert_lives(pid_t child) {
    int status;
    pid_t pid;

    pid = waitpid(child, &status, WNOHANG);

    if (pid == -1) {
        fprintf(stderr, "Waiting for child %d failed. Errno %d.\n", child, errno);
        exit(1);
    }

    if (pid > 0) {
        if (WIFEXITED(status)) {
            fprintf(stderr, "Child %d exited with status code %d.\n", child, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Child %d died to a signal %d.\n", child, WTERMSIG(status));
        } else {
            fprintf(stderr, "Child %d exited unexpectedly, reason unknown.\n", child);
        }
        exit(1);
    }
}

void mkdir_expect(const char * path) {
    if (mkdir(path, S_IRWXU)) {
        fprintf(stderr, "Creating a directory %s failed. Errno: %d.\n", path, errno);
        exit(1);
    }

    fprintf(stderr, "Created directory %s.\n", path);
}

void rename_expect(char const * src, char const *dst) {
    if (rename(src, dst)) {
        fprintf(stderr, "Renaming file %s to %s failed. Errno: %d.\n", src, dst, errno);
        exit(1);
    }

    fprintf(stderr, "Renamed file %s to %s.\n", src, dst);
}