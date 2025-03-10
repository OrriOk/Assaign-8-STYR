// This changes the way some includes behave.
// This should stay before any include.
#define _GNU_SOURCE

#include "pipe.h"
#include <sys/wait.h> /* For waitpid */
#include <unistd.h> /* For fork, pipe */
#include <stdlib.h> /* For exit */
#include <fcntl.h>
#include <errno.h>

int execute(char *argv[])
{

    if (argv == NULL) {
        return -1;
    }

    // -------------------------
    // TODO: Open a pipe
    // -------------------------

    int pipe_fd[2];
    if (pipe2(pipe_fd, O_CLOEXEC) == -1) {
        return -1;
    }

    int child_pid = fork();
    if (child_pid == -1) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return -1;
    } else if (child_pid == 0) {

        // Replace program
        close(pipe_fd[0]);

        execvp(argv[0], argv);

        // -------------------------
        // TODO: Write the error on the pipe
        // -------------------------
        int exec_errno = errno;
        if (write(pipe_fd[1], &exec_errno, sizeof(exec_errno)) == -1) {
            // Writing failed - not much we can do here
        }
        close(pipe_fd[1]);

        exit(127);
    } else {
        int status, hadError = 0;

        

        int waitError = waitpid(child_pid, &status, 0);
        if (waitError == -1) {
            // Error while waiting for child.
            hadError = 1;
        } else if (!WIFEXITED(status)) {
            // Our child exited with another problem (e.g., a segmentation fault)
            // We use the error code ECANCELED to signal this.
            hadError = 1;
            errno = ECANCELED;
        } else {
            // -------------------------
            // TODO: If there was an execvp error in the child, set errno
            //       to the value execvp set it to.
            // -------------------------

            close(pipe_fd[1]);

            int child_errno;
            ssize_t bytes_read = read(pipe_fd[0], &child_errno, sizeof(child_errno));
            close(pipe_fd[0]);

            if (bytes_read > 0) {
                // Child failed to exec, set errno
                errno = child_errno;
                hadError = 1;
            }

        }

        return hadError ? -1 : WEXITSTATUS(status);
    }
}
