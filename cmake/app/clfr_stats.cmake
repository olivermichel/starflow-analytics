
add_executable(clfr_stats
        src/app/clfr_stats_main.cc
        src/etc/format_helpers.cc
        src/modules/clfr_file_reader.cc
        src/proto/starflow.pb.cc
        src/types/clfr.cc
        src/types/features.cc
        src/types/key.cc
        src/types/packet.cc)

target_link_libraries(clfr_stats protobuf)
target_link_libraries(clfr_stats raft)

