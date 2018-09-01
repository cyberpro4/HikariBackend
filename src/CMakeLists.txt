set (CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/lib/json/include ${CMAKE_SOURCE_DIR}/lib/logging)
include_directories(config/include)
include_directories(http/include)

add_subdirectory(config)
add_subdirectory(http)

add_executable(HikariBackend ../lib/logging/rickycorte/Logging.hpp HikariStaticConfig.hpp Source.cpp)

target_link_libraries(HikariBackend HikariConfig)
target_link_libraries(HikariBackend HikariHttp)