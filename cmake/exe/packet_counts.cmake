
add_executable(packet_counts
        src/app/packet_counts_main.cc
        src/etc/format_helpers.cc
        src/kernels/clfr_file_reader.cc
        src/kernels/formatted_printer.cc
        src/kernels/group_by.cc
        src/modules/clfr_file_reader.cc
        src/modules/counter.cc
        src/proto/starflow.pb.cc
        src/types/clfr.cc
        src/types/features.cc
        src/types/key.cc
        src/types/packet.cc)

target_link_libraries(packet_counts protobuf)
target_link_libraries(packet_counts raft)
