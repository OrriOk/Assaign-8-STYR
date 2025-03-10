#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <mqueue.h>

mqd_t startClient(void);

int sendQuitTask(mqd_t client);

int sendAddTask(mqd_t client, int operand1, int operand2);

int stopClient(mqd_t client);

int runServer(void);

// On skel, change QUEUE_NAME to a unique name.
#define QUEUE_NAME ( "/simple_calculator" __TIME__ )

#endif
