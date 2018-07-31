FROM ubuntu:rolling

RUN apt-get update -qq && \
    apt-get install -y g++ cmake git wget ssh libmysqlclient-dev libboost-all-dev libtcmalloc-minimal4 libcurl4-openssl-dev libssl-dev && \
    rm -rf /var/lib/apt/lists/*
