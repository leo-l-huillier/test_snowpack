/*
** EPITECH PROJECT, 2022
** test_snowpack
** File description:
** client.c
*/

#include "include.h"


// Global variables
volatile sig_atomic_t flag = 0;

int get_name(gvc_t *gvc)
{
	printf("Please enter your name: ");
  	fgets(gvc->name, 32, stdin);
  	str_trim_lf(gvc->name, strlen(gvc->name));

	if (strlen(gvc->name) > 32){
		printf("Name must be less than 30 characters.\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int client(int argc, char **argv)
{
	if (argc < 3) {
		printf("to few arguments\n");
		return EXIT_FAILURE;
	}
	gvc_t *gvc = malloc(sizeof(gvc_t));
	gvc->is_connected = false;
	gvc->secret = argv[2];
	gvc->sockfd = 0;
	char *ip = "127.0.0.1";
	int port = 3000;

	get_name(gvc);

	struct sockaddr_in server_addr;

	/* Socket settings */
	gvc->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);


    // Connect to Server
    if (connect(gvc->sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	// Send name
	send(gvc->sockfd, gvc->name, 32, 0);

	printf("=== WAINTING ANOTHER USER WITH THE SAME SECRET===\n");

	pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, (void*)gvc) != 0){
		printf("ERROR: pthread\n");
    return EXIT_FAILURE;
	}

	pthread_t recv_msg_thread;
    if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, (void*)gvc) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	while (1){
		if(flag){
			printf("\nBye\n");
			break;
    }
	}

	close(gvc->sockfd);

	return EXIT_SUCCESS;
}
