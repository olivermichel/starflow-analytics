#!/usr/bin/env sh

BUILD_PATH=${STARFLOW_BUILD_PATH:="build"}

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 FILE.clfr"
    exit 1;
fi

for i in 1 2 4 8 12
do
    a0[$i]=$($BUILD_PATH/parallel_test --benchmark --parallel $i --batch 1 --input $1)
    a50[$i]=$($BUILD_PATH/parallel_test --benchmark --parallel $i --batch 50 --input $1)
    a100[$i]=$($BUILD_PATH/parallel_test --benchmark --parallel $i --batch 100 --input $1)
    a500[$i]=$($BUILD_PATH/parallel_test --benchmark --parallel $i --batch 500 --input $1)
    echo "$i ${a0[$i]} ${a50[$i]} ${a100[$i]} ${a500[$i]}"
done
