// auth_fuzzer.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 10240
#define SMTP_PORT 25002 // Use a non-standard port for testing

// Mock SMTP server that handles authentication
void run_mock_auth_server(int port, const char *fuzz_data, size_t fuzz_size)
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
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Send initial greeting
    const char *greeting = "220 localhost ESMTP Auth Fuzzer ready\r\n";
    send(client_fd, greeting, strlen(greeting), 0);

    // Wait for EHLO
    read(client_fd, buffer, 1024);

    // Send EHLO response with AUTH capability
    const char *ehlo_response = "250-localhost\r\n250-AUTH PLAIN LOGIN CRAM-MD5\r\n250 OK\r\n";
    send(client_fd, ehlo_response, strlen(ehlo_response), 0);

    // Wait for AUTH command
    read(client_fd, buffer, 1024);

    // Send fuzzed authentication response
    send(client_fd, fuzz_data, fuzz_size, 0);

    // Close sockets
    close(client_fd);
    close(server_fd);
}

// Run msmtp client with authentication
void run_msmtp_auth_client(int port)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "../src/msmtp --host=localhost --port=%d --from=test@example.com "
             "--auth=plain --user=testuser --password=testpass recipient@example.com",
             port);

    FILE *pipe = popen(cmd, "w");
    if (!pipe)
    {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    // Send a simple email
    fprintf(pipe, "Subject: Test\n\nThis is a test.\n");
    pclose(pipe);
}

// Main fuzzing function
int main(int argc, char **argv)
{
    // For AFL, read input from stdin
    char fuzz_input[MAX_INPUT_SIZE];
    size_t bytes_read = read(0, fuzz_input, MAX_INPUT_SIZE - 1);
    if (bytes_read <= 0)
    {
        return 1;
    }
    fuzz_input[bytes_read] = '\0';

    // Replace null bytes with spaces to avoid truncating the input
    for (size_t i = 0; i < bytes_read; i++)
    {
        if (fuzz_input[i] == '\0')
        {
            fuzz_input[i] = ' ';
        }
    }

    // Fork to run server and client
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process - run mock auth server
        run_mock_auth_server(SMTP_PORT, fuzz_input, bytes_read);
        exit(0);
    }
    else
    {
        // Parent process - wait a moment for server to start
        usleep(100000); // 100ms

        // Run msmtp client with auth
        run_msmtp_auth_client(SMTP_PORT);

        // Wait for child to finish
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
