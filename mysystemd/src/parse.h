#ifndef PARSE_H
#define PARSE_H

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "units.h"
#include "demctl.h"
#include "cJSON.h"
#include "zlog.h"
 
int read_config(void);

#endif

