#include <stdio.h>
#include <unistd.h>

#include "funcs.h"
#include "demctl.h"
#include "ipc.h"

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include <string.h>

// #define SOCK_PATH "/tmp/demipc"

static void skeleton_daemon()
{
    pid_t pid;

    /*  Fork off the parent process */
    pid = fork();

    /*  An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /*  Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /*  On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /*  Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /*  Fork off for the second time*/
    pid = fork();

    /*  An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /*  Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /*  Set new file permissions */
    umask(0);

    /*  Change the working directory to the root directory */
    /*  or another appropriated directory */
    chdir("/");

    /*  Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /*  Open the log file */
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main ()
{
    /* printf("mysystemd\n");

    char *monitor = NULL;
    get_pointer_config(&monitor);
    int check = 0;
    check = supports_full_hd(monitor);
    printf("%d\n", check);

    execl("./deamons/exampled", "exampld", NULL);
    printf("asdf\n"); */


    /* printf("asdf\n");
    skeleton_daemon();

    while (1)
    {
        syslog (LOG_NOTICE, "First daemon started.");
        sleep (20);
        break;
    }

    syslog (LOG_NOTICE, "First daemon terminated.");
    closelog();

    return EXIT_SUCCESS; */


    pthread_t tid;
    pthread_create(&tid, NULL, &recver, NULL);
    pthread_join(tid, NULL);

}

