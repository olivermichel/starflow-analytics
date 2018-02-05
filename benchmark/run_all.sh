#!/usr/bin/env sh

BUILD_PATH="cmake-build-release"

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 FILE.clfr"
    exit 1;
fi

$BUILD_PATH/active_connections --benchmark --input $1 > out/packet_counts.dat
$BUILD_PATH/ewma_latencies --benchmark --input $1 > out/ewma_latencies.dat
$BUILD_PATH/flowlet_size_histogram --benchmark --input $1 > out/flowlet_size_histogram.dat
$BUILD_PATH/packet_counts --benchmark --input $1 > out/packet_counts.dat
$BUILD_PATH/passthrough --benchmark --input $1 > out/passthrough.dat
$BUILD_PATH/tcp_non_monotonic --benchmark --input $1 > out/tcp_non_monotonic.dat
$BUILD_PATH/tcp_out_of_sequence --benchmark --input $1 > out/tcp_out_of_sequence.dat
