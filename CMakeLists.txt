cmake_minimum_required(VERSION 3.1)
project(BAD_CODE C)

set(CMAKE_C_STANDARD 11)

#set(CMAKE_EXE_LINKER_FLAGS -static)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_definitions(${GTK_CFLAGS_OTHER})


add_executable(BAD_CODE main.c headers/functions.c headers/sqliteFunctions.c headers/ftkFunctions.c)


target_link_libraries(BAD_CODE LINK_PUBLIC sqlite3)
target_link_libraries(BAD_CODE LINK_PUBLIC ${GTK_LIBRARIES})