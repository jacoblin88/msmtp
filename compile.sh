# For the SMTP protocol fuzzer
afl-gcc -I. afl_smtp_protocol_fuzzer.c src/smtp.c src/net.c src/readbuf.c src/list.c src/xalloc.c src/tools.c -o afl_smtp_protocol_fuzzer

# For the message parser fuzzer
afl-gcc -I. afl_message_parser_fuzzer.c src/msmtp.c src/list.c src/xalloc.c src/tools.c -o afl_message_parser_fuzzer

# For the network fuzzer
afl-gcc -I. afl_network_fuzzer.c src/net.c src/readbuf.c src/xalloc.c -o afl_network_fuzzer

# For the SMTP auth fuzzer
afl-gcc -I. afl_smtp_auth_fuzzer.c src/smtp.c src/net.c src/readbuf.c src/list.c src/xalloc.c src/tools.c src/base64.c src/md5-apps.c -o afl_smtp_auth_fuzzer
