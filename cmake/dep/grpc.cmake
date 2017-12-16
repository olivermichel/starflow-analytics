
find_library(GRPC_LIB grpc++)
find_file(GRPC_INC grpc++)
find_program(GRPC_CPP_GEN grpc_cpp_plugin)

if(NOT GRPC_LIB OR NOT GRPC_INC OR NOT GRPC_CPP_GEN)
    message(WARNING "grpc++: not found")
else()
    message(STATUS "grpc++: GRPC_LIB:${GRPC_LIB}, GRPC_INC:${GRPC_INC}, GRPC_CPP_GEN:${GRPC_CPP_GEN}")
endif(NOT GRPC_LIB OR NOT GRPC_INC OR NOT GRPC_CPP_GEN)
