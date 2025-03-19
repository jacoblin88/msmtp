// afl_network_fuzzer.c
// AFL fuzz harness for network handling in msmtp

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "src/net.h"
#include "src/readbuf.h"

// Mock file descriptor for simulating network I/O
typedef struct {
    uint8_t *data;
    size_t size;
    size_t pos;
} mock_fd_t;

// Global mock file descriptor
mock_fd_t g_mock_fd;

// Mock functions to replace network I/O
int recv(int fd, void *buf, size_t len, int flags) {
    (void)fd; // Unused
    (void)flags; // Unused
    
    if (g_mock_fd.pos >= g_mock_fd.size) {
        return 0; // EOF
    }
    
    size_t remaining = g_mock_fd.size - g_mock_fd.pos;
    size_t to_copy = (len < remaining) ? len : remaining;
    
    memcpy(buf, g_mock_fd.data + g_mock_fd.pos, to_copy);
    g_mock_fd.pos += to_copy;
    
    return to_copy;
}

int send(int fd, const void *buf, size_t len, int flags) {
    (void)fd; // Unused
    (void)buf; // Unused
    (void)flags; // Unused
    
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
    
    if (file_size < 2) {
        fclose(f);
        return 0;
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
    
    // Create a readbuf
    readbuf_t readbuf;
    readbuf_init(&readbuf);
    
    // Test net_gets
    char buffer[1024];
    size_t len;
    char *errstr = NULL;
    
    net_gets(42, &readbuf, buffer, sizeof(buffer), &len, &errstr);
    free(errstr);
    errstr = NULL;
    
    // Reset position to try other functions
    g_mock_fd.pos = 0;
    
    // Test net_puts
    net_puts(42, (const char *)g_mock_fd.data, g_mock_fd.size, &errstr);
    free(errstr);
    
    // Test SOCKS5 proxy handling
    if (g_mock_fd.size > 20) {
        g_mock_fd.pos = 0;
        net_socks5_connect(42, "example.org", 25, &errstr);
        free(errstr);
    }
    
    // Clean up
    free(g_mock_fd.data);
    
    return 0;
}
