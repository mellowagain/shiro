FROM ubuntu:rolling

# Dependencies installable throught official repositories
RUN apt-get update -qq && \
    apt-get install -y g++ cmake git wget ssh libmysqlclient20 default-libmysqlclient-dev libboost-all-dev libtcmalloc-minimal4 libcurl4-openssl-dev libssl-dev libmcrypt-dev liblzma-dev && \
    rm -rf /var/lib/apt/lists/*

# HinnantDate
RUN git clone https://github.com/HowardHinnant/date.git && \
    mkdir -p /usr/include/date && \
    cp date/include/date/date.h /usr/include/date/ && \
    rm -rf date/

# sqlpp11
RUN git clone https://github.com/rbock/sqlpp11.git && \
    mkdir -p sqlpp11/build && \
    cd sqlpp11/build && \
    cmake .. && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf sqlpp11/

# sqlpp11-connector-mysql
RUN git clone https://github.com/rbock/sqlpp11-connector-mysql.git && \
    mkdir -p sqlpp11-connector-mysql/build && \
    cd sqlpp11-connector-mysql/build && \
    cmake -DDATE_INCLUDE_DIR=/usr/include/ -DSQLPP11_INCLUDE_DIR=/usr/local/include/ .. && \
    make && \
    make install && \
    cd ../../ && \
    rm -rf sqlpp11-connector-mysql/
