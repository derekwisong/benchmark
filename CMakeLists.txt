cmake_minimum_required(VERSION 3.0.0)
project(benchmark VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

IF (WIN32)
  # clang
  set(CFLAGS_NO_OPTIMIZATION "-Wall -Wextra -O0")
  set(CFLAGS_OPTIMIZED "-Wall -Wextra -O3")
  # msvc
  #set(CMAKE_CXX_FLAGS /O1 /EHsc)
  #set(CMAKE_CXX_FLAGS_DEBUG /Z7)
ELSE()
  set(CFLAGS_NO_OPTIMIZATION "-Wall -Wextra -O0")
  set(CFLAGS_OPTIMIZED "-Wall -Wextra -O3")
ENDIF()
  
set(CMAKE_CXX_FLAGS_DEBUG "-g ${CFLAGS_NO_OPTIMIZATION}")
set(CMAKE_CXX_FLAGS_RELEASE ${CFLAGS_OPTIMIZED})


set(HEADER_FILES
  "include/bench.h"
  "include/bench/display.h"
  "include/bench/timing.h"
  )
  
add_library(bench
  "src/bench.cpp"
  "src/bench/display.cpp"
  "src/bench/timing.cpp"
  ${HEADER_FILES})

target_include_directories(bench PUBLIC include)
set_property(TARGET bench PROPERTY CXX_STANDARD 20)

set(BENCHMARKS_HEADER_FILES
  "include/bench/benchmarks/cpu.h"
  "include/bench/benchmarks/fileio.h"
  "include/bench/benchmarks/memory.h"
  )

add_library(benchmarks
  "src/bench/benchmarks/cpu.cpp"
  "src/bench/benchmarks/fileio.cpp"
  "src/bench/benchmarks/memory.cpp"
  ${BENCHMARKS_HEADER_FILES})

target_include_directories(benchmarks PUBLIC include)
# always compile the benchmarks sample library without optimization
target_compile_options(benchmarks PUBLIC "-O0")
set_property(TARGET benchmarks PROPERTY CXX_STANDARD 20)

# The benchmark executable is a sample application

add_executable(benchmark src/main.cpp)
target_link_libraries(benchmark
  bench
  benchmarks
  )
target_include_directories(benchmark PRIVATE include)
set_property(TARGET benchmark PROPERTY CXX_STANDARD 20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()

add_subdirectory(vendor)
add_subdirectory(tests)
