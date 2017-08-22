cmake_minimum_required(VERSION 3.7)
project(stkacc)

include_directories(/usr/include/postgresql)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c spider.h spider.c global.h util.h util.c analyst.h analyst.c)
add_executable(stkacc ${SOURCE_FILES})

target_link_libraries(stkacc curl)
target_link_libraries(stkacc pq)
