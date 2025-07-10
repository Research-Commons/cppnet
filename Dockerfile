FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
        build-essential \
        clang \
        cmake \
        git \
        curl \
        wget \
        pkg-config \
        libssl-dev

# Remove conflicting package before Node.js install
RUN apt-get remove -y libnode-dev || true

# Install Node.js (for llhttp build)
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs

# Clone and build llhttp
RUN git clone https://github.com/nodejs/llhttp.git /opt/llhttp && \
    cd /opt/llhttp && \
    npm ci && \
    make

RUN cp /opt/llhttp/build/libllhttp.a /usr/local/lib/ && \
    cp /opt/llhttp/build/llhttp.h /usr/local/include/

WORKDIR /app
COPY . .

RUN cmake . && make

CMD ["./server"]
