cmake_minimum_required (VERSION 3.7)
project (HikariBackend)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wmissing-declarations -Wmissing-noreturn")

add_subdirectory(src)

# benchmarks
add_subdirectory(benchmark)