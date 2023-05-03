#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT "8080"
#define LISTEN_BACKLOG 50

int main(int argc, char const* argv[])
{
	struct addrinfo hints;
	struct addrinfo *result;
	int socket_fd, client_fd, s;
	char buffer[1024] = { 0 };
	char* hello = "Hello from server";

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if (s = getaddrinfo(NULL, PORT , &hints, &result)) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	
	socket_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (socket_fd<0)
	{
		fprintf(stderr, "socket() failed. (%d) %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	

	if (bind(socket_fd, result->ai_addr, result->ai_addrlen))
	{
		fprintf(stderr, "bind() failed. (%d) %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}
	

	if (listen(socket_fd, LISTEN_BACKLOG) < 0){
        fprintf(stderr, "listen() failed. (%d) %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	struct sockaddr_storage client_addr;
	socklen_t client_addrlen = sizeof(client_addr);
	client_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addrlen);
	if (client_fd < 0){
		fprintf(stderr, "accept() failed. (%d) %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	read(client_fd, buffer, 1024);
	printf("%s\n", buffer);
	send(client_fd, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	close(client_fd);

	// closing the listening socket
	shutdown(socket_fd, SHUT_RDWR);

	return 0;
}
