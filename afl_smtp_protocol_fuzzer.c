// afl_smtp_protocol_fuzzer.c
// AFL fuzz harness for SMTP protocol handling in msmtp

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "src/smtp.h"
#include "src/net.h"
#include "src/readbuf.h"
#include "src/list.h"

// Mock file descriptor for simulating network I/O
typedef struct {
    uint8_t *data;
    size_t size;
    size_t pos;
} mock_fd_t;

// Global mock file descriptor
mock_fd_t g_mock_fd;

// Mock functions to replace network I/O
int net_recv(int fd, void *buf, size_t len, char **errstr) {
    (void)fd; // Unused
    (void)errstr; // Unused
    
    if (g_mock_fd.pos >= g_mock_fd.size) {
        return 0; // EOF
    }
    
    size_t remaining = g_mock_fd.size - g_mock_fd.pos;
    size_t to_copy = (len < remaining) ? len : remaining;
    
    memcpy(buf, g_mock_fd.data + g_mock_fd.pos, to_copy);
    g_mock_fd.pos += to_copy;
    
    return to_copy;
}

int net_send(int fd, const void *buf, size_t len, char **errstr) {
    (void)fd; // Unused
    (void)buf; // Unused
    (void)errstr; // Unused
    
    // Just pretend we sent the data
    return len;
}

int main(int argc, char **argv) {
    // Check if we have an input file
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    // Open the input file
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Failed to open input file");
        return 1;
    }
    
    // Get file size
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if (file_size < 4) {
        fclose(f);
        return 0; // Need at least a few bytes for a valid SMTP response
    }
    
    // Allocate buffer for file data
    g_mock_fd.data = (uint8_t *)malloc(file_size);
    if (!g_mock_fd.data) {
        fclose(f);
        return 1;
    }
    
    // Read file data
    size_t bytes_read = fread(g_mock_fd.data, 1, file_size, f);
    fclose(f);
    
    g_mock_fd.size = bytes_read;
    g_mock_fd.pos = 0;
    
    // Create a new SMTP server
    smtp_server_t srv = smtp_new(NULL, SMTP_PROTO_SMTP);
    srv.fd = 42; // Arbitrary non-negative value
    
    // Try to parse SMTP messages
    list_t *msg = NULL;
    char *errstr = NULL;
    
    // Test smtp_get_msg
    smtp_get_msg(&srv, &msg, &errstr);
    if (msg) {
        list_xfree(msg, free);
    }
    free(errstr);
    errstr = NULL;
    
    // Reset position to try other functions
    g_mock_fd.pos = 0;
    
    // Test smtp_get_greeting
    smtp_get_greeting(&srv, &msg, NULL, &errstr);
    if (msg) {
        list_xfree(msg, free);
    }
    free(errstr);
    errstr = NULL;
    
    // Reset position to try other functions
    g_mock_fd.pos = 0;
    
    // Test smtp_init
    smtp_init(&srv, "example.org", &msg, &errstr);
    if (msg) {
        list_xfree(msg, free);
    }
    free(errstr);
    
    // Clean up
    smtp_close(&srv);
    free(g_mock_fd.data);
    
    return 0;
}
