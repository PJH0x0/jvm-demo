add_executable(java "")
add_library(java-lib STATIC "")
list(APPEND cxx_flags "-fPIC" "-std=c++20" "-O0")
target_compile_options(java-lib
    PRIVATE
    ${cxx_flags}
)
target_compile_options(java
    PRIVATE
    ${cxx_flags}
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZipLib)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} source)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/include header)
list(APPEND jvm_source 
    ${source} 
    $<TARGET_OBJECTS:SRC_LZMA-objs> 
    $<TARGET_OBJECTS:SRC_BZIP2-objs>
    $<TARGET_OBJECTS:SRC_ZLIB-objs> 
    $<TARGET_OBJECTS:SRC_LIB_ZIP-objs>)
target_sources(java-lib
    PRIVATE
    ${jvm_source}
    
    PUBLIC
    ${header}
)
target_sources(java
    PRIVATE
    ${jvm_source}
    PUBLIC
    ${header}
)
target_include_directories(java-lib
    PUBLIC
    ${header})
target_include_directories(java
    PUBLIC
    ${header})
target_link_libraries(java 
    PRIVATE
    pthread)

