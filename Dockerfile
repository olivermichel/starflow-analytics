
FROM olivermichel/ubuntu_protobuf_grpc:latest

RUN cd /root \
    && git clone https://github.com/RaftLib/RaftLib.git raft \
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
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make
