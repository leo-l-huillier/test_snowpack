/*
** technical test, 2022
** test_snowpack
** File description:
** the server side
*/
#include "include.h"

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Add clients to queue
void queue_add(client_t *cl)
{
	pthread_mutex_lock(&clients_mutex);
	for(int i = 0; i < MAX_CLIENTS; ++i) {
		if(!clients[i]) {
			clients[i] = cl;
			break;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

// Remove clients to queue
void queue_remove(int uid)
{
	pthread_mutex_lock(&clients_mutex);
	for(int i = 0; i < MAX_CLIENTS; ++i) {
		if(clients[i]) {
			if(clients[i]->uid == uid) {
				clients[i] = NULL;
				break;
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

//Send message to all clients except sender
void send_message(char *s, int uid)
{
	pthread_mutex_lock(&clients_mutex);
	for(int i = 0; i < MAX_CLIENTS; ++i) {
		if(clients[i]) {
			if(clients[i]->uid != uid) {
				if(write(clients[i]->sockfd, s, strlen(s)) < 0) {
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

struct sockaddr_in serv_addr_settings(struct sockaddr_in serv_addr)
{
	char *ip = "127.0.0.1";
	int port = 3000;

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);
	return serv_addr;
}

void init_struct_server(gvs_t *gvs)
{
	gvs->cli_count = 0;
    gvs->uid = 10;
    gvs->leave_flag = 0;
}

int server(void)
{
	int option = 1, listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t tid;

	//initialisation
	gvs_t *gvs = malloc(sizeof(gvs_t));
	init_struct_server(gvs);
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr = serv_addr_settings(serv_addr);

  /* Ignore pipe signals */
	//signal(SIGPIPE, SIG_IGN);

	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0) {
		perror("ERROR: setsockopt failed");
    	return EXIT_FAILURE;
	}
    if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    	perror("ERROR: Socket binding failed");
    	return EXIT_FAILURE;
    }
    if (listen(listenfd, 10) < 0) {
    	perror("ERROR: Socket listening failed");
    	return EXIT_FAILURE;
	}

	printf("WAITING FOR INCOMMING CONNECTION\n");

	while(1){
		socklen_t clilen = sizeof(cli_addr);
		connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &clilen);

		//Client settings
		gvs->cli = (client_t *)malloc(sizeof(client_t));
		gvs->cli->address = cli_addr;
		gvs->cli->sockfd = connfd;
		gvs->cli->uid = gvs->uid++;

		//Add client to the queue and fork thread
		queue_add(gvs->cli);
		pthread_create(&tid, NULL, &handle_client, (void*)gvs);

		// Reduce CPU usage
		sleep(1);
	}

	return EXIT_SUCCESS;
}
