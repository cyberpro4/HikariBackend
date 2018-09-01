include_directories(${CMAKE_SOURCE_DIR}/lib/logging)
include_directories(include)


add_library(HikariHttp STATIC src/Server.cpp include/http/Server.hpp)

target_link_libraries(HikariHttp pthread)
