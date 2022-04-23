#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "helper.h"


void log_to_file(char *text,int client_id){
    FILE *f = fopen("log.txt","a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(f,"%d-%02d-%02d %02d:%02d:%02d | %d | %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, client_id, text);
    fclose(f);
}

int next_client_id = 0;

int clients_queue_id_arr[MAX_CLIENTS_NUMBER];

int actual_usr_id = 0;

int active_users_counter = 0;

int is_server_running = 1;

int user_exists(int user_id) {
    if (user_id > next_client_id)
        return 0;

    return clients_queue_id_arr[user_id] != -1;
}

int user_queue_exists(int userQueueId) {
    for (int i = 0; i < next_client_id; i++) {
        if (clients_queue_id_arr[i] == userQueueId)
            return 1;
    }

    return 0;
}

void prepare_message(struct msg* input, struct msg* output) {
    __time_t now;
    time(&now);

    char date[21];
    strftime(date, 21, "%d-%m-%Y_%H:%M:%S", localtime(&now));

    sprintf(output->msg_text.buf, "from %d, %s - %s", actual_usr_id,
            input->msg_text.buf, date);
}

void send_shutdown_to_all_clients() {
    struct msg msg;

    msg.msg_text.id = SERVER_ID;
    msg.msg_type = SHUTDOWN;

    sprintf(msg.msg_text.buf, "STOP");

    for (int i = 0; i < next_client_id; i++) {
        if (!user_exists(i))
            continue;

        if (msgsnd(clients_queue_id_arr[i], &msg, sizeof(struct msg_text), 0) ==
            -1) {
            print_error("Server: ERROR while sending data!");

        } else {
            print_some_info("Server: SHUTDOWN signal sent");
        }
    }
}

int send_message(int id, struct msg* output, int type) {
    if (user_exists(id) && msgsnd(clients_queue_id_arr[id], output,
                                  sizeof(struct msg_text), 0) == -1) {
        print_error("Server: ERROR while sending data!");
        return -1;

    } else {
        if (type == 0) {
            printf("\033[1;32mServer:\033[0m send response to client - %d.\n\n",
                   actual_usr_id);
        } else {
            printf(
                "\033[1;32mServer:\033[0m send message to client %d from %d.\n",
                id, actual_usr_id);
        }
        return 1;
    }
}

int get_free_index() {
    if (next_client_id < MAX_CLIENTS_NUMBER) {
        ++next_client_id;
        return next_client_id - 1;

    } else {
        for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
            if (clients_queue_id_arr[i] == -1)
                return i;
        }
    }

    return -1;
}

void handle_SIGINT(int signal_num) {
    print_some_info("Server: Signal SIGINT received");

    if (active_users_counter > 0) {
        send_shutdown_to_all_clients();
    } else {
        print_sth_and_exit("Closing server!", 0);
    }
}

void stop_command(struct msg* input, struct msg* output) {
    // remove queue

    clients_queue_id_arr[actual_usr_id] = -1;

    log_to_file("STOPPED",actual_usr_id);

    sprintf(output->msg_text.buf, "STOP - user is removed.");

    --active_users_counter;

    if (active_users_counter == 0) {
        is_server_running = 0;
    }
}


void list_command(struct msg* input, struct msg* output) {
    char item[10];

    log_to_file("LISTED",actual_usr_id);

    int offset = 0;

    for (int i = 0; i < next_client_id; i++) {
        if (i != next_client_id - 1)
            sprintf(item, "%d, ", i);

        else
            sprintf(item, "%d", i);

        memcpy(output->msg_text.buf + offset, item,
               strlen(item) * sizeof(char));

        offset += strlen(item);
    }

    *(output->msg_text.buf + offset) = '\0';
}

void init_command(struct msg* input, struct msg* output) {
    int user_queue_id = strtol(input->msg_text.buf, NULL, 0);

    log_to_file("INIT",user_queue_id);

    if (user_queue_exists(user_queue_id)) {
        sprintf(output->msg_text.buf, "INIT - User already exist.");
        output->msg_type = -1;

    } else {
        int index = get_free_index();

        if (index != -1) {
            sprintf(output->msg_text.buf, "%d", index);

            clients_queue_id_arr[index] = user_queue_id;

            actual_usr_id = index;

            output->msg_type = index;

            active_users_counter++;

        } else {
            sprintf(output->msg_text.buf, "INIT - Too many clients.");

            output->msg_type = -1;
        }
    }
}

