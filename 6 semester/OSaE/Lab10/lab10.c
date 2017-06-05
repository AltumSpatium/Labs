#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <pthread.h>
#define BUFFER_SIZE 1024
#define CONNECTIONS_COUNT 1024
#define MAX_LENGTH 1024

struct sockaddr_in get_server_struct(int argc, char *argv[]);
int open_server_fd(struct sockaddr_in* server);
void* handle_connection(void* vargp);
void handle_request(int client_fd, char* method, char* uri, char* version);
void handle_response(int client_fd, char* method, char* uri, char* version);
void handle_error(
	int client_fd,
	char* cause,
	char *error_code,
	char* short_message,
	char* long_message
);
void parse_uri(char* uri, char* file_name);
void convert_spaces(char* source);
void send_file(int client_fd, char* file_name, int file_size);
void get_file_type(char* file_name, char* file_type);


int main (int argc, char *argv[]) {
	int server_fd, *client_fd;
	struct sockaddr_in server, client;
	socklen_t client_len = sizeof(client);
	pthread_t thread;

	server = get_server_struct(argc, argv);
	server_fd = open_server_fd(&server);
	printf("Server is listening on %d\n", ntohs(server.sin_port));

	while (1) {
		client_fd = malloc(sizeof(int));
		*client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);
		pthread_create(&thread, NULL, handle_connection, client_fd);
	}

	return 0;
}


struct sockaddr_in get_server_struct(int argc, char *argv[]){
	struct sockaddr_in server;
	struct in_addr address;
	address.s_addr = htonl(INADDR_ANY);
	int port = 8080;
	char c;

	while ((c = getopt(argc, argv, "p:r:a:")) != -1) {
		switch (c) {
			case 'r':
				chdir(optarg);
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'a':
				inet_aton(optarg, &address);
				break;
			case '?':
				fprintf(stderr,"Wrong arguments given.\n");
				exit(1);
			default:
				exit(1);
		}
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr = address;
	return server;
}


int open_server_fd(struct sockaddr_in* server){
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int opt_val = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
	bind(server_fd, (struct sockaddr *) server, sizeof(struct sockaddr_in));
	listen(server_fd, CONNECTIONS_COUNT);
	return server_fd;
}


void* handle_connection(void* vargp){
	char method[MAX_LENGTH];
	char uri[MAX_LENGTH];
	char version[MAX_LENGTH];

	pthread_detach(pthread_self());
	int client_fd = *((int*) vargp);
	handle_request(client_fd, method, uri, version);
	handle_response(client_fd, method, uri, version);

	close(client_fd);
	free(vargp);
	return NULL;
}


void handle_request(int client_fd, char* method, char* uri, char* version) {
	char buffer[BUFFER_SIZE];
	int recv_length = recv(client_fd, buffer, BUFFER_SIZE, 0);
	sscanf(buffer, "%s %s %s", method, uri, version);
	printf("Request: %s %s %s\n", method, uri, version);
	while (recv_length == BUFFER_SIZE) {
		recv(client_fd, buffer, BUFFER_SIZE, 0);
	}
}


void handle_response(int client_fd, char* method, char* uri, char* version){
	char file_name[MAX_LENGTH];
	struct stat file_stat;

	if (strcasecmp(method, "GET")) {
		handle_error(
			client_fd,
			method,
			"501",
			"Not Implemented",
			"This server doesn't have an implementation of this method."
		);
		return;
	}

	parse_uri(uri, file_name);
	if (stat(file_name, &file_stat) < 0) {
		handle_error(
			client_fd,
			file_name,
			"404",
			"Not found",
			"File does not exist."
		);
		return;
	}

	if (!(S_ISREG(file_stat.st_mode)) || !(S_IRUSR & file_stat.st_mode)) {
		handle_error(
			client_fd,
			file_name,
			"403",
			"Forbidden",
			"Server has no access to this file."
		);
		return;
	}

	send_file(client_fd, file_name, file_stat.st_size);
}


void handle_error(
	int client_fd,
	char* cause,
	char* error_code,
	char* short_message,
	char* long_message
){
	char header[BUFFER_SIZE];
	char html[BUFFER_SIZE];

	sprintf(
		html,
		"<html>\
			<head>\
				<title>Server error</title>\
			</head>\
			<body>\
				<h1>Error %s: %s</h1>\
				<p>%s: %s</p>\
			</body>\
		</html>",
		error_code,
		short_message,
		cause,
		long_message
	);

	sprintf(
		header,
		"HTTP/1.0 %s %s\r\n\
		Content-type: text/html\r\n\
		Content-length: %d\r\n\r\n",
		error_code,
		short_message,
		strlen(html)
	);

	send(client_fd, header, strlen(header), 0);
	send(client_fd, html, strlen(html), 0);
	printf("Response: HTTP/1.0 %s %s\n", error_code, short_message);
}


void parse_uri(char* uri, char* file_name){
	convert_spaces(uri);
	char* args_pointer = index(uri, '?');
	if (args_pointer){
		*args_pointer = '\0';
	}
	strcpy(file_name, ".");
	strcat(file_name, uri);
	if (uri[strlen(uri)-1] == '/') {
		strcat(file_name, "index.html");
	}
}

void convert_spaces(char* source){
	char* space_pointer;
	char* str_pointer = source;
	char buffer[MAX_LENGTH];
	buffer[0] = '\0';
	while((space_pointer = strstr(str_pointer, "\%20"))){
		*space_pointer = '\0';
		strcat(buffer, str_pointer);
		strcat(buffer, " ");
		str_pointer = space_pointer+3;
	}
	strcat(buffer, str_pointer);
	strcpy(source, buffer);
}

void send_file(int client_fd, char* file_name, int file_size){
	int file_fd;
	char* memory_pointer;
	char file_type[MAX_LENGTH];
	char header[BUFFER_SIZE];

	get_file_type(file_name, file_type);
	sprintf(
		header,
		"HTTP/1.0 200 OK\r\n\
		Server: Simple Web Server\r\n\
		Charset=utf-8\r\n\
		Content-length: %d\r\n\
		Content-type: %s\r\n\r\n",
		file_size,
		file_type
	);

	file_fd = open(file_name, O_RDONLY, 0);
	memory_pointer = mmap(0, file_size, PROT_READ, MAP_PRIVATE, file_fd, 0);
	send(client_fd, header, strlen(header), 0);
	send(client_fd, memory_pointer, file_size, 0);
	printf("Response: HTTP/1.0 200 OK\n");
	close(file_fd);
	munmap(memory_pointer, file_size);
}

void get_file_type(char* file_name, char* file_type){
	if(strstr(file_name, ".html")){
		strcpy(file_type, "text/html");
	} else if (strstr(file_name, ".gif")){
		strcpy(file_type, "image/gif");
	} else if (strstr(file_name, ".jpg")){
		strcpy(file_type, "image/jpeg");
	} else if (strstr(file_name, ".png")){
		strcpy(file_type, "image/png");
	} else if (strstr(file_name, ".mp3")){
		strcpy(file_type, "audio/mpeg");
	} else {
		strcpy(file_type, "text/plain");
	}
}
