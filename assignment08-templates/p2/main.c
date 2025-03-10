#include "testlib.h"
#include "message_queue.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    // This is just in case you missed to clean up the queue.
    // If your program works correctly, this line should not be needed.
    mq_unlink("/simple_calculator");

    switch(fork()) {
    case -1:
        perror("fork");
        exit(0);
    case 0:
        // Child is server
        printf("Starting test server\n");
        if (runServer() < 0) {
            perror("runServer");
            exit(1);
        }
        printf("Test server done\n");
        break;
    default:
        // Hope that the server is started now.
        sleep(1);
        // Send some test commands to server.
        printf("Running test client\n");
        mqd_t clientHandle = startClient();
        if (clientHandle < 0) {
            perror("startClient");
            exit(1);
        }
        if (sendAddTask(clientHandle, 10, -2) < 0) {
            perror("sendAddTask");
        }
        if (sendQuitTask(clientHandle) < 0) {
            perror("sendQuitTask");
        }
        if (stopClient(clientHandle) < 0) {
            perror("stopClient");
        }
        printf("Test client done\n");
        wait(NULL);
    }

    return 0;
}
