
add_executable(active_connections
        src/app/active_connections_main.cc
        src/etc/format_helpers.cc
        src/etc/timer.cc
        src/kernels/benchmark_printer.cc
        src/kernels/clfr_file_reader.cc
        src/kernels/formatted_printer.cc
        src/kernels/group_by.cc
        src/kernels/map.cc
        src/modules/clfr_file_reader.cc
        src/proto/starflow.pb.cc
        src/types/clfr.cc
        src/types/features.cc
        src/types/key.cc
        src/types/packet.cc)

target_link_libraries(active_connections protobuf)
target_link_libraries(active_connections raft)
