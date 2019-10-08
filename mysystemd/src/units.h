#ifndef UNITS_H
#define UNITS_H

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#include "settings.h"
#include "zlog.h"

enum Answer
{
    RECV = 0,
    NOT_RECV,
};

enum Status
{
    STARTED = 0,
    STOPPED,
};

typedef struct Unit 
{
    char name[NAME_MAX];
    char args[NAME_MAX];
    pid_t pid;
    char status;
    char answer;
    unsigned int non_respons;
    struct Unit *next;
} Unit;

extern struct Unit *head;

void unit_prints(void);

int unit_init_pid(
    char const *name,
    char const *args,
    pid_t const pid);

int unit_change_answer(
    char const *name,
    char const *args,
    pid_t const pid);

int unit_update_non_response(void);

int unit_new_element(
    char const *name,
    char const *arg);

#endif

