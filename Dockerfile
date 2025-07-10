FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# Install Node.js (for llhttp build)
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get update && \
    apt-get install -y nodejs

# Install build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
        build-essential \
        clang \
        git \
        curl \
        wget \
        pkg-config \
        libssl-dev

# Clone and build llhttp
RUN git clone https://github.com/nodejs/llhttp.git /opt/llhttp && \
    cd /opt/llhttp && \
    npm ci && \
    make

RUN cp /opt/llhttp/build/* /usr/local/lib/ && \
    cp /opt/llhttp/include/* /usr/local/include/

WORKDIR /app
COPY . .

RUN cmake . && make

CMD ["./server"]