void _2all_command(struct msg* input, struct msg* output) {
    prepare_message(input, output);

    char buff[256] = "2ALL ";
    strcat(buff,output->msg_text.buf);
    log_to_file(buff,actual_usr_id);

    int sent_counter = 0;

    for (int i = 0; i < next_client_id; i++) {
        if (i != actual_usr_id && send_message(i, output, 1))
            ++sent_counter;
    }

    sprintf(output->msg_text.buf, "2ALL - Send %d/%d message", sent_counter,
            next_client_id - 1);
}

void _2one_command(struct msg* input, struct msg* output) {
    prepare_message(input, output);

    char s[5];
    sprintf(s,"%d",input->msg_text.additional_arg);

    char buff[256] = "2ONE ";
    strcat(buff,s);
    strcat(buff," ");
    strcat(buff,output->msg_text.buf);
    log_to_file(buff,actual_usr_id);

    if (!user_exists(input->msg_text.additional_arg)) {
        sprintf(output->msg_text.buf,
                "2ONE - Destination user (%d) DOES NOT exist",
                input->msg_text.additional_arg);

    } else {
        send_message(input->msg_text.additional_arg, output, 1);

        sprintf(output->msg_text.buf, "2ONE - OK, send message");
    }
}

void execute_command(struct msg* input, struct msg* output) {
    actual_usr_id = input->msg_text.id - SHIFT_ID;

    if (!user_exists(actual_usr_id) && input->msg_type != INIT) {
        sprintf(output->msg_text.buf, "User DOES NOT exist.");

        output->msg_text.id = SERVER_ID;
        output->msg_type = ERROR;

        return;
    }

    switch (input->msg_type) {
        case STOP: {
            stop_command(input, output);
        } break;

        case LIST: {
            list_command(input, output);
        } break;

        case INIT: {
            init_command(input, output);
        } break;

        case _2ALL: {
            _2all_command(input, output);
        } break;

        case _2ONE: {
            _2one_command(input, output);
        } break;

        default:
            break;
    }

    output->msg_text.id = SERVER_ID;

    output->msg_type = actual_usr_id + SHIFT_ID;
}


int main(int argc, char* argv[], char* env[]) {
    char* homedir = getenv("HOME");

    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        clients_queue_id_arr[i] = -1;
    }

    key_t msg_queue_key;

    int qid;

    struct msg message, response;

    if ((msg_queue_key = ftok(homedir, PROJ_ID)) == (key_t)-1) {
        print_sth_and_exit("Server: ERROR while getting key using ftok", 15);
    }

    if ((qid = msgget(msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
        print_sth_and_exit("Server: ERROR while creating a queue!", 16);
    }

    struct sigaction action;

    action.sa_handler = handle_SIGINT;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);

    action.sa_flags = 0;

    sigaction(SIGINT, &action, NULL);

    print_some_info("Server is running!");

    while (is_server_running) {
        if (msgrcv(qid, &message, sizeof(struct msg_text), -100, 0) == -1) {
            if (EINTR != errno) {  // ignore interrupting by SIGINT
                print_error("Server: ERROR while reading input data!");
            }

            continue;

        } else {
            printf(
                    "\033[1;32mServer:\033[0m message received:\n\ttype: %s, id: "
                    "%d, "
                    "message: %s \n",
                    type_to_string(message.msg_type), message.msg_text.id,
                    message.msg_text.buf);

            execute_command(&message, &response);
        }

        if (message.msg_type == STOP)
            continue;

        send_message(actual_usr_id, &response, 0);
    }


    // end working of server
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        printf("\033[1;32mServer:\033[0m Error while closing client queue.\n");
    }

    printf("\033[1;32mServer:\033[0m Server close.\n");

    return 0;
}
