include_directories(${CMAKE_SOURCE_DIR}/lib/logging)
include_directories(include)

add_library(HikariConfig STATIC src/Config.cpp src/ConfigFile.cpp)
