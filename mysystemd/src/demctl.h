#ifndef DEMCTL_H
#define DEMCTL_H

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <wait.h>
#include <sys/stat.h>

#include "settings.h"
#include "zlog.h"
#include "units.h"
#include "parse.h"

void dem_control(void);

int init_deamons(struct unit *head);

int system_vfork(const char *cmd);

int valid_dem(const char *name);

int valid_file(const char *path);

int init_log(void);

void daemonize(void);

#endif

