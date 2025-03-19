// afl_message_parser_fuzzer.c
// AFL fuzz harness for message parsing in msmtp

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "src/list.h"
#include "src/tools.h"
#include "src/msmtp.h"

// Function declaration from msmtp.c
int msmtp_read_headers(FILE *mailf, FILE *tmpf,
        list_t *recipients,
        char **from,
        int *have_date,
        int *have_msgid,
        char **errstr);

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
    
    // Create a list for recipients
    list_t *recipients = list_new();
    
    // Variables for the results
    char *from = NULL;
    int have_date = 0;
    int have_msgid = 0;
    char *errstr = NULL;
    
    // Call the header parsing function
    msmtp_read_headers(f, NULL, recipients, &from, &have_date, &have_msgid, &errstr);
    
    // Clean up
    fclose(f);
    list_xfree(recipients, free);
    free(from);
    free(errstr);
    
    return 0;
}
