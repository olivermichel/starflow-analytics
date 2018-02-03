
add_executable(tcp_out_of_sequence
    src/app/tcp_out_of_sequence_main.cc
    src/etc/format_helpers.cc
    src/etc/timer.cc
    src/kernels/benchmark_printer.cc
    src/kernels/clfr_file_reader.cc
    src/kernels/filter.cc
    src/kernels/formatted_printer.cc
    src/kernels/group_by.cc
    src/modules/clfr_file_reader.cc
    src/proto/starflow.pb.cc
    src/types/clfr.cc
    src/types/features.cc
    src/types/key.cc
    src/types/packet.cc)

target_link_libraries(tcp_out_of_sequence protobuf)
target_link_libraries(tcp_out_of_sequence raft)
