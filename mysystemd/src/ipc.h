#ifndef IPC_H
#define IPC_H

#include <zmq.h>
#include "units.h"

extern pthread_mutex_t recver_sync;
extern struct unit *head;

void *recver();

#endif

