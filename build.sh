#!/bin/bash

CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
echo "Current directory: $CURRENT_DIR"

export CMAKE_PREFIX_PATH=${CURRENT_DIR}/dawn/install/Release

pushd "$CURRENT_DIR"
    cmake -S . -B out/Release -DCMAKE_BUILD_TYPE=Release || {
        echo "❌cmake prepare failed."
        exit 1
    }
    
    cmake --build out/Release || {
        echo "❌cmake --build failed."
        exit 1
    }
    
popd

pushd "$CURRENT_DIR/out/Release"
    echo "Running MyDemo..."
    echo
    echo
    echo
    ./MyDemo
popd