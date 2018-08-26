set (CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR}/lib/json/include ${CMAKE_SOURCE_DIR}/lib/logging)
include_directories(config/include)

add_subdirectory(config)


add_executable(HikariBackend ../lib/logging/rickycorte/Logging.h HikariStaticConfig.h Source.cpp)

target_link_libraries(HikariBackend HikariConfig)