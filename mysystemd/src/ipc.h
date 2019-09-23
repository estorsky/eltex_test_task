#ifndef IPC_H
#define IPC_H

#include <zmq.h>
#include <limits.h>
#include <pthread.h>

//def path to ipc

enum types_mes {
    init,
    running
};

struct message {
    char type;
    int pid;
    char name_dem[NAME_MAX];
};

void *recver( void *const arg);

#endif
