/*
** EPITECH PROJECT, 2022
** test_snowpack
** File description:
** send_and_receive.c
*/
#include "include.h"

void send_msg_handler(gvc_t *gvc)
{
    char message[LENGTH] = {};
	char buffer[LENGTH + 32] = {};

    while(1) {

  	str_overwrite_stdout();
    fgets(message, LENGTH, stdin);
    str_trim_lf(message, LENGTH);
    
    if (strcmp(message, "exit") == 0)
			break;
    else {
      sprintf(buffer, "%s: %s\n", gvc->name, message);
      send(gvc->sockfd, buffer, strlen(buffer), 0);
    }
	bzero(message, LENGTH);
    bzero(buffer, LENGTH + 32);
  }
}

static void wait_for_connection(char *message, gvc_t *gvc)
{
	if (gvc->is_connected == false) {
        for (int i = 0; strcmp(message, gvc->secret) != 0; i++) {
			bzero(message, strlen(message));
            if (i == 30) {
                printf("nobody was connected with the same secret\n");
                exit(1);
            }
            int receive = recv(gvc->sockfd, message, LENGTH, MSG_DONTWAIT);
            send(gvc->sockfd, gvc->secret, strlen(gvc->secret), 0);
            sleep(1);
        }
        gvc->is_connected = true;
		printf("successfully connected to an other uther");
        str_overwrite_stdout();
        sleep(1);
		send(gvc->sockfd, gvc->secret, strlen(gvc->secret), 0);
    }
}

void recv_msg_handler(gvc_t *gvc)
{
	char message[LENGTH] = {};
	const void *secret_send = gvc->secret;

  while (1) {
	int receive = recv(gvc->sockfd, message, LENGTH, MSG_DONTWAIT);
	wait_for_connection(message, gvc);
    if (receive > 0 && is_secret(message) == 1) {
		if (my_strstr(message, "[CMD]ECHOREPLY snowpack") == 0) {
			const void *response = "snowpack\n";
			send(gvc->sockfd, response, strlen(response), 0);
		}
		printf("%s", message);
      	str_overwrite_stdout();
    } else if (receive == 0)
			break;
	memset(message, 0, sizeof(message));
    }
}
