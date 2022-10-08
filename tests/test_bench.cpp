#include "bench.h"

#include <gtest/gtest.h>

#include <chrono>

TEST(TestBench, TestRunLambdaOnce) {
  int index = 0;
  auto lambda = [&index] { ++index; };
  bench::Benchmark benchmark{"Lambda", lambda};
  benchmark.run();
  EXPECT_EQ(index, 1);
}

TEST(TestBench, TestRunFunctorOnce) {
  class Functor {
  public:
    size_t counter = 0;
    void operator()() { ++counter; }
  } functor;

  bench::Benchmark benchmark{"Functor", functor};
  EXPECT_EQ(functor.counter, 0);
  benchmark.run();
  EXPECT_EQ(functor.counter, 1);
}

TEST(TestBenchRunner, TestRunFor1ms) {
  // run a no-op benchmark as many times as possible in 1 millisecond
  // ensure that it ran for at least 1 millisecond and for no longer
  // than an extra 100 microseconds
  auto test_time = std::chrono::milliseconds(1);
  bench::BenchmarkRunner runner(test_time.count());
  auto start = std::chrono::steady_clock::now();
  auto results = runner.run("no-op", [] {});
  auto end = std::chrono::steady_clock::now();
  auto runtime = end - start;
  // expect to be able to call the no op function at least 100 times (generally expect much higher)
  EXPECT_GE(results.counter, 100);
  // runtime took at least the expected test time
  EXPECT_GE(runtime, test_time);
  // assert that the runtime is within 100 microseconds of the expected 1 millisecond runtime
  EXPECT_LT(runtime - test_time, std::chrono::microseconds(100));
}