
add_executable(tcp_out_of_sequence
        ${ETC_SRC}
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${PROTO_SRC}
        ${TYPES_SRC}
        src/app/tcp_out_of_sequence_main.cc)

target_link_libraries(tcp_out_of_sequence protobuf)
target_link_libraries(tcp_out_of_sequence pcap)
target_link_libraries(tcp_out_of_sequence raft)
target_link_libraries(tcp_out_of_sequence grpc++)
