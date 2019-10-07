#include "units.h"

void unit_prints(
    const struct unit *head)
{
    while (head)
    {
        printf("n:%s  ", head->name);
        printf("a:%s  ", head->args);
        printf("p:%d  ", head->pid);
        printf("s:%d  ", head->status);
        printf("f:%d  ", head->flag);
        printf("r:%d  ", head->non_respons);
        printf("\n");
        head = head->next;
    }
    printf("\n");
}

void unit_init_pid(
    struct unit *head,
    char const *name,
    char const *args,
    pid_t const pid)
{
    while (head)
    {
        char full_name[NAME_MAX];
        strcpy(full_name, PATH_DEAMONS);
        strcat(full_name, head->name);

        bool name_valid = strcmp(full_name, name); 
        bool args_valid = strcmp(head->args, args); 

        if ((name_valid || args_valid) == 0)
        {
            if (0 == head->pid)
            {
                head->pid = pid;
                dzlog_notice("deamon (%s '%s') new pid = %d",
                    head->name,
                    head->args,
                    head->pid);
                break;
            }
        }
        head = head->next;
    }
}

void unit_change_flag(
    struct unit *head,
    char const *name,
    char const *args,
    pid_t const pid)
{
    while (head)
    {
        char full_name[NAME_MAX] = { 0 };
        strcpy(full_name, PATH_DEAMONS);
        strcat(full_name, head->name);

        bool name_valid = strcmp(full_name, name); 
        bool args_valid = strcmp(head->args, args); 
        bool pid_valid = pid - head->pid; 

        if ((name_valid || args_valid || pid_valid) == 0)
        {
            // dzlog_info("%s (%d) found in list", name, pid);

            if (head->flag == no_answer)
            {
                head->flag = answer;
                head->non_respons = 0;
                // dzlog_info("%s (%d) flag changed", name, pid);
            }
            break;
        }
        head = head->next;
    }
}

void unit_update_non_response(
    struct unit *head)
{
    while (head)
    {
        if (head->flag == no_answer)
        {
            dzlog_warn("no response received answer (%s '%s') %d",
                    head->name,
                    head->args,
                    head->pid);
            head->non_respons++;
        }

        if (head->non_respons > LIMIT_NON_RESPONSE)
        {
            if (head->pid != 0)
            {
                if ((kill(head->pid, SIGKILL)))
                {
                    dzlog_notice("send SIGKILL to %d", head->pid);
                }
            }
            head->pid = 0;
            head->status = stopped;
        }

        head->flag = no_answer;

        head = head->next;
    }
}

int unit_new_element(
    struct unit **head,
    char const *name,
    char const *args)
{
    int ret = 0;

    if (valid_dem(name) != 1)
    {
        dzlog_debug("%s deamons is not valid", name);
        ret = -1;
        goto finally;
    }

    struct unit *tmp = (struct unit*) malloc(sizeof(struct unit));
    strcpy(tmp->name, name);
    strcpy(tmp->args, args);
    tmp->pid = 0;
    tmp->status = stopped;
    tmp->flag = no_answer;
    tmp->non_respons = 0;

    tmp->next = (*head);
    (*head) = tmp;

finally:
    return ret;
}

