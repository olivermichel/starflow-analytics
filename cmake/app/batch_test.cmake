
add_executable(batch_test
        src/app/batch_test_main.cc
        src/etc/format_helpers.cc
        src/etc/timer.cc
        src/kernels/benchmark_printer.cc
        src/kernels/clfr_file_reader.cc
        src/modules/clfr_file_reader.cc
        src/proto/starflow.pb.cc
        src/types/clfr.cc
        src/types/features.cc
        src/types/key.cc
        src/types/packet.cc)

target_link_libraries(batch_test protobuf)
target_link_libraries(batch_test raft)
