#!/bin/bash

CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
echo "Current directory: $CURRENT_DIR"

export CMAKE_PREFIX_PATH=${CURRENT_DIR}/dawn/install/Release

pushd "$CURRENT_DIR"
    cmake -S . -B out/Release -DCMAKE_BUILD_TYPE=Release
    cmake --build out/Release
popd

pushd "$CURRENT_DIR/out/Release"
    echo "Running MyDemo..."
    echo
    echo
    echo
    ./MyDemo
popd