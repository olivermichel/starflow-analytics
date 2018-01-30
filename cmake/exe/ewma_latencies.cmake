
add_executable(ewma_latencies
        ${ETC_SRC}
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${PROTO_SRC}
        ${TYPES_SRC}
        src/app/ewma_latencies_main.cc)

target_link_libraries(ewma_latencies protobuf)
target_link_libraries(ewma_latencies pcap)
target_link_libraries(ewma_latencies raft)
target_link_libraries(ewma_latencies grpc++)
