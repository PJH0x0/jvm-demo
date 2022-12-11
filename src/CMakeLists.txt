add_executable(java "")
add_library(java-test STATIC "")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


aux_source_directory(${CMAKE_CURRENT_LIST_DIR} source)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/include header)
set(zip_lib_header "../libs/ZipLib")
list(APPEND jvm_source 
    ${source} 
    $<TARGET_OBJECTS:SRC_LZMA-objs> 
    $<TARGET_OBJECTS:SRC_BZIP2-objs>
    $<TARGET_OBJECTS:SRC_ZLIB-objs> 
    $<TARGET_OBJECTS:SRC_LIB_ZIP-objs>)
target_sources(java-test
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
target_include_directories(java-test
    PUBLIC
    ${header}
    ${zip_lib_header}
    )
target_include_directories(java
    PUBLIC
    ${header}
    ${zip_lib_header})
# target_link_libraries(java 
#     PRIVATE
#     pthread)

