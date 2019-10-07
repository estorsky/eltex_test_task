#ifndef SHARED_IPC_H
#define SHARED_IPC_H

#include <limits.h>
#include <pthread.h>

char const PATH_IPC[] = "ipc:///tmp/demipc";

enum types_mes
{
    init,
    running
};

struct message
{
    char type;
    pid_t pid;
    char name[NAME_MAX];
    char args[NAME_MAX];
};

#endif
