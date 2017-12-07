
add_executable(packet_capture
        ${ETC_SRC}
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${PROTO_SRC}
        ${TYPES_SRC}
        src/packet_capture_main.cc)

target_link_libraries(packet_capture protobuf)
target_link_libraries(packet_capture pcap)
target_link_libraries(packet_capture raft)