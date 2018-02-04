#!/usr/bin/env sh

BUILD_PATH="../cmake-build-release/"

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 FILE.clfr"
    exit 1;
fi

$BUILD_PATH/packet_counts --benchmark --input $1 > output.dat
gnuplot plot.gp
