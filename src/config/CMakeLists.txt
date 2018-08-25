
include_directories(${CMAKE_SOURCE_DIR}/lib/json/include ${CMAKE_SOURCE_DIR}/src/include)
include_directories(include)

add_library(HikariConfig include/config/ConfigFile.h ConfigFile.cpp)