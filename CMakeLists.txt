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


set(HEADER_FILES
  "include/bench.h"
  "include/bench/cpu.h"
  "include/bench/display.h"
  "include/bench/fileio.h"
  "include/bench/memory.h"
  "include/bench/timing.h"
  )


add_library(bench
  "src/bench.cpp"
  "src/bench/cpu.cpp"
  "src/bench/display.cpp"
  "src/bench/fileio.cpp"
  "src/bench/memory.cpp"
  "src/bench/timing.cpp"
  ${HEADER_FILES})

target_include_directories(bench PRIVATE include)
set_property(TARGET bench PROPERTY CXX_STANDARD 17)

add_executable(benchmark src/main.cpp ${HEADER_FILES})
target_link_libraries(benchmark bench)
target_include_directories(benchmark PRIVATE include)
set_property(TARGET benchmark PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()