cmake_minimum_required(VERSION 3.7)
project(exchrate)

include_directories(/usr/include/postgresql)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c spider.h spider.c global.h analyst.h analyst.c util.h util.c)
add_executable(exchrate ${SOURCE_FILES})

target_link_libraries(exchrate curl)
target_link_libraries(exchrate pq)
