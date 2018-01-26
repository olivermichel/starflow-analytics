
add_executable(packet_counts
        ${ETC_SRC}
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${PROTO_SRC}
        ${TYPES_SRC}
        src/app/packet_counts_main.cc)

target_link_libraries(packet_counts protobuf)
target_link_libraries(packet_counts pcap)
target_link_libraries(packet_counts raft)
target_link_libraries(packet_counts grpc++)
