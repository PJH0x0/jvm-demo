cmake_minimum_required(VERSION 3.11.0)
project(jvm-demo-test LANGUAGES C CXX)

#list(APPEND cxx_flags "-fPIC" "-std=c++20" "-O0" "-Wc++11-extensions")
set(CMAKE_CXX_STANDARD 11)
set(POSITION_INDEPENDENT_CODE ON)

#include(${CMAKE_CURRENT_LIST_DIR}/../src/CMakeLists.txt)
#include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/include)
option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
  # Prevent GoogleTest from overriding our compiler/linker options
  # when building with Visual Studio
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Prevent GoogleTest from using PThreads
  set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
  # adds the targers: gtest, gtest_main, gmock, gmock_main
  add_subdirectory(
    googletest
  )
  include_directories(googletest/googletest/include)

  # Silence std::tr1 warning on MSVC
  if(MSVC)
    foreach(_tgt gtest gtest_main gmock gmock_main)
      target_compile_definitions(${_tgt}
        PRIVATE
        "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
      )
    endforeach()
  endif()

  find_package (Threads)
  add_compile_definitions(TEST_PATH="${PROJECT_SOURCE_DIR}")

  file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.cpp )
  add_executable(jvm_test ${sources})
  target_link_libraries(jvm_test
    PRIVATE
    java-test
    gtest_main
    pthread
  )
  enable_testing()

  add_test(
    NAME google_test
    COMMAND $<TARGET_FILE:jvm_test>
  )
endif()