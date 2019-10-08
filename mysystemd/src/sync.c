#include "../include/shared_sync.h"
#include "sync.h"

void *recver()
{
    dzlog_info("recver thread start");

    void *context = NULL;
    void *responder = NULL;
    Message mes = { 0 };

    errno = 0;
    context = zmq_ctx_new();
    if (NULL == context)
    {
        dzlog_error("zmq context not created: %s", strerror(errno));
        goto finally;
    }

    errno = 0;
    responder = zmq_socket(context, ZMQ_DEALER);
    if (NULL == responder)
    {
        dzlog_error("zmq socket not created: %s", strerror(errno));
        goto finally;
    }

    errno = 0;
    if (0 != zmq_bind(responder, PATH_IPC))
    {
        dzlog_error("zmq bind error: %s", strerror(errno));
        goto finally;
    }

    while (1)
    {
        errno = 0;
        if (-1 == zmq_recv(responder, &mes, sizeof(mes), 0))
        {
            dzlog_error("zmq recv error: %s", strerror(errno));
        }

        pthread_mutex_lock(&recver_sync);

        if (INIT == mes.type)
        {
            if (0 != unit_init_pid(mes.name, mes.args, mes.pid))
            {
                dzlog_error("init pid faild");
            }
        }

        if (0 != unit_change_answer(mes.name, mes.args, mes.pid))
        {
            dzlog_error("change answer faild");
        }

        pthread_mutex_unlock(&recver_sync);
    }

finally:
    zmq_ctx_destroy(context);
    zmq_close(responder);
    pthread_exit(0);
}

