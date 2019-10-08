#ifndef SYNC_H
#define SYNC_H

#include <zmq.h>

#include "units.h"
#include "zlog.h"

extern pthread_mutex_t recver_sync;

void *recver();

#endif

