cmake_minimum_required(VERSION 3.11.0)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE Debug)
project(jvm-demo LANGUAGES C CXX)
set(WITH_GFLAGS OFF)
add_subdirectory(libs/ZipLib)
add_subdirectory(libs/glog)

add_executable(java "")
add_library(java-test SHARED "")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#aux_source_directory(${CMAKE_CURRENT_LIST_DIR} source)
#aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/include header)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.c)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS src/*.h)

add_compile_definitions(INST_DEBUG=0)

target_sources(java-test
    PRIVATE
    ${sources})
target_sources(java
    PRIVATE
    ${sources})
target_include_directories(java-test
    PUBLIC
    src/)
target_include_directories(java
    PUBLIC
    src/)
target_link_libraries(java-test PUBLIC zip glog)
target_link_libraries(java PUBLIC zip glog)
enable_testing()
add_subdirectory(tests)
