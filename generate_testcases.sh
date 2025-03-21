#!/bin/bash
# Script to generate additional test cases for msmtp fuzzing

# Create directory for test cases if it doesn't exist
mkdir -p /fuzzing/inputs

# ==========================================
# SMTP Protocol Test Cases (for msmtp_fuzzer)
# ==========================================

# Basic valid SMTP commands
echo "HELO example.com\r\nMAIL FROM:<user@example.com>\r\nRCPT TO:<recipient@example.com>\r\nDATA\r\nSubject: Test\r\n\r\nBody content\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_basic.txt

# SMTP commands with extended options
echo "EHLO example.com\r\nMAIL FROM:<user@example.com> SIZE=1024 BODY=8BITMIME\r\nRCPT TO:<recipient@example.com> NOTIFY=SUCCESS,FAILURE\r\nDATA\r\nSubject: Extended Test\r\n\r\nExtended body content\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_extended.txt

# SMTP with authentication
echo "EHLO example.com\r\nAUTH LOGIN\r\ndXNlcm5hbWU=\r\ncGFzc3dvcmQ=\r\nMAIL FROM:<user@example.com>\r\nRCPT TO:<recipient@example.com>\r\nQUIT\r\n" > /fuzzing/inputs/smtp_auth.txt

# SMTP with TLS negotiation
echo "EHLO example.com\r\nSTARTTLS\r\nEHLO example.com\r\nMAIL FROM:<user@example.com>\r\nRCPT TO:<recipient@example.com>\r\nQUIT\r\n" > /fuzzing/inputs/smtp_tls.txt

# SMTP with multiple recipients
echo "EHLO example.com\r\nMAIL FROM:<user@example.com>\r\nRCPT TO:<recipient1@example.com>\r\nRCPT TO:<recipient2@example.com>\r\nRCPT TO:<recipient3@example.com>\r\nDATA\r\nSubject: Multiple Recipients\r\n\r\nTest with multiple recipients\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_multi_rcpt.txt

# SMTP with large message
large_body=$(printf 'A%.0s' {1..2000})
echo -e "EHLO example.com\r\nMAIL FROM:<user@example.com>\r\nRCPT TO:<recipient@example.com>\r\nDATA\r\nSubject: Large Message\r\n\r\n$large_body\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_large.txt

# SMTP with malformed commands
echo "EHLO\r\nMAIL FROM:\r\nRCPT\r\nDATA\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_malformed.txt

# SMTP with special characters
echo "EHLO example.com\r\nMAIL FROM:<user+tag@example.com>\r\nRCPT TO:<\"recipient with spaces\"@example.com>\r\nDATA\r\nSubject: Special Chars: äöüß!@#$%^&*()\r\n\r\nContent with special chars: ©®™€£¥§\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_special_chars.txt

# SMTP with line breaks in unusual places
echo "EH\r\nLO example.com\r\nMAIL\r\n FROM:<user@example.com>\r\nRCPT TO\r\n:<recipient@example.com>\r\nDATA\r\nSubject: Broken\r\n Lines\r\n\r\nTest\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_broken_lines.txt

# ==========================================
# MSMTPD Server Test Cases (for msmtpd_fuzzer)
# ==========================================

# Basic client connection
echo "EHLO client.example.com\r\nMAIL FROM:<client@example.com>\r\nRCPT TO:<local@localhost>\r\nDATA\r\nSubject: Server Test\r\n\r\nServer test message\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_basic.txt

# Client with invalid commands
echo "XXXX\r\nEHLO client.example.com\r\nFROM:<client@example.com>\r\nTO:<local@localhost>\r\nMESSAGE\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_invalid.txt

# Client with very long commands
long_domain=$(printf 'a%.0s' {1..500})
echo "EHLO $long_domain\r\nMAIL FROM:<client@example.com>\r\nRCPT TO:<local@localhost>\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_long.txt

# Client with rapid command sequence
echo "EHLO client.example.com\r\nMAIL FROM:<client@example.com>\r\nRCPT TO:<local@localhost>\r\nDATA\r\n.\r\nRSET\r\nMAIL FROM:<another@example.com>\r\nRCPT TO:<local@localhost>\r\nDATA\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_rapid.txt

