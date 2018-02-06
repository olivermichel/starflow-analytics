
add_executable(zip_test
        src/kernels/zip.cc
        src/app/zip_test_main.cc)

target_link_libraries(zip_test raft)
