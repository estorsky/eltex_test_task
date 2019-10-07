#include "demctl.h"

pthread_mutex_t recver_sync = PTHREAD_MUTEX_INITIALIZER;
struct unit *head = NULL;

char const PATH_CONFIG[] = "./deamons/config.json";
char const PATH_DEAMONS[] = "./deamons/";

int main ()
{
    int ret = 0;

    daemonize();

    if (0 != init_log())
    {
        ret = -1;
        goto finally;
    }

    if (0 != read_config())
    {
        ret = -2;
        goto finally;
    }

    dem_control();

finally:
    return ret;
}

