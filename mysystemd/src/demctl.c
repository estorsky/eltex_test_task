#include "demctl.h"

extern struct unit *head;

void dem_control(
    void)
{
    init_deamons(head);

    pthread_t tid;
    pthread_create(&tid, NULL, &recver, NULL);

    while (1)
    {
        // dzlog_info("round iteration");
        sleep(DELAY_ROUND_SEC);

        pthread_mutex_lock(&recver_sync);

        // unit_prints(head);
        unit_update_non_response(head);
        init_deamons(head);

        pthread_mutex_unlock(&recver_sync);
    }

    pthread_join(tid, NULL);
}

int init_deamons(
    struct unit *head)
{
    char cmd [NAME_MAX] = { 0 };
    int ret = 0;

    while (head)
    {
        if (head->status != started)
        {
            strcpy(cmd, PATH_DEAMONS);
            strcat(cmd, head->name);
            strcat(cmd, " ");
            strcat(cmd, head->args);
            // printf("%s\n", cmd);
            if (system_vfork(cmd))
            {
                dzlog_info("deamon '%s' '%s' start",
                    head->name,
                    head->args);
                ret++;
            } 
            else
            {
                dzlog_info("deamon '%s' '%s' not start",
                    head->name,
                    head->args);
            }

            head->status = started;
            head->non_respons = 0;
        }
        head = head->next;
    }

    return ret;
}

int system_vfork(
    const char *cmd)
{
    pid_t pid = -1;
    pid_t pid_wait = -1;
    int ret = 0;
    const char *sh = "/bin/sh";
    char *const argv[] = { (char *const)sh, (char *const)"-c", (char *const)cmd, (char *const)NULL };

    pid = vfork();
    if (pid < 0)
    {
        ret = -1;
        goto exit;
    }

    if (pid == 0)
    {
        execv(sh, argv);
        _exit(127);
    }

    do
    {
        pid_wait = waitpid(pid, &ret, 0);
        if (pid_wait <= -1 && errno != EINTR)
        {
            ret = -1;
            break;
        }
    } while (pid_wait != pid);

exit:
    return ret;
}

int valid_dem(
    const char *name)
{
    char path[PATH_MAX] = { 0 };
    strcpy(path, PATH_DEAMONS);
    strcat(path, name);

    // printf("%s\n", path);

    return valid_file(path);
}

int valid_file(
    const char *path)
{
    struct stat st;

    if (stat(path, &st) < 0)
        return -1;

    return S_ISREG(st.st_mode);
}

int init_log(void)
{
    int ret = 0;

    int rc;
    rc = dzlog_init("zlog.conf", "my_cat");

    if (rc) {
        printf("init failed\n");
        ret = -1;
        goto finally;
    }

    dzlog_info("__________________start_logging__________________");

    // dzlog_info("hello, zlog");
    // dzlog_error("hello, zlog");
    // dzlog_warn("hello, zlog");
    // dzlog_notice("hello, zlog");
    // dzlog_debug("hello, zlog");

    // zlog_fini();

finally:
    return ret;
}

void daemonize(
    void)
{
    pid_t pid;

    /*  Fork off the parent process */
    pid = fork();

    /*  An error occurred */
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /*  Success: Let the parent terminate */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    /*  On success: The child process becomes session leader */
    if (setsid() < 0)
    {
        exit(EXIT_FAILURE);
    }

    /*  Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /*  Fork off for the second time*/
    pid = fork();

    /*  An error occurred */
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /*  Success: Let the parent terminate */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    /*  Set new file permissions */
    umask(0);

    /*  Change the working directory to the root directory */
    /*  or another appropriated directory */
    // chdir("/");

    /*  Close all open file descriptors */
    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /*  Open the log file */
    // openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

