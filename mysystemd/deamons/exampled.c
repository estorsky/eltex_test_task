#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <zmq.h>

#include <limits.h>
#include <pthread.h>

#include <string.h>

#define MAX_LEN 256

// #include "../src/ipc.h"
#include "../../mysystemd_shared_lib/include/mysystemd_shared_lib.h"

int main (int argc, char* argv[])
{
    mysystemd_sync();
    // printf("%s\n", argv[0]);

    /* char buffer[MAX_LEN];
    buffer[0] = 0;
    sprintf(buffer, "%s", argv[0]);
    int offset = strlen(buffer);
    while (argv++,--argc)
    {
        int toWrite = MAX_LEN-offset;
        int written = snprintf(buffer+offset, toWrite, "%s ", *argv);
        if(toWrite < written)
        {
            break;
        }
        offset += written;
    }     
    printf("%s\n", buffer); */

    /* while (1)
       {
        sleep(1);
        printf("a");
        if (argc > 1)
        {
            if (!strcmp(argv[1], "b"))
            {
                printf("b");
            }
        }
        if (argc > 2)
        {
            if (!strcmp(argv[2], "c"))
            {
                printf("c");
            }
        }
        fflush(stdout);
    } */

}

