/*
** EPITECH PROJECT, 2022
** test_snowpack
** File description:
** include.h
*/

#ifndef INCLUDE_H_
    #define INCLUDE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

//client side----------------------------------------------

#define LENGTH 2048

typedef struct global_values_client_s {
    bool is_connected;
    char *secret;
    int sockfd;
    char name[32];
} gvc_t;

int client(int argc, char **argv);
void recv_msg_handler(gvc_t *gvc);
void send_msg_handler(gvc_t *gvc);

//server side----------------------------------------------
int server(void);

//lib
void str_overwrite_stdout();
int my_strstr(char *str , char const *to_find);
void str_trim_lf (char* arr, int length);
int is_secret(char *secret);



#endif