#cmake_minimum_required(VERSION 3.25.0)
#project(libzip LANGUAGES CXX C)

message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
list(APPEND c_flags "-fPIC" "-Wno-enum-conversion" "-O3")
list(APPEND cxx_flags "-fPIC" "-std=c++11" "-O3")
list(APPEND ld_flags "-pthread")



file(GLOB SRC_ZLIB ${CMAKE_CURRENT_LIST_DIR}/extlibs/zlib/*.c)
add_library(SRC_ZLIB-objs
OBJECT
${SRC_ZLIB}
)
target_compile_options(SRC_ZLIB-objs
  PRIVATE
    ${c_flags}
)
file(GLOB SRC_LZMA ${CMAKE_CURRENT_LIST_DIR}/extlibs/lzma/unix/*.c)
add_library(SRC_LZMA-objs
OBJECT
${SRC_LZMA}
)
target_compile_options(SRC_LZMA-objs
  PRIVATE
    ${c_flags}
)
file(GLOB SRC_BZIP2 ${CMAKE_CURRENT_LIST_DIR}/extlibs/bzip2/*.c)
add_library(SRC_BZIP2-objs
OBJECT
${SRC_BZIP2}
)
target_compile_options(SRC_BZIP2-objs
  PRIVATE
    ${c_flags}
)
file(GLOB SRC_LIB_ZIP ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/detail/*.cpp)
add_library(SRC_LIB_ZIP-objs
OBJECT
${SRC_LIB_ZIP}
)
target_compile_options(SRC_LIB_ZIP-objs
  PRIVATE
    ${cxx_flags}
)


find_package (Threads)

add_library(zip-shared
SHARED
$<TARGET_OBJECTS:SRC_LZMA-objs>
$<TARGET_OBJECTS:SRC_BZIP2-objs>
$<TARGET_OBJECTS:SRC_ZLIB-objs>
$<TARGET_OBJECTS:SRC_LIB_ZIP-objs>)
target_link_libraries(zip-shared ${ld_flags})
