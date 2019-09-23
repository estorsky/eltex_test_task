#ifndef UNITS_H
#define UNITS_H

#include <limits.h>

struct unit {
    char name[NAME_MAX];
    char param[NAME_MAX];
    char status;
    unsigned int pid;
    unsigned int num_non_respons;
    struct unit *next;
};

#endif

