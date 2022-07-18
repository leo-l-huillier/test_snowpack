/*
** EPITECH PROJECT, 2022
** test_snowpack
** File description:
** Handle all communication with the client
*/
#include "include.h"

static client_t *get_name_client(client_t *cli, char *name, char *buff_out, gvs_t *gvs)
{
	if(recv(cli->sockfd, name, 32, 0) <= 0) {
		printf("Didn't enter the name.\n");
		gvs->leave_flag = 1;
	} else {
		strcpy(cli->name, name);
		sprintf(buff_out, "%s has joined\n", cli->name);
		printf("%s", buff_out);
		send_message(buff_out, cli->uid);
	}
	return cli;
}

static void handle_communication(client_t *cli, char *buff_out, gvs_t *gvs)
{
	while(1){
		if (gvs->leave_flag)
			break;
		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0){
			if(strlen(buff_out) > 0){
				send_message(buff_out, cli->uid);
				if (is_secret(buff_out) == 1) {
					str_trim_lf(buff_out, strlen(buff_out));
					printf("%s\n", buff_out);
				}
			}
		} else if (receive == 0) {
			sprintf(buff_out, "%s has left\n", cli->name);
			printf("%s", buff_out);
			send_message(buff_out, cli->uid);
			gvs->leave_flag = 1;
		} else {
			printf("ERROR: -1\n");
			gvs->leave_flag = 1;
		}
		bzero(buff_out, BUFFER_SZ);
	}
}

static void remove_client(client_t *cli, gvs_t *gvs)
{
	close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    gvs->cli_count--;
    pthread_detach(pthread_self());
}

void *handle_client(gvs_t *gvs)
{
	char buff_out[BUFFER_SZ];
	char name[32];
	
	gvs->cli_count++;

	// Name
	get_name_client(gvs->cli, name, buff_out, gvs);
	bzero(buff_out, BUFFER_SZ);
	handle_communication(gvs->cli, buff_out, gvs);
	remove_client(gvs->cli, gvs);
	return NULL;
}
