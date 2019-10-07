#include "../../mysystemd/include/shared_ipc.h"
#include "ipc.h"

void mysystemd_sync (
    char *argv[])
{
    pthread_t tid;
    pthread_create(&tid, NULL, &sender, (void *)argv);
    pthread_join(tid, NULL);
}

void* sender(
    void *arg)
{
    char **tmp = (char **)arg;
    char name_with_args[NAME_MAX] = { 0 };

    // strcat(name_with_args, arg[0]);
    int i = 1;
    while (tmp[i] != NULL)
    {
        strcat(name_with_args, tmp[i]);
        strcat(name_with_args, " ");
        i++;
    }
    name_with_args[strlen(name_with_args)-1] = 0;

    printf("%s\n", name_with_args);

    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_DEALER);
    zmq_connect (requester, PATH_IPC);

    struct message mes;
    mes.type = init;
    mes.pid = getpid();
    // printf("%d\n", mes.pid);
    strcpy(mes.name, tmp[0]);
    strcpy(mes.args, name_with_args);
    // printf ("send init message\n\ttype = %d\n\tname = %s\n\n", mes.pid, mes.name);
    zmq_send (requester, &mes, sizeof(mes), 0);

    mes.type = running;
    while (1)
    {
        sleep(DELAY);
        // printf ("send message\n\tpid = %d\n\tname = %s\n\n", mes.pid, mes.name);
        zmq_send (requester, &mes, sizeof(mes), 0);
    }

    zmq_close (requester);
    zmq_ctx_destroy (context);
}

