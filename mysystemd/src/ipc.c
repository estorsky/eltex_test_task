#include "../include/shared_ipc.h"
#include "ipc.h"

void *recver()
{
    dzlog_info("recver thread start");
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_DEALER);
    struct message mes;

    zmq_bind (responder, PATH_IPC);

    while (1)
    {
        zmq_recv (responder, &mes, sizeof(mes), 0);
        /* printf ("received message:\n"
                "\ttype  = %d\n"
                "\tpid   = %d\n"
                "\tname  = %s\n"
                "\targs = %s\n",
                mes.type, mes.pid, mes.name, mes.args); */

        /* dzlog_notice("received message:  "
                "type = '%d'  "
                "pid = '%d'  "
                "name = '%s'  "
                "args = '%s'",
                mes.type, mes.pid, mes.name, mes.args); */

        pthread_mutex_lock(&recver_sync);

        if (mes.type == init)
        {
            unit_init_pid(head, mes.name, mes.args, mes.pid);
        }
        unit_change_flag(head, mes.name, mes.args, mes.pid);
        // unit_prints(head);

        pthread_mutex_unlock(&recver_sync);
    }

    zmq_ctx_destroy(context);
    zmq_close(responder);
}

