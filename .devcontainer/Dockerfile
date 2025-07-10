# Use Microsoft official C++ dev container base image
FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-22.04

# Argument: Optionally reinstall a specific CMake version from source
ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.21.5"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

# Install Node.js (required for building llhttp)
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get update && \
    apt-get install -y nodejs

# Install build-essential tools, clang, git, and other useful packages
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

# Copy the built llhttp library and headers to system locations
RUN cp /opt/llhttp/build/* /usr/local/lib/ && \
    cp /opt/llhttp/include/* /usr/local/include/

# [Optional] Uncomment to install additional vcpkg ports
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment to install additional packages
# RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
#     && apt-get -y install --no-install-recommends <your-package-list-here>

# Set working directory for your project
WORKDIR /app

# Copy your project files into the container
COPY . .

# Build your project (assuming you use CMake)
RUN cmake . && make

# Set the default command to run your server (update as needed)
CMD ["./server"]
