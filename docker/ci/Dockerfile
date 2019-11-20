FROM ubuntu:rolling

# Initially populate apt-get package list
RUN apt-get update -qq

# Building essentials and common software
RUN apt-get install -y gcc-8 g++-8 cmake git wget curl ssh

# Shiro dependencies available on the official repositories
RUN apt-get install -y \
    libmysqlclient21 \
    default-libmysqlclient-dev \
    libboost-all-dev \
    libtcmalloc-minimal4 \
    libcurl4-openssl-dev \
    libssl-dev \
    liblzma-dev \
    libmaxminddb-dev

# Remove apt-get package lists to minimize container size
RUN rm -rf /var/lib/apt/lists/*

# Set gcc 8 as standard compiler
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
ENV CC=/usr/bin/gcc-8
ENV CXX=/usr/bin/g++-8

# HinnantDate
RUN git clone https://github.com/HowardHinnant/date.git && \
    mkdir -p /usr/include/date && \
    cp date/include/date/date.h /usr/include/date/ && \
    rm -rf date/

# sqlpp11
RUN git clone https://github.com/rbock/sqlpp11.git && \
    mkdir -p sqlpp11/build && \
    cd sqlpp11/build && \
    cmake -DENABLE_TESTS=Off .. && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf sqlpp11/

# sqlpp11-connector-mysql
RUN git clone https://github.com/rbock/sqlpp11-connector-mysql.git && \
    mkdir -p sqlpp11-connector-mysql/build && \
    cd sqlpp11-connector-mysql/build && \
    cmake -DDATE_INCLUDE_DIR=/usr/include/ -DSQLPP11_INCLUDE_DIR=/usr/local/include/ -DENABLE_TESTS=Off .. && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf sqlpp11-connector-mysql/

# cpp-redis & tacopie
RUN git clone https://github.com/cpp-redis/cpp_redis.git && \
    cd cpp_redis && \
    git submodule init && \
    git submodule update && \
    mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf cpp_redis/
