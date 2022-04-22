#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils/message.h"
#include "utils/types.h"
#include "que/que.h"
#include "utils/utils.h"
#include "keygen/keygen.h"

int server_queue, private_queue;
int friend_queue = -1;
int id;

void send_to_server(message_t *message)
{
    if (send(server_queue, message))
    {
        perror("unable to send message to server");
    }
}

void clean()
{
    message_t message;
    message.type = TYPE_STOP;
    message.id = id;

    send_to_server(&message);
    close_queue(server_queue);
    delete_queue(private_queue, get_private_key());

}

void handle_sigint(int sig)
{
    exit(0);
}

void hanlde_stop()
{
    kill(getpid(), SIGINT);
}

void handle_list()
{
    message_t message;
    message.type = TYPE_LIST;
    message.id = id;
    send_to_server(&message);
}

void handle_2ONE_send(int receiverID,char* mess) // TODO
{
    message_t message;
    message.type = TYPE_LIST;
    message.id = id;
    send_to_server(&message);
}

void handle_2ALL_send(char* mess) // TODO
{
    message_t message;
    message.type = TYPE_LIST;
    message.id = id;
    send_to_server(&message);
}

void sender_handle_line(char *command, char *rest)
{

    if (strcmp("STOP", command) == 0)
    {
        hanlde_stop();
    }
    else if (strcmp("LIST", command) == 0)
    {
        handle_list();
    }
    else if (strcmp("2ONE", command) == 0)
    {
        int tmp = 0; //TODO FIX
        handle_2ONE_send(tmp,rest);
    }
    else if (strcmp("2ALL", command) == 0)
    {
        handle_2ALL_send(rest);
    }
}

void initialize()
{
    key_t key;
    if ((key = get_public_key()) == -1)
    {
        perror("Can't generate public key");
    }
    if ((server_queue = get_queue(key)) == -1)
    {
        perror("cant open server queue");
        exit(1);
    }
    key_t private_key = get_private_key();
    if ((private_queue = create_queue(private_key)) == -1)
    {
        perror("cant create private queue");
        exit(1);
    }

    message_t message;
    message.type = TYPE_INIT;
    sprintf(message.text, "%d", private_key);
    message.pid = getpid();

    if (send(server_queue, &message) == -1)
    {
        perror("unable to register");
        exit(1);
    }

    if (receive(private_queue, &message) == -1)
    {
        perror("unable to register");
        exit(1);
    }

    id = message.id;
    printf("successfully registered with id %d\n", id);
}

void sender()
{
    char line[256];
    char command[256];
    char rest[256];

    fgets(line, 1024, stdin);
    separate_command(line, command, rest);
    sender_handle_line(command, rest);
}

void handle_message(message_t *message)
{
    printf("_________\n");
    char mess[256];
    sprintf(mess, "%s", message->text);
    printf("F: %s\n", mess);
    printf("_________\n");
}

void catcher()
{

    signal(SIGINT, handle_sigint);

    while (!is_empty(private_queue))
    {
        message_t message;

        if (receive_no_wait(private_queue, &message) != -1)
        {

            switch (message.type)
            {
            case TYPE_STOP:
                printf("exiting\n");
                exit(0);
                break;
            case TYPE_2ONE:
                printf("TYPE_2ONE received\n");
                break;
            case TYPE_2ALL:
                printf("TYPE_2ALL received\n");
                break;
            default:
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    atexit(clean);
    signal(SOMETHING_HAPPEND, catcher);
    signal(SIGINT, handle_sigint);
    initialize();
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = catcher;
    sigaction(SOMETHING_HAPPEND, &sa, NULL);
    while (1)
    {
        sender();
        catcher();
    }

    return 0;
}