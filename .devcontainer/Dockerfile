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
    libtool \
    zlib1g-dev \
    libbz2-dev \
    liblzma-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Autoconf 2.71
RUN wget https://ftp.gnu.org/gnu/autoconf/autoconf-2.71.tar.gz && \
    tar -xf autoconf-2.71.tar.gz && \
    cd autoconf-2.71 && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf autoconf-2.71 autoconf-2.71.tar.gz

# Download and install Boost 1.85.0
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.85.0/source/boost_1_85_0.tar.gz && \
    tar xfz boost_1_85_0.tar.gz && \
    cd boost_1_85_0 && \
    ./bootstrap.sh --prefix=/usr/local && \
    ./b2 install --with=all --without-stacktrace && \
    cd .. && \
    rm -rf boost_1_85_0 boost_1_85_0.tar.gz

# Download and Install QuickFIX
RUN git clone https://github.com/quickfix/quickfix.git /quickfix \
&& cd /quickfix \
&& touch README \
&& ./bootstrap \
&& ./configure \
&& make \
&& make install \
&& ldconfig

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
