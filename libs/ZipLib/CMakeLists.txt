#cmake_minimum_required(VERSION 3.25.0)
#project(libzip LANGUAGES CXX C)

#list(APPEND c_flags "-fPIC" "-Wno-enum-conversion" "-O3")
#list(APPEND cxx_flags "-fPIC" "-std=c++11" "-O3")
#list(APPEND ld_flags "-pthread")
#include_directories(${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-enum-conversion -O3 -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread")

file(GLOB SRC_ZLIB ${CMAKE_CURRENT_LIST_DIR}/extlibs/zlib/*.c)
file(GLOB SRC_LZMA ${CMAKE_CURRENT_LIST_DIR}/extlibs/lzma/unix/*.c)
file(GLOB SRC_BZIP2 ${CMAKE_CURRENT_LIST_DIR}/extlibs/bzip2/*.c)
file(GLOB SRC_LIB_ZIP ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/detail/*.cpp)
# find_package (Threads)

add_library(zip
SHARED
${SRC_LIB_ZIP}
${SRC_LZMA}
${SRC_BZIP2}
${SRC_ZLIB})
target_include_directories(zip PUBLIC ${CMAKE_CURRENT_LIST_DIR})
#set_target_properties(zip PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

