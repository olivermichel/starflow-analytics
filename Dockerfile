
FROM ubuntu:16.04

RUN apt-get update -q \
    && apt-get install -y build-essential software-properties-common

RUN add-apt-repository ppa:ubuntu-toolchain-r/test \
    && apt-get update -q \
    && apt-get install -qy g++-6 autoconf libtool curl cmake git libboost-dev libbz2-dev libpcap-dev unzip \
    && update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6

WORKDIR /root
RUN git clone -b v3.5.1 https://github.com/google/protobuf.git \
    && cd protobuf \
    && git submodule update --init \
    && ./autogen.sh \
    && ./configure \
    && make \
    && make install \
    && ldconfig

WORKDIR /root
RUN git clone -b v1.8.5 https://github.com/grpc/grpc.git \
    && cd grpc \
    && git submodule update --init \
    && make \
    && make install \
    && ldconfig

WORKDIR /root
RUN git clone https://github.com/RaftLib/RaftLib.git raft \
    && cd raft \
    && git submodule update --init \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && make install \
    && ldconfig

WORKDIR /root
ADD . starflow_analytics
RUN cd starflow_analytics \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make
