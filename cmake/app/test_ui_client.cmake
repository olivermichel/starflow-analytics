
add_executable(test_ui_client
        ${PROTO_SRC}
        src/modules/ui_client.cc
        src/test_ui_client_main.cc)

target_link_libraries(test_ui_client protobuf)
target_link_libraries(test_ui_client grpc++)
