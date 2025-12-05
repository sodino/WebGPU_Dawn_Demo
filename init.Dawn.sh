#!/bin/bash

# 初始化 Dawn 项目环境
if [ -f "dawn/out/Debug/HelloTriangle" ]; then
    echo "Dawn environment already initialized."
elif
    rm -rf "dawn" "out"
    git clone https://dawn.googlesource.com/dawn    
fi

pushd "dawn"
    python3 tools/fetch_dawn_dependencies.py
    mkdir -p out/Debug
    pushd out/Debug
        cmake -GNinja ../.. # 使用 Ninja 生成构建系统
        ninja # 编译

        echo "Open HelloTriangle example : "
        if [ -f "./HelloTriangle" ]; then
            echo "Executable found."
            ./HelloTriangle
        else
            echo "Executable not found!"
            exit 1
        fi
    popd


    ## Release 构建，为TestDawn充当依赖
    cmake -S . -B out/Release -DDAWN_FETCH_DEPENDENCIES=ON -DDAWN_ENABLE_INSTALL=ON -DCMAKE_BUILD_TYPE=Release
    cmake --build out/Release
    cmake --install out/Release --prefix install/Release
popd
