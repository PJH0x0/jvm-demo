cmake_minimum_required(VERSION 3.11.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
project(jvm-demo LANGUAGES C CXX)

add_subdirectory(libs/ZipLib)

add_executable(java "")
add_library(java-test SHARED "")

#aux_source_directory(${CMAKE_CURRENT_LIST_DIR} source)
#aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/include header)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp src/*.c)
file(GLOB_RECURSE headers CONFIGURE_DEPENDS src/*.h)

target_sources(java-test
    PRIVATE
    ${sources}
    
    PUBLIC
    ${headers})
target_sources(java
    PRIVATE
    ${sources}
    ${headers})
target_include_directories(java-test
    PUBLIC
    ${headers})
target_include_directories(java
    PUBLIC
    ${headers})
target_link_libraries(java-test PUBLIC zip)
target_link_libraries(java PUBLIC zip)
enable_testing()
add_subdirectory(tests)