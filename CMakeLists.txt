cmake_minimum_required(VERSION 3.0.0)
project(benchmark VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

IF (WIN32)
  # set stuff for windows

  # clang
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -O1")

  # msvc
  #set(CMAKE_CXX_FLAGS "/O1 /EHsc")
  #set(CMAKE_CXX_FLAGS_DEBUG "/Z7")
ELSE()
# set stuff for other systems
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -O1")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
ENDIF()

set(CMAKE_CXX_FLAGS_RELEASE "")


set(HEADER_FILES include/benchmarks.h)

add_library(benchmarks src/benchmarks.cpp ${HEADER_FILES})
target_include_directories(benchmarks PRIVATE include)
set_property(TARGET benchmarks PROPERTY CXX_STANDARD 17)

add_executable(benchmark src/main.cpp ${HEADER_FILES})
target_link_libraries(benchmark benchmarks)
target_include_directories(benchmark PRIVATE include)
set_property(TARGET benchmark PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()