#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("usage: ./client ip port\n");
        return 1;
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *server_address;
    if (getaddrinfo(argv[1], argv[2], &hints, &server_address)) {
        printf("getaddrinfo() failed.\n");
        return 1;
    }

    printf("Creating socket...\n");
    int socket_fd;
    socket_fd = socket(server_address->ai_family,
                        server_address->ai_socktype, 
                        server_address->ai_protocol);
    if (socket_fd<0) {
        printf("socket() failed.\n");
        return 1;
    }

    printf("Connecting...\n");
    if (connect(socket_fd,
                server_address->ai_addr, 
                server_address->ai_addrlen)) {
        printf("connect() failed.\n");
        return 1;
    }
    
    freeaddrinfo(server_address);

    printf("Connected.\n");

    fd_set fd_group;
    char buffer[BUFFER_SIZE+1] = { 0 };
    struct timeval wait;

    while(1) {
        // select() modifies file descriptors after it runs
        FD_ZERO(&fd_group);
        FD_SET(socket_fd, &fd_group);
        FD_SET(STDIN_FILENO, &fd_group);

        // select() modifies wait after it runs
        wait.tv_sec = 2;
        wait.tv_usec = 0;

        // since we know socket_fd is larger value than STDIN_FILENO...
        if (select(socket_fd+1, &fd_group, 0, 0, &wait) < 0) {
            printf("select() failed.\n");
            return 1;
        }

        if (FD_ISSET(socket_fd, &fd_group)) {
            if (recv(socket_fd, buffer, BUFFER_SIZE, 0) < 1) {
                printf("Connection closed by server.\n");
                break;
            }
            printf("%s\n",buffer);
            memset(buffer, 0, BUFFER_SIZE+1);
        }

        if(FD_ISSET(STDIN_FILENO, &fd_group)) {
            if (!fgets(buffer, BUFFER_SIZE, stdin)) {
                break;
            }
            send(socket_fd, buffer, strlen(buffer), 0);
            printf("\n");
            memset(buffer, 0, BUFFER_SIZE+1);
        }
    }

    printf("Closing socket...\n");
    close(socket_fd);

    printf("Finished.\n");
    return 0;
}

