
cmake_minimum_required(VERSION 3.5)
project(starflow_analytics)

set(CMAKE_CXX_STANDARD 14) # raft requires C++14
set(CMAKE_CXX_FLAGS -DL1D_CACHE_LINE_SIZE=64) # 64 bytes for x86_64 -- adapt for used arch

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    link_libraries(pthread)
endif()

include(cmake/common.cmake)

include(cmake/dep/catch.cmake)
include(cmake/dep/grpc.cmake)
include(cmake/dep/pcap.cmake)
include(cmake/dep/protobuf.cmake)
include(cmake/dep/raft.cmake)
include(cmake/dep/cxxopts.cmake)

include_directories(ext/include)

set(OUT_DIR ${CMAKE_CURRENT_LIST_DIR}/out)

set(MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/src/modules)
set(MODULES_SRC
        ${MODULES_DIR}/clfr_file_reader.h ${MODULES_DIR}/clfr_file_reader.cc
        ${MODULES_DIR}/clfr_file_writer.h ${MODULES_DIR}/clfr_file_writer.cc
        ${MODULES_DIR}/counter.h ${MODULES_DIR}/counter.cc
        ${MODULES_DIR}/ewma.h ${MODULES_DIR}/ewma.cc
        ${MODULES_DIR}/filter.h ${MODULES_DIR}/filter.cc
        ${MODULES_DIR}/flow_table.h ${MODULES_DIR}/flow_table.cc
        ${MODULES_DIR}/live_capture.h ${MODULES_DIR}/live_capture.cc
        ${MODULES_DIR}/pcap_file_reader.h ${MODULES_DIR}/pcap_file_reader.cc
        ${MODULES_DIR}/raw_packet_parser.h ${MODULES_DIR}/raw_packet_parser.cc
        ${MODULES_DIR}/tzsp_receiver.h ${MODULES_DIR}/tzsp_receiver.cc
        ${MODULES_DIR}/ui_adapter.h ${MODULES_DIR}/ui_adapter.cc
        ${MODULES_DIR}/ui_client.h ${MODULES_DIR}/ui_client.cc
        ${MODULES_DIR}/ui_service.h ${MODULES_DIR}/ui_service.cc)

set(TYPES_DIR ${CMAKE_CURRENT_LIST_DIR}/src/types)
set(TYPES_SRC
        ${TYPES_DIR}/clfr.h ${TYPES_DIR}/clfr.cc
        ${TYPES_DIR}/features.h ${TYPES_DIR}/features.cc
        ${TYPES_DIR}/key.h ${TYPES_DIR}/key.cc
        ${TYPES_DIR}/packet.h ${TYPES_DIR}/packet.cc
        ${TYPES_DIR}/raw_packet.h ${TYPES_DIR}/raw_packet.cc)

set(KERNELS_DIR ${CMAKE_CURRENT_LIST_DIR}/src/kernels)
set(KERNELS_SRC
        ${KERNELS_DIR}/benchmark_printer.h ${KERNELS_DIR}/benchmark_printer.cc
        ${KERNELS_DIR}/clfr_file_writer.h ${KERNELS_DIR}/clfr_file_writer.cc
        ${KERNELS_DIR}/clfr_file_reader.h ${KERNELS_DIR}/clfr_file_reader.cc
        ${KERNELS_DIR}/clfr_printer.h ${KERNELS_DIR}/clfr_printer.cc
        ${KERNELS_DIR}/flow_table.h ${KERNELS_DIR}/flow_table.cc
        ${KERNELS_DIR}/filter.h ${KERNELS_DIR}/filter.cc
        ${KERNELS_DIR}/formatted_printer.h ${KERNELS_DIR}/formatted_printer.cc
        ${KERNELS_DIR}/group_by.h ${KERNELS_DIR}/group_by.cc
        ${KERNELS_DIR}/live_capture.h ${KERNELS_DIR}/live_capture.cc
        ${KERNELS_DIR}/map.h ${KERNELS_DIR}/map.cc
        ${KERNELS_DIR}/packet_printer.h ${KERNELS_DIR}/packet_printer.cc
        ${KERNELS_DIR}/pcap_file_reader.h ${KERNELS_DIR}/pcap_file_reader.cc
        ${KERNELS_DIR}/printer.h ${KERNELS_DIR}/printer.cc
        ${KERNELS_DIR}/raw_packet_parser.h ${KERNELS_DIR}/raw_packet_parser.cc
        ${KERNELS_DIR}/replicator.h ${KENRELS_DIR}/replicator.cc
        ${KERNELS_DIR}/tzsp_receiver.h ${KERNELS_DIR}/tzsp_receiver.cc
        ${KERNELS_DIR}/zip.h ${KERNELS_DIR}/zip.cc)

set(ETC_DIR ${CMAKE_CURRENT_LIST_DIR}/src/etc)
set(ETC_SRC
        ${ETC_DIR}/format_helpers.h ${ETC_DIR}/format_helpers.cc
        ${ETC_DIR}/tcp_flow_state.h ${ETC_DIR}/tcp_flow_state.cc
        ${ETC_DIR}/timer.h ${ETC_DIR}/timer.cc)

set(PROTO_DIR ${CMAKE_CURRENT_LIST_DIR}/src/proto)
set(PROTO_SRC
        ${PROTO_DIR}/starflow.pb.h ${PROTO_DIR}/starflow.pb.cc
        ${PROTO_DIR}/starflow.grpc.pb.h ${PROTO_DIR}/starflow.grpc.pb.cc)

file(MAKE_DIRECTORY ${PROTO_DIR})
file(MAKE_DIRECTORY ${OUT_DIR})

add_custom_command(
        OUTPUT ${PROTO_SRC}
        MAIN_DEPENDENCY starflow.proto
        COMMAND protoc
        ARGS --cpp_out=src/proto starflow.proto
        COMMAND ${PROTOBUF_PROTOC}
        ARGS --grpc_out=src/proto
             --plugin=protoc-gen-grpc=${GRPC_CPP_GEN} starflow.proto
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

include(cmake/app/active_connections.cmake)
include(cmake/app/batch_test.cmake)
include(cmake/app/clfr_stats.cmake)
include(cmake/app/ewma_latencies.cmake)
include(cmake/app/flowlet_size_histogram.cmake)
include(cmake/app/packet_counts.cmake)
include(cmake/app/parallel_test.cmake)
include(cmake/app/passthrough.cmake)
include(cmake/app/pcap_flow_file_exporter.cmake)
include(cmake/app/tcp_non_monotonic.cmake)
include(cmake/app/tcp_out_of_sequence.cmake)
include(cmake/app/test_runner.cmake)
include(cmake/app/test_ui_client.cmake)
include(cmake/app/zip_test.cmake)
