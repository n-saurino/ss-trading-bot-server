# Dockerfile

# Use an official Ubuntu as a base image
FROM ubuntu:20.04

# Set environment variables to make the build non-interactive
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Chicago  

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    git \
    vim \
    wget \
    libboost-all-dev \
    libssl-dev \
    libstdc++-9-dev \
    ccache \
    clang \
    valgrind \
    libcurl4-openssl-dev \
    libpq-dev \
    libxml2-dev \
    libjsoncpp-dev \
    libgtest-dev \
    clang-tidy \
    clang-format \
    doxygen \
    autoconf \
    automake \
    && rm -rf /var/lib/apt/lists/*

# Install Google Test
RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib && \
    cd / && \
    rm -rf /usr/src/gtest

# Set the working directory inside the container
WORKDIR /workspace

# Create a non-root user for security purposes
RUN useradd -ms /bin/bash devuser
USER devuser

# Default command to run when the container starts
CMD ["/bin/bash"]