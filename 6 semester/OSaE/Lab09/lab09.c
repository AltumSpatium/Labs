#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


#define BUFFER_SIZE 1024


int main (int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s [port]\n", argv[0]);
	}
	char buf[BUFFER_SIZE];
	int port = atoi(argv[1]);
	int server_fd, client_fd, err;
	struct sockaddr_in server, client;

	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (server_fd < 0) {
		printf("Could not create socket.\n");
		exit(0);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt_val = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

	bind(server_fd, (struct sockaddr *) &server, sizeof(server));
	listen(server_fd, 128);

	printf("Server is listening on %d\n", port);

	while (1) {
		socklen_t client_len = sizeof(client);
		client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

		if (client_fd < 0) {
			printf("Could not establish new connection.\n");
			exit(0);
		}
		
		while (1) {
			int read = recv(client_fd, buf, BUFFER_SIZE, 0);
 			if (!read) break;
			send(client_fd, buf, read, 0);
		}
	}

	return 0;
} 
