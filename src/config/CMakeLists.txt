
include_directories(${CMAKE_SOURCE_DIR}/lib/json/include ${CMAKE_SOURCE_DIR}/lib/logging)
include_directories(include/config)

add_library(HikariConfig STATIC Config.cpp ConfigFile.cpp )