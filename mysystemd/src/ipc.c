#include "ipc.h"

void *recver(
        void *const arg) //
{
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_DEALER);
    struct message mes;

    zmq_bind (responder, "ipc:///tmp/demipc");

    int num = 0;
    while (1) {
        zmq_recv (responder, &mes, sizeof(mes), 0);
        printf ("Received message %d:\n\ttype = %d\n\tpid = %d\n\tname_dem = %s\n\n", num, mes.type, mes.pid, mes.name_dem);
        num++;
    }

    zmq_ctx_destroy(context);
    zmq_close(responder);

// finally:
    // pthread_exit(NULL);
}
