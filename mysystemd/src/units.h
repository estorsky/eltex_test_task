#ifndef UNITS_H
#define UNITS_H

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <sys/types.h>
#include <signal.h>

#include "ipc.h"
#include "demctl.h"

enum flag
{
    answer,
    no_answer
};

enum status
{
    started,
    stopped
};

struct unit 
{
    char name[NAME_MAX];
    char args[NAME_MAX];
    pid_t pid;
    char status;
    char flag;
    unsigned int non_respons;
    struct unit *next;
};

void unit_prints(
    const struct unit *head);

void unit_init_pid(
    struct unit *head,
    char const *name,
    char const *args,
    pid_t const pid);

void unit_change_flag(
    struct unit *head,
    char const *name,
    char const *args,
    pid_t const pid);

void unit_update_non_response(
    struct unit *head);

int unit_new_element(
    struct unit **head,
    char const *name,
    char const *arg);

#endif

