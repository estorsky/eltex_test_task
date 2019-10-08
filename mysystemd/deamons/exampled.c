#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include "../../mysystemd_shared_lib/include/mysystemd_shared_lib.h"

int daemonize(void)
{
    pid_t pid;
    int ret = 0;

    pid = fork();

    if (pid < 0)
    {
        ret = -1;
        goto finally;
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0)
    {
        ret = -2;
        goto finally;
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0)
    {
        ret = -3;
        goto finally;
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    chdir("/");

    for (int i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
    {
        close (i);
    }

finally:
    return ret;
}

int main (int argc, char* argv[])
{
    daemonize();

    mysystemd_sync(argv);

    pause();

    return 0;
}

