#include "testlib.h"
#include "pipe.h"
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main()
{
    test_start("pipe.c");

    printf("Running ls\n");
    errno = 0;
    char *argv1[] = {
        "/bin/ls",
        "/home",
        NULL
    };
    int result1 = execute(argv1);
    test_equals_int(result1, 0);
    test_equals_int(errno, 0);

    printf("Running unknown-program-name\n");
    errno = 0;
    char * argv2[] = {
	"unknown-program-name",
        NULL
    };
    int result2 = execute(argv2);
    test_equals_int(result2, -1);
    test_equals_int(errno, ENOENT);
    printf("Error: %s (errno=%d)\n", strerror(errno), errno);

    return test_end();
}
