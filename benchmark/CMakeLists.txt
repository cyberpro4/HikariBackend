
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(HttpRequestBenchmark ${CMAKE_SOURCE_DIR}/src/http/Request.hpp ${CMAKE_SOURCE_DIR}/src/http/Request.cpp http_request_parse_bench.cpp)