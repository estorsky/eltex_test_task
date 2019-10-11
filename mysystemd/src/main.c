#include "parse.h"
#include "demctl.h"

pthread_mutex_t recver_sync = PTHREAD_MUTEX_INITIALIZER;
struct Unit *head = NULL;

char const PATH_DEAMONS[] = "./deamons/";
char const CONFIG_FILE[] = "./deamons/config.json";
char const ZLOG_CONFIG_FILE[] = "zlog.conf";

int main (void)
{
    int ret = 0;

    if (0 != daemonize())
    {
        ret = -1;
        goto finally;
    }

    if (0 != init_log())
    {
        ret = -2;
        goto finally;
    }

    if (0 != read_config())
    {
        ret = -3;
        goto finally;
    }

    dem_control();

finally:
    dzlog_info("__________________end_logging__________________");
    zlog_fini();
    return ret;
}

