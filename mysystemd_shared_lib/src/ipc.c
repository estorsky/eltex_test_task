#include "ipc.h"
#include <unistd.h>
#include <sys/types.h>

void *sender(
        void *const arg) //
{
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_DEALER);
    zmq_connect (requester, "ipc:///tmp/demipc");

    struct message mes;
    mes.type = init;
    mes.pid = getpid();
    printf("%d\n", mes.pid);
    sprintf(mes.name_dem, "%s", "test");
    printf ("send init message\n\ttype = %d\n\tname_dem = %s\n\n", mes.type, mes.name_dem);
    zmq_send (requester, &mes, sizeof(mes), 0);

    int request_nbr;
    while (1)
    {
        sleep(5);
        mes.type = running;
        printf ("send message %d\n\ttype = %d\n\tname_dem = %s\n\n", request_nbr, mes.type, mes.name_dem);
        zmq_send (requester, &mes, sizeof(mes), 0);
        request_nbr++;
    }

    zmq_close (requester);
    zmq_ctx_destroy (context);

// finally:
    // pthread_exit(NULL);
}

void mysystemd_sync ()
{
    pthread_t tid;
    pthread_create(&tid, NULL, &sender, NULL);
    pthread_join(tid, NULL);
}

