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

add_executable(benchmark main.cpp)
set_property(TARGET benchmark PROPERTY CXX_STANDARD 17)

