
add_executable(clfr_stats
        ${ETC_SRC}
        ${KERNELS_SRC}
        ${MODULES_SRC}
        ${PROTO_SRC}
        ${TYPES_SRC}
        src/app/clfr_stats_main.cc)

target_link_libraries(clfr_stats protobuf)
target_link_libraries(clfr_stats pcap)
target_link_libraries(clfr_stats raft)
target_link_libraries(clfr_stats grpc++)
