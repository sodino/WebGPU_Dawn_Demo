cmake_minimum_required(VERSION 3.13)

project(MyDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json by default so tools (like clangd / VS Code C++ ext)
# can use exact compile flags for indexing and "go to definition".
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(Threads REQUIRED)
find_package(Dawn REQUIRED)
add_executable(${PROJECT_NAME} hello_webgpu.cpp)

# Declare dependency on the dawn::webgpu_dawn library
target_link_libraries(${PROJECT_NAME} dawn::webgpu_dawn)

