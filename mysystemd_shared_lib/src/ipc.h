#ifndef IPC_H
#define IPC_H

#include "../include/mysystemd_shared_lib.h"
#include <zmq.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>

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

#endif
