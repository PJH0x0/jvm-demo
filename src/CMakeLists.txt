add_executable(java "")
list(APPEND cxx_flags "-fPIC" "-std=c++11" "-O3")
target_compile_options(java
    PRIVATE
    ${cxx_flags}
)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../libs/ZipLib)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} source)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/include header)
target_sources(java 
    PRIVATE
    ${source}
    $<TARGET_OBJECTS:SRC_LZMA-objs>
    $<TARGET_OBJECTS:SRC_BZIP2-objs>
    $<TARGET_OBJECTS:SRC_ZLIB-objs>
    $<TARGET_OBJECTS:SRC_LIB_ZIP-objs>
    PUBLIC
    ${header}
    )
target_include_directories(java
    PUBLIC
    $(CMAKE_CURRENT_LIST_DIR)/include)

#target_link_libraries(java PRIVATE libzip-shared)