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
        libssl-dev \
        nodejs \
        npm

# Clone and build llhttp using its official workflow
RUN git clone https://github.com/nodejs/llhttp.git /opt/llhttp && \
    cd /opt/llhttp && \
    npm ci && \
    make

# Optionally, copy built llhttp artifacts to a system location if you want to link against them
# For example:
# RUN cp /opt/llhttp/build/* /usr/local/lib/ && cp /opt/llhttp/include/* /usr/local/include/

# Set the working directory for your project
WORKDIR /app

# Copy your project files into the container
COPY . .

# Build your project (assuming you use CMake)
RUN cmake . && make

# Set the default command to run your server
CMD ["./server"]
