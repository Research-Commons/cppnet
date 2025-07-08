FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        cmake \
        clang \
        git \
        curl \
        wget \
        pkg-config \
        libssl-dev

# Install Node.js 18.x (or 20.x) from NodeSource
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs

# Clone and build llhttp using its official workflow
RUN git clone https://github.com/nodejs/llhttp.git /opt/llhttp && \
    cd /opt/llhttp && \
    npm ci && \
    make

WORKDIR /app
COPY . .
RUN cmake . && make
CMD ["./server"]
