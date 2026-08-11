cmake_minimum_required(VERSION 3.22)
project(zuc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------------------
# Main test executable
# -----------------------------------------------------------------------------
add_executable(zuc_test 
    test.cpp
    test_container_kits.cpp
    test_span_kits.cpp
    test_time_kits.cpp
    test_string_kits.cpp
    test_io_kits.cpp
    test_random_kits.cpp
    test_common_concepts.cpp
    test_numerics_kits.cpp
)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping -O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
add_test(NAME zuc_test COMMAND zuc_test)