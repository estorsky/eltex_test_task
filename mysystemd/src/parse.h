#ifndef PARSE_H
#define PARSE_H

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "zlog.h"
#include "cJSON.h"
#include "units.h"
 
extern struct unit *head;

int read_config(void);

#endif

