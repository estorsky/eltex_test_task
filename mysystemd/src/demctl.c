#include "demctl.h"

void dem_control(void)
{
    pthread_t tid;

    if (0 > init_deamons())
    {
        dzlog_error("init deamons faild");
        goto finally;
    }

    errno = 0;
    if (0 != pthread_create(&tid, NULL, &recver, NULL))
    {
        dzlog_error("pthread_create: %s", strerror(errno));
    }

    while (1)
    {
        unit_prints();

        sleep(DELAY_ROUND_SEC);

        pthread_mutex_lock(&recver_sync);

        if (0 != unit_update_non_response())
        {
            dzlog_error("update non response faild");
        }

        if (0 > init_deamons())
        {
            dzlog_error("init deamons faild");
        }

        pthread_mutex_unlock(&recver_sync);
    }

    pthread_join(tid, NULL);

finally:
    pthread_exit(0);
}

int init_deamons(void)
{
    struct Unit *deamon = NULL;
    char cmd [PATH_MAX] = { 0 };
    int ret = 0;

    deamon = head;

    if (NULL == deamon)
    {
        ret = -1;
        goto finally;
    }

    while (deamon)
    {
        if (STARTED != deamon->status)
        {
            snprintf(cmd, PATH_MAX,"%s%s %s", PATH_DEAMONS,  deamon->name, deamon->args);

            if (0 == start_dem(cmd))
            {
                dzlog_info("deamon '%s' '%s' start",
                    deamon->name,
                    deamon->args);
                ret++;
            } 
            else
            {
                dzlog_error("deamon '%s' '%s' not start",
                    deamon->name,
                    deamon->args);
            }

            deamon->status = STARTED;
            deamon->non_respons = 0;
        }
        deamon = deamon->next;
    }

finally:
    return ret;
}

int start_dem(const char *cmd)
{
    pid_t pid = -1;
    int ret = 0;
    const char *sh = "/bin/sh";
    char *const argv[] = { (char *const)sh,
        (char *const)"-c",
        (char *const)cmd,
        (char *const)NULL };

    pid = vfork();
    if (pid < 0)
    {
        ret = -1;
        goto finally;
    }

    if (pid == 0)
    {
        execv(sh, argv);
        _exit(127);
    }

finally:
    return ret;
}

int valid_dem_unsafe(const char *name)
{
    char path[PATH_MAX] = { 0 };

    snprintf(path, PATH_MAX, "%s%s", PATH_DEAMONS, name);

    return valid_file(path);
}

int valid_file(const char *path)
{
    struct stat st = { 0 };
    int ret = 0;

    if (NULL == path)
    {
        ret = -1;
        goto finally;
    }

    if (stat(path, &st) < 0)
    {
        ret = -2;
        goto finally;
    }

    if (1 == S_ISREG(st.st_mode))
    {
        ret = 0;
    }

finally:
    return ret;
}

int init_log(void)
{
    int ret = 0;

    if (-1 == dzlog_init(ZLOG_CONFIG_FILE, "my_cat"))
    {
        ret = -1;
        goto finally;
    }

    dzlog_info("__________________start_logging__________________");

finally:
    return ret;
}

int daemonize(void)
{
    pid_t pid;
    int ret = 0;

    pid = fork();

    if (pid < 0)
    {
        dzlog_error("fork doesnt work");
        ret = -1;
        goto finally;
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0)
    {
        dzlog_error("fork doesnt work");
        ret = -2;
        goto finally;
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();

    if (pid < 0)
    {
        dzlog_error("second fork doesnt work");
        ret = -3;
        goto finally;
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    // chdir("/");

    for (int i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
    {
        close (i);
    }

finally:
    return ret;
}

