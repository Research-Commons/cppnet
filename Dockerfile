# Use Microsoft official C++ devcontainer base image
FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# Install Node.js (required for building llhttp)
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get update && \
    apt-get install -y nodejs

# Install build tools, clang, git, and other useful packages
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        clang \
        git \
        curl \
        wget \
        pkg-config \
        libssl-dev

# Clone and build llhttp (HTTP/1.1 parser library)
RUN git clone https://github.com/nodejs/llhttp.git /opt/llhttp && \
    cd /opt/llhttp && \
    npm ci && \
    make

# Copy the built llhttp static library and header to system locations
RUN cp /opt/llhttp/build/libllhttp.a /usr/local/lib/ && \
    cp /opt/llhttp/build/llhttp.h /usr/local/include/

# Set working directory for your project
WORKDIR /app

# Copy your project files into the container
COPY . .

# Build your project (assuming you use CMake)
RUN cmake . && make

# Set the default command to run your server (update as needed)
CMD ["./server"]
