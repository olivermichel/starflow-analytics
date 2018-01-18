
set(TEST_SRC
        test/types/clfr_test.cc
        test/types/features_test.cc
        test/types/raw_packet_test.cc
        test/types/key_test.cc
        test/modules/flow_table_test.cc
        test/modules/pcap_reader_test.cc
        test/etc/tcp_flow_state_test.cc
        test/etc/timer_test.cc
        test/modules/raw_packet_parser_test.cc)

add_executable(test_runner
        test/test_runner_main.cc
        ${ETC_SRC}
        ${MODULES_SRC}
        ${TYPES_SRC}
        ${PROTO_SRC}
        ${TEST_SRC})

target_include_directories(test_runner PUBLIC test/include)
target_link_libraries(test_runner pcap)
target_link_libraries(test_runner protobuf)
target_link_libraries(test_runner grpc++)

enable_testing()

add_test(NAME CLFR
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner CLFR)

add_test(NAME RawPacket
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner RawPacket)

add_test(NAME FlowTable
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner FlowTable)

add_test(NAME PCAPReader
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner PCAPReader)

add_test(NAME TCPFlowState
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner TCPFlowState)

add_test(NAME Timer
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner Timer)

add_test(NAME RawPacketParser
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner RawPacketParser)

add_test(NAME Key
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner Key)

add_test(NAME Features
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND test_runner Features)
