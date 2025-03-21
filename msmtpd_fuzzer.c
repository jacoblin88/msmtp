// msmtpd_fuzzer.c
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
#define SMTP_PORT 25001 // Use a non-standard port for testing

// Run msmtpd server
void run_msmtpd_server(int port)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "../src/msmtpd --interface=127.0.0.1 --port=%d --command=\"cat > /dev/null\"", port);

    system(cmd);
}

// Send fuzzed SMTP commands to the server
void send_fuzzed_commands(int port, const char *fuzz_data, size_t fuzz_size)
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // Read greeting
    read(sock, buffer, 1024);

    // Send fuzzed data
    send(sock, fuzz_data, fuzz_size, 0);

    // Close socket
    close(sock);
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
        // Child process - run msmtpd server
        run_msmtpd_server(SMTP_PORT);
        exit(0);
    }
    else
    {
        // Parent process - wait a moment for server to start
        usleep(500000); // 500ms

        // Send fuzzed commands
        send_fuzzed_commands(SMTP_PORT, fuzz_input, bytes_read);

        // Kill the server
        kill(pid, SIGTERM);

        // Wait for child to finish
        int status;
        waitpid(pid, &status, 0);
    }

    return 0;
}
