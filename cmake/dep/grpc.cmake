
find_package(PkgConfig REQUIRED)
pkg_search_module(GRPC REQUIRED grpc++)
message(STATUS "libgrpc ${GRPC_VERSION}: ${GRPC_LIBRARY_DIRS}, include: ${GRPC_INCLUDE_DIRS}")
