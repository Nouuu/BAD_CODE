cmake_minimum_required(VERSION 3.15)
project(BAD_CODE C)

set(CMAKE_C_STANDARD 11)
add_executable(BAD_CODE main.c)

target_link_libraries(BAD_CODE LINK_PUBLIC sqlite3)
