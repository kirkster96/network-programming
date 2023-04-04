#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define PORT "8080"
#define LISTEN_BACKLOG 50

volatile sig_atomic_t stop_flag = 0;

void sigint_handler(int signum) { 
    //Handler for SIGINT
    stop_flag = 1;
}

int main() {
    signal(SIGINT, sigint_handler);
    printf("Press Ctrl+C to shutdown\n");

    char address_buffer[100];
    int server_fd, client_fd, send_count;

    // Standard HTTP response header
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "IPv6 Server time is: ";

    time_t timer;
    char *time_msg;

    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);

    printf("getaddrinfo()\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, PORT, &hints, &bind_address);

    printf("socket()\n");
    server_fd = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (server_fd < 0) {
        fprintf(stderr, "socket() failed.\n");
        return 1;
    }

    printf("bind()\n");
    if (bind(server_fd, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed.\n");
        return 1;
    }
    freeaddrinfo(bind_address);


    printf("listen()\n");
    if (listen(server_fd, LISTEN_BACKLOG) < 0) {
        fprintf(stderr, "listen() failed.\n");
        return 1;
    }

    while (!stop_flag)
    {    
        printf("Waiting for connection...\n");
        client_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_len);
        if (client_fd < 0) {
            fprintf(stderr, "accept() failed.\n");
            return 1;
        }

        printf("Client is connected... ");
        memset(address_buffer, 0, sizeof(address_buffer));
        getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
        printf("%s\n", address_buffer);

        printf("Sending response...\n");
        send_count = send(client_fd, response, strlen(response), 0);
        printf("Sent %d of %d bytes.\n", send_count, (int)strlen(response));
        time(&timer);
        time_msg = ctime(&timer);
        send_count = send(client_fd, time_msg, strlen(time_msg), 0);
        printf("Sent %d of %d bytes.\n", send_count, (int)strlen(time_msg));

        printf("Closing connection...\n");
        close(client_fd);
    }

    printf("Closing listening socket...\n");
    shutdown(server_fd, SHUT_RD);

    printf("Finished.\n");

    return 0;
}
