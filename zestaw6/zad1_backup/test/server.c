#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "utils/message.h"
#include "utils/types.h"
#include "que/que.h"
#include "utils/utils.h"
#include "keygen/keygen.h"

#define MAX_CLIENTS 128

int queue;

int clients[MAX_CLIENTS][2];

int num_clients = 0;
int clients_tab_size = 0;

int stops = 0;

void clean()
{
    key_t key;
    if ((key = get_public_key()) == -1)
    {
        perror("Can't generate public key");
    }
    delete_queue(queue, key);
}

void send_private(unsigned int client_id, message_t *message)
{
    if (client_id >= clients_tab_size || clients[client_id][0] == -1)
    {
        fprintf(stderr, "wrong client!\n");
        return;
    }
    if ((send(clients[client_id][0], message) == -1))
    {
        perror("unable to send private message");
    }
}

void handle_sigint(int sig)
{
    stops = 1;
    if (num_clients == 0)
    {
        exit(0);
    }

    message_t message;
    message.type = TYPE_STOP;
    for (int i = 0; i < clients_tab_size; ++i)
    {
        if (clients[i][0] != -1)
        {
            send_private(i, &message);
            kill(clients[i][1], SOMETHING_HAPPEND);
        }
    }
}

int find_client_id()
{

    if (clients_tab_size < MAX_CLIENTS)
    {
        num_clients++;
        return clients_tab_size++;
    }
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i][0] == -1)
        {
            num_clients++;
            return i;
        }
    }

    return -1;
}

void handle_init(message_t *msg)
{
    key_t key;
    sscanf(msg->text, "%d", &key);
    int pid = msg->pid;

    int cl_id = find_client_id();
    if (cl_id != -1)
    {
        if ((clients[cl_id][0] = get_queue(key)) == -1)
        {

            perror("cant open client private queue");
        }

        clients[cl_id][1] = pid;

        message_t message;
        message.type = TYPE_INIT;
        message.id = cl_id;

        send_private(cl_id, &message);

        printf("sent register confirmation for client: %d\n", cl_id);
    }
    else
    {
        fprintf(stderr, "clients full error\n");
    }
    FILE *ptr = fopen("logs.txt.txt","a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(ptr,"logs.txt: %d-%02d-%02d %02d:%02d:%02d %d INIT\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, msg->id);
    fclose(ptr);
}

void init()
{
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        clients[i][0] = -1;
    }
}

void handle_stop(message_t *message)
{
    int client_id = message->id;
    if (close_queue(clients[client_id][0]) == -1)
    {
        perror("cant close client queue\n");
    }
    clients[client_id][0] = -1;
    num_clients--;
    printf("client: %d stops, clients left: %d\n", client_id, num_clients);
}

void handle_list(message_t *message)
{
    int client_id = message->id;
    printf("\n------------------------------\n");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if(clients[i][0] != -1){
            if(client_id == i){
                printf("List caller: id %d\n",i);
            }else{
                printf("id %d\n",i);
            }
        }

    }
    printf("------------------------------\n");
}

void handle_2ALL(message_t *message)
{
    int client_id = message->id;
    printf("2ALL\n%s\n------------------------------\n",message->text);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if(clients[i][0] != -1 && client_id != i){
            send_private(i,message);
            printf("%d\n",i);
        }

    }
    printf("------------------------------\n");
}

void handle_2ONE(message_t *message)
{
    int client_id = message->id;
    printf("2ONE\n%s\n------------------------------\n",message->text);
    send_private(client_id,message);
    printf("------------------------------\n");
}

int main(int argc, char *argv[])
{

    init();
    atexit(clean);

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handle_sigint;

    sigaction(SIGINT, &sa, NULL);

    key_t key = get_public_key();
    if ((queue = create_queue(key)) == -1)
    {
        perror("cant create queue");
        exit(1);
    }

    message_t message;

    while (1)
    {
        if (receive(queue, &message) == -1)
        {
            printf("cant receive\n");
            if (errno != EINTR)
            {
                perror("cant receive message");
                exit(1);
            }

            continue;
        }
        switch (message.type)
        {

        case TYPE_STOP:
        {
            handle_stop(&message);
            break;
        }

        case TYPE_LIST:
        {
            handle_list(&message);
            break;
        }
        case TYPE_INIT:
        {
            handle_init(&message);
            break;
        }
            case TYPE_2ALL:
            {
                handle_2ALL(&message);
                break;
            }

        default:
        {
            fprintf(stderr, "wrong type\n");
            break;
        }
        }
    }

    return 0;
}