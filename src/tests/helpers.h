#ifndef HELPERS_H
#define HELPERS_H

#include <sys/types.h>

/**
 * Opens the file at the given `path` for reading and returns the file descriptor.
 * Exits upon error.
 */
int open_expect(const char * path);

/**
 * Creates a file at the given `path` and opens it for reading and writing.
 * Fails if the given `path` is occupied.
 * Exists upon error.
 */
int create_expect(const char * path);

/**
 * Closes the given `file`.
 * Exits upon error.
 */
void close_expect(int file);

/**
 * Spawns a child process and returns it's pid.
 * The child calls NOTIFY for `event` on the given `file`.
 * Its return code is the errno value after the NOTIFY call.
 * Exits upon error.
 */
pid_t spawn_notify_expect(int file, int event);

/**
 * Waits for a child process to normally exit with status code `status`.
 * Waits for at most `timeout` seconds.
 * Exits upon error.
 */
void wait_exited_expect(pid_t child, int status, int timeout);

/**
 * Waits for a child process to be killed by a signal `sig`.
 * Waits for at most `timeout` seconds.
 * Exits upon error.
 */
void wait_signaled_expect(pid_t child, int sig, int timeout);

/**
 * Asserts that a child is alive.
 * Exits upon error.
 */
void assert_lives(pid_t child);

/** Creates a directory under the given `path`.
 * Exists upon error.
 */
void mkdir_expect(char const * path);

/** Renames a file from `src` to `dst`.
 * Exists upon error.
 */
void rename_expect(char const * src, char const *dst);

#define NR_NOTIFY 8

#endif /* HELPERS_H */
