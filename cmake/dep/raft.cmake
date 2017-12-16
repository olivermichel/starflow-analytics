
find_library(RAFT_LIB protobuf)
find_file(RAFT_INC google/protobuf)

if(NOT RAFT_LIB OR NOT RAFT_INC)
    message(WARNING "raft: not found")
else()
    message(STATUS "raft: RAFT_LIB:${RAFT_LIB}, RAFT_INC:${RAFT_INC}")
endif(NOT RAFT_LIB OR NOT RAFT_INC)
