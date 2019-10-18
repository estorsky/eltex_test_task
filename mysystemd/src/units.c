#include "units.h"

static bool valid_name_and_args(
    char const *name_first,
    char const *name_second,
    char const *args_first,
    char const *args_second);

void unit_prints(void)
{
    struct Unit *deamon = NULL;

    deamon = head;

    while (deamon)
    {
        printf("n:%s  ", deamon->name);
        printf("a:%s  ", deamon->args);
        printf("p:%d  ", deamon->pid);
        printf("s:%d  ", deamon->status);
        printf("f:%d  ", deamon->answer);
        printf("r:%d  ", deamon->non_respons);
        printf("\n");
        deamon = deamon->next;
    }
    printf("\n");
}

int unit_init_pid(
    char const *name,
    char const *args,
    pid_t const pid)
{
    struct Unit *deamon = NULL;
    int ret = 0;

    deamon = head;

    if ( NULL == deamon || NULL == name || NULL == args)
    {
        ret = -1;
        goto finally;
    }

    while (deamon)
    {
        char full_name[NAME_MAX] = { 0 };

        snprintf(full_name, NAME_MAX, "%s%s", PATH_DEAMONS, deamon->name);

        if (valid_name_and_args(full_name, name, deamon->args, args))
        {
            if (0 == deamon->pid)
            {
                deamon->pid = pid;
                dzlog_notice("deamon (%s '%s') new pid %d",
                    deamon->name,
                    deamon->args,
                    deamon->pid);
                break;
            }
        }
        deamon = deamon->next;
    }

finally:
    return ret;
}

int unit_change_answer(
    char const *name,
    char const *args,
    pid_t const pid)
{
    struct Unit *deamon = NULL;
    int ret = 0;

    deamon = head;

    if ( NULL == deamon || NULL == name || NULL == args)
    {
        ret = -1;
        goto finally;
    }

    while (deamon)
    {
        char full_name[NAME_MAX] = { 0 };

        snprintf(full_name, NAME_MAX, "%s%s", PATH_DEAMONS, deamon->name);

        if (valid_name_and_args(full_name, name, deamon->args, args)
            && (pid == deamon->pid))
        {
            if (NOT_RECV == deamon->answer)
            {
                deamon->answer = RECV;
                deamon->non_respons = 0;
            }
            break;
        }

        deamon = deamon->next;
    }

finally:
    return ret;
}

int unit_update_non_response(void)
{
    struct Unit *deamon = NULL;
    int ret = 0;
    errno = 0;

    deamon = head;

    if ( NULL == deamon)
    {
        ret = -1;
        goto finally;
    }

    while (deamon)
    {
        if (NOT_RECV == deamon->answer)
        {
            dzlog_warn("no response received answer (%s '%s') %d",
                    deamon->name,
                    deamon->args,
                    deamon->pid);
            deamon->non_respons++;
        }

        if (deamon->non_respons > LIMIT_NON_RESPONSE)
        {
            if (0 != deamon->pid)
            {
                errno = 0;
                if (0 != kill(deamon->pid, SIGKILL))
                {
                    dzlog_notice("error kill %d: %s",
                        deamon->pid,
                        strerror(errno));
                }
                else
                {
                    dzlog_notice("send SIGKILL successfull to %d", deamon->pid);
                }
            }

            deamon->pid = 0;
            deamon->status = STOPPED;
        }

        deamon->answer = NOT_RECV;

        deamon = deamon->next;
    }

finally:
    return ret;
}

int unit_new_element(
    char const *name,
    char const *args)
{
    struct Unit *tmp = NULL;
    int ret = 0;

    if ( NULL == name || NULL == args)
    {
        ret = -1;
        goto finally;
    }

    tmp = malloc(sizeof(struct Unit));
    if (NULL == tmp)
    {
        ret = -3;
        goto finally;
    }

    snprintf(tmp->name, NAME_MAX, "%s", name);
    snprintf(tmp->args, NAME_MAX, "%s", args);
    tmp->pid = 0;
    tmp->status = STOPPED;
    tmp->answer = NOT_RECV;
    tmp->non_respons = 0;

    tmp->next = head;
    head = tmp;

finally:
    return ret;
}

#ifndef TESTING
static bool valid_name_and_args(
    char const *name_first,
    char const *name_second,
    char const *args_first,
    char const *args_second)
{
    bool name_valid = true;
    bool args_valid = true;
    bool ret = false;

    if ( NULL == name_first
        || NULL == name_second
        || NULL == args_first
        || NULL == args_second)
    {
        ret = false;
        goto finally;
    }

    // strncmp return 0 (false) if strings equal, therefore invert bool var
    name_valid = strncmp(name_first, name_second, NAME_MAX);
    args_valid = strncmp(args_first, args_second, NAME_MAX);

    if (false == (name_valid || args_valid))
    {
        ret = true;
    }

finally:
    return ret;
}
#endif

