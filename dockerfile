FROM ubuntu:22.04

# Install basic build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        cmake \
        git \
        curl \
        wget \
        pkg-config \
        libssl-dev

# Clone llhttp (or any other dependencies you need)
RUN git clone https://github.com/nodejs/llhttp.git /opt/llhttp && \
    cd /opt/llhttp && \
    cmake . && \
    make && \
    make install

# Copy your project files
WORKDIR /app

# Build your project
RUN cmake . && make

# Set the default command
CMD ["./server"]