# Client with NOOP commands
echo "EHLO client.example.com\r\nNOOP\r\nNOOP\r\nMAIL FROM:<client@example.com>\r\nNOOP\r\nRCPT TO:<local@localhost>\r\nNOOP\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_noop.txt

# Client with RSET commands
echo "EHLO client.example.com\r\nMAIL FROM:<client@example.com>\r\nRSET\r\nMAIL FROM:<reset@example.com>\r\nRCPT TO:<local@localhost>\r\nRSET\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_rset.txt

# Client with VRFY and EXPN commands (often disabled but worth testing)
echo "EHLO client.example.com\r\nVRFY postmaster\r\nEXPN users\r\nMAIL FROM:<client@example.com>\r\nQUIT\r\n" > /fuzzing/inputs/msmtpd_vrfy_expn.txt

# ==========================================
# Authentication Test Cases (for auth_fuzzer)
# ==========================================

# Basic AUTH LOGIN sequence
echo "334 VXNlcm5hbWU6\r\ndXNlcm5hbWU=\r\n334 UGFzc3dvcmQ6\r\ncGFzc3dvcmQ=\r\n235 Authentication successful\r\n" > /fuzzing/inputs/auth_login.txt

# AUTH PLAIN sequence
echo "334\r\ndXNlcm5hbWUAcGFzc3dvcmQ=\r\n235 Authentication successful\r\n" > /fuzzing/inputs/auth_plain.txt

# AUTH CRAM-MD5 sequence
echo "334 PDE4OTYuNjk3MTcwOTUyQHBvc3RvZmZpY2UucmVzdG9uLm1jaS5uZXQ+\r\ndXNlcm5hbWUgODVlMjcxZDc3ODM0NzZiMDRjNGI2ZTM2YTFiZWFmNzE=\r\n235 Authentication successful\r\n" > /fuzzing/inputs/auth_cram_md5.txt

# Failed authentication
echo "334 VXNlcm5hbWU6\r\ndXNlcm5hbWU=\r\n334 UGFzc3dvcmQ6\r\ncGFzc3dvcmQ=\r\n535 Authentication failed\r\n" > /fuzzing/inputs/auth_failed.txt

# Malformed base64 in authentication
echo "334 VXNlcm5hbWU6\r\nd$%^&*XNlcm5hbWU=\r\n334 UGFzc3dvcmQ6\r\ncGF$%^&*zc3dvcmQ=\r\n" > /fuzzing/inputs/auth_malformed.txt

# Very long credentials
long_base64=$(printf 'A%.0s' {1..1000})
echo "334 VXNlcm5hbWU6\r\n$long_base64\r\n334 UGFzc3dvcmQ6\r\n$long_base64\r\n" > /fuzzing/inputs/auth_long.txt

# Multiple authentication attempts
echo "334 VXNlcm5hbWU6\r\ndXNlcm5hbWU=\r\n334 UGFzc3dvcmQ6\r\ncGFzc3dvcmQ=\r\n535 Authentication failed\r\n334 VXNlcm5hbWU6\r\nYW5vdGhlcnVzZXI=\r\n334 UGFzc3dvcmQ6\r\nYW5vdGhlcnBhc3N3b3Jk\r\n235 Authentication successful\r\n" > /fuzzing/inputs/auth_multiple.txt

# AUTH with different mechanisms in sequence
echo "334 VXNlcm5hbWU6\r\ndXNlcm5hbWU=\r\n334 UGFzc3dvcmQ6\r\ncGFzc3dvcmQ=\r\n535 Authentication failed\r\n334\r\ndXNlcm5hbWUAcGFzc3dvcmQ=\r\n235 Authentication successful\r\n" > /fuzzing/inputs/auth_mixed.txt

# Empty responses
echo "334 VXNlcm5hbWU6\r\n\r\n334 UGFzc3dvcmQ6\r\n\r\n" > /fuzzing/inputs/auth_empty.txt

echo "Generated $(ls /fuzzing/inputs | wc -l) test cases in /fuzzing/inputs/"
