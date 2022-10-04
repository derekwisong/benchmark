include(CTest)
enable_testing()

set (CMAKE_CXX_STANDARD 17)

add_executable(TestGtest
    test_bench.cpp
)

target_link_libraries(TestGtest
    bench
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TestGtest)
