#include "../../mysystemd/include/shared_sync.h"
#include "sync.h"

static void* sender(void *arg);

int mysystemd_sync(char *argv[])
{
    pthread_t tid;
    int ret = 0;

    if (0 != pthread_create(&tid, NULL, &sender, (void *)argv))
    {
        ret = -1;
        goto finally;
    }

    if (0 != pthread_detach(tid))
    {
        ret = -2;
        goto finally;
    }

finally:
    return ret;
}

static void* sender(void *arg)
{
    char **tmp = (char **)arg;
    void *context = NULL;
    void *requester = NULL;
    char args[NAME_MAX] = { 0 };
    struct Message mes = { 0 };
    int i = 1;

    if (NULL == *tmp)
    {
        printf("error: null in arg");
        goto finally;
    }

    errno = 0;
    context = zmq_ctx_new ();
    if (NULL == context)
    {
        printf("error: zmq context not created (%s)", strerror(errno));
        goto finally;
    }

    errno = 0;
    requester = zmq_socket (context, ZMQ_DEALER);
    if (NULL == requester)
    {
        printf("error: zmq socket not created (%s)", strerror(errno));
        goto finally;
    }

    errno = 0;
    if (0 != zmq_connect(requester, PATH_IPC))
    {
        printf("error: zmq connection faild (%s)", strerror(errno));
        goto finally;
    }

    while (NULL != tmp[i])
    {
        snprintf(args + strlen(args), NAME_MAX - strlen(args), "%s ", tmp[i]);
        i++;
    }
    args[strlen(args) - 1] = 0;

    mes.type = INIT;
    mes.pid = getpid();

    snprintf(mes.name, NAME_MAX, "%s", tmp[0]);
    snprintf(mes.args, NAME_MAX, "%s", args);

    errno = 0;
    if (-1 == zmq_send(requester, &mes, sizeof(mes), 0))
    {
        printf("error: zmq send faild (%s)", strerror(errno));
    }

    mes.type = RUNNING;
    while (1)
    {
        sleep(DELAY);

        errno = 0;
        if (-1 == zmq_send(requester, &mes, sizeof(mes), 0))
        {
            printf("error: zmq send faild (%s)", strerror(errno));
        }
    }

finally:
    zmq_close(requester);
    zmq_ctx_destroy(context);
    pthread_exit(0);
}

