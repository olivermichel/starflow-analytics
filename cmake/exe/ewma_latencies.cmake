
add_executable(ewma_latencies
    src/app/ewma_latencies_main.cc
    src/etc/format_helpers.cc
    src/kernels/clfr_file_reader.cc
    src/kernels/formatted_printer.cc
    src/kernels/group_by.cc
    src/modules/clfr_file_reader.cc
    src/modules/ewma.cc
    src/proto/starflow.pb.cc
    src/types/clfr.cc
    src/types/features.cc
    src/types/key.cc
    src/types/packet.cc)

target_link_libraries(ewma_latencies protobuf)
target_link_libraries(ewma_latencies raft)
