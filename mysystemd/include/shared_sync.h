#ifndef SHARED_SYNC_H
#define SHARED_SYNC_H

#include <limits.h>
#include <pthread.h>

char const PATH_IPC[] = "ipc:///tmp/demipc";

enum Types_mes
{
    INIT = 0,
    RUNNING,
};

typedef struct Message
{
    char type;
    pid_t pid;
    char name[NAME_MAX];
    char args[NAME_MAX];
} Message;

#endif
