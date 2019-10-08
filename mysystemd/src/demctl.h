#ifndef DEMCTL_H
#define DEMCTL_H

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <sys/stat.h>

#include "settings.h"
#include "units.h"
#include "sync.h"
#include "zlog.h"

void dem_control(void);

int init_deamons(void);

int start_dem(const char *cmd);

int valid_dem_unsafe(const char *name);

int valid_file(const char *path);

int init_log(void);

int daemonize(void);

#endif

