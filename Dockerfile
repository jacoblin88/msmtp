# Dockerfile for msmtp fuzzing
FROM aflplusplus/aflplusplus:latest

# Install dependencies with more verbose output
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    autoconf \
    automake \
    libtool \
    pkg-config \
    libgnutls28-dev \
    libgsasl7-dev \
    libsecret-1-dev \
    texinfo \
    git \
    gettext \
    gettext-base \
    autopoint \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Verify autopoint is installed
RUN which autopoint || (echo "autopoint not found" && exit 1)

# Set up working directory
WORKDIR /fuzzing

# Clone msmtp repository
RUN git clone https://git.marlam.de/git/msmtp.git

# Build msmtp with AFL instrumentation
WORKDIR /fuzzing/msmtp
RUN ls -la && \
    autoreconf -i -f && \
    CC=afl-clang-fast CXX=afl-clang-fast++ ./configure --prefix=/usr && \
    make

# Copy fuzzer source files
COPY msmtp_fuzzer.c /fuzzing/
COPY msmtpd_fuzzer.c /fuzzing/
COPY auth_fuzzer.c /fuzzing/

# Compile fuzzers
WORKDIR /fuzzing
RUN afl-clang-fast -o msmtp_fuzzer msmtp_fuzzer.c && \
    afl-clang-fast -o msmtpd_fuzzer msmtpd_fuzzer.c && \
    afl-clang-fast -o auth_fuzzer auth_fuzzer.c

# Create directories for fuzzing inputs and outputs
RUN mkdir -p /fuzzing/inputs /fuzzing/outputs

# Create initial test cases
RUN echo "EHLO localhost\r\nMAIL FROM:<test@example.com>\r\nRCPT TO:<recipient@example.com>\r\nDATA\r\nSubject: Test\r\n\r\nThis is a test.\r\n.\r\nQUIT\r\n" > /fuzzing/inputs/smtp_commands.txt && \
    echo "334 VXNlcm5hbWU6\r\n235 Authentication successful\r\n" > /fuzzing/inputs/auth_response.txt

# Create a script to run all fuzzers
RUN echo '#!/bin/bash\n\
    # Function to handle cleanup on exit\n\
    cleanup() {\n\
    echo "Stopping all fuzzing processes..."\n\
    pkill -f afl-fuzz\n\
    exit 0\n\
    }\n\
    \n\
    # Set up trap for cleanup\n\
    trap cleanup SIGINT SIGTERM\n\
    \n\
    # Create output directories if they do not exist\n\
    mkdir -p /fuzzing/outputs/msmtp_fuzzer\n\
    mkdir -p /fuzzing/outputs/msmtpd_fuzzer\n\
    mkdir -p /fuzzing/outputs/auth_fuzzer\n\
    \n\
    # Start SMTP protocol fuzzer\n\
    echo "Starting SMTP protocol fuzzer..."\n\
    afl-fuzz -i /fuzzing/inputs -o /fuzzing/outputs/msmtp_fuzzer -M msmtp_main -t 5000 -- /fuzzing/msmtp_fuzzer &\n\
    MSMTP_PID=$!\n\
    sleep 5\n\
    \n\
    # Check if fuzzer is running\n\
    if ! kill -0 $MSMTP_PID 2>/dev/null; then\n\
    echo "SMTP protocol fuzzer failed to start properly"\n\
    else\n\
    echo "SMTP protocol fuzzer running with PID $MSMTP_PID"\n\
    fi\n\
    \n\
    # Start MSMTPD server fuzzer\n\
    echo "Starting MSMTPD server fuzzer..."\n\
    afl-fuzz -i /fuzzing/inputs -o /fuzzing/outputs/msmtpd_fuzzer -S msmtpd_secondary -t 5000 -- /fuzzing/msmtpd_fuzzer &\n\
    MSMTPD_PID=$!\n\
    sleep 5\n\
    \n\
    # Check if fuzzer is running\n\
    if ! kill -0 $MSMTPD_PID 2>/dev/null; then\n\
    echo "MSMTPD server fuzzer failed to start properly"\n\
    else\n\
    echo "MSMTPD server fuzzer running with PID $MSMTPD_PID"\n\
    fi\n\
    \n\
    # Start authentication fuzzer\n\
    echo "Starting authentication fuzzer..."\n\
    afl-fuzz -i /fuzzing/inputs -o /fuzzing/outputs/auth_fuzzer -S auth_secondary -t 5000 -- /fuzzing/auth_fuzzer &\n\
    AUTH_PID=$!\n\
    sleep 5\n\
    \n\
    # Check if fuzzer is running\n\
    if ! kill -0 $AUTH_PID 2>/dev/null; then\n\
    echo "Authentication fuzzer failed to start properly"\n\
    else\n\
    echo "Authentication fuzzer running with PID $AUTH_PID"\n\
    fi\n\
    \n\
    echo "All fuzzers started. Press Ctrl+C to stop."\n\
    echo "Fuzzing results will be saved to /fuzzing/outputs/"\n\
    \n\
    # Wait for all background processes\n\
    wait\n\
    ' > /fuzzing/run_fuzzers.sh && chmod +x /fuzzing/run_fuzzers.sh

# Set the entry point
ENTRYPOINT ["/fuzzing/run_fuzzers.sh"]
