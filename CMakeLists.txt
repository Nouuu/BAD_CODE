cmake_minimum_required(VERSION 3.1)
project(BAD_CODE LANGUAGES C)
enable_language(C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_LINK_SEARCH_START_STATIC TRUE)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_C_FLAGS -Werror)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_definitions(${GTK_CFLAGS_OTHER})

add_executable(BAD_CODE source/main.c source/headers/functions.c source/headers/sqliteFunctions.c source/headers/gtkFunctions.c)


target_link_libraries(BAD_CODE LINK_PUBLIC sqlite3)
target_link_libraries(BAD_CODE LINK_PUBLIC ${GTK_LIBRARIES})