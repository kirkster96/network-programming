#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PORT "8080"
#define LISTEN_BACKLOG 50

int main(int argc, char const* argv[])
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int socket_fd, client_fd, s;
	char buffer[1024] = { 0 };
	char* hello = "Hello from server";

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	s = getaddrinfo(NULL, PORT , &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		socket_fd = socket(rp->ai_family, rp->ai_socktype,
					rp->ai_protocol);
		if (socket_fd < 0)
			continue;

		if (bind(socket_fd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;                  /* Success */
	}

	if (listen(socket_fd, LISTEN_BACKLOG) < 0){
		perror("listen"); 
		exit(EXIT_FAILURE);
	}

	struct sockaddr_storage client_addr;
	socklen_t client_addrlen = sizeof(client_addr);
	client_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addrlen);
	if (client_fd < 0){
		perror("accept"); 
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
