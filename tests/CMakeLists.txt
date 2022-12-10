cmake_minimum_required(VERSION 3.11.0)
project(jvm-demo-test LANGUAGES C CXX)

list(APPEND cxx_flags "-fPIC" "-std=c++11" "-O0" "-Wc++11-extensions")

#include(${CMAKE_CURRENT_LIST_DIR}/../src/CMakeLists.txt)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/include)
option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
  )
  FetchContent_GetProperties(googletest)

  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Prevent GoogleTest from using PThreads
    set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

    # adds the targers: gtest, gtest_main, gmock, gmock_main
    add_subdirectory(
      ${googletest_SOURCE_DIR}
      ${googletest_BINARY_DIR}
    )

    # Silence std::tr1 warning on MSVC
    if(MSVC)
      foreach(_tgt gtest gtest_main gmock gmock_main)
        target_compile_definitions(${_tgt}
          PRIVATE
          "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
        )
      endforeach()
    endif()
  endif()

  add_executable(jvm-demo-test "")

  target_sources(jvm-demo-test
    PRIVATE
    test.cpp
  )
  target_compile_options(jvm-demo-test
    PRIVATE
    ${cxx_flags}
  )
  target_link_libraries(jvm-demo-test
    PRIVATE
    java-lib
    gtest_main
  )
  enable_testing()

  add_test(
    NAME google_test
    COMMAND $<TARGET_FILE:jvm-demo-test>
  )
endif()