cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(zuc)

# Main executable
add_executable(zuc_test test.cpp)
target_include_directories(zuc_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Enable testing
add_test(NAME zucTests COMMAND zuc_test)