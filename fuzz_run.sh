# Create input directories with initial test cases
mkdir -p afl_inputs/smtp_protocol
mkdir -p afl_inputs/message_parser
mkdir -p afl_inputs/network
mkdir -p afl_inputs/smtp_auth

# Create output directories for AFL findings
mkdir -p afl_findings/smtp_protocol
mkdir -p afl_findings/message_parser
mkdir -p afl_findings/network
mkdir -p afl_findings/smtp_auth

# Create some initial test cases
echo "220 smtp.example.com ESMTP ready" > afl_inputs/smtp_protocol/greeting.txt
echo "250-smtp.example.com\r\n250-SIZE 52428800\r\n250-AUTH PLAIN LOGIN\r\n250 HELP" > afl_inputs/smtp_protocol/capabilities.txt

echo "From: user@example.com\r\nTo: recipient@example.com\r\nSubject: Test\r\n\r\nTest message" > afl_inputs/message_parser/simple_email.txt

echo "HTTP/1.1 200 OK\r\nContent-Length: 10\r\n\r\nTest data" > afl_inputs/network/http_response.txt

echo "334 VXNlcm5hbWU6" > afl_inputs/smtp_auth/auth_challenge.txt

# Run the fuzzers
afl-fuzz -i afl_inputs/smtp_protocol -o afl_findings/smtp_protocol -- ./afl_smtp_protocol_fuzzer @@
afl-fuzz -i afl_inputs/message_parser -o afl_findings/message_parser -- ./afl_message_parser_fuzzer @@
afl-fuzz -i afl_inputs/network -o afl_findings/network -- ./afl_network_fuzzer @@
afl-fuzz -i afl_inputs/smtp_auth -o afl_findings/smtp_auth -- ./afl_smtp_auth_fuzzer @@