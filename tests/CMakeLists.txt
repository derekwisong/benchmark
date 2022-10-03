include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 17)

add_executable(TestStub test_stub.cpp)
add_test(NAME TestStub COMMAND TestStub)

add_executable(TestBench test_bench.cpp)
target_link_libraries(TestBench bench)
add_test(NAME TestBench COMMAND TestBench)