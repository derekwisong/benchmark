#pragma once

#include "bench/display.h"

// include the rest of the benchmark headers for convenience to users
#include "bench/cpu.h"
#include "bench/fileio.h"
#include "bench/memory.h"

namespace bench {

template <typename Func> class Benchmark {
  const std::string name;
  Func &&func;

public:
  Benchmark(std::string name, Func &&func) : name(std::move(name)), func(std::move(func)) {}
  const std::string &get_name() const { return name; }
  inline void run() const { func(); };
  void operator()() const { run(); }
};

class BenchmarkRunner {
  int runtime_ms;

public:
  BenchmarkRunner(int runtime_ms) : runtime_ms(runtime_ms) {}
  BenchmarkRunner() : BenchmarkRunner(1000){};

  template <typename Func> void run(std::string description, Func &&func) {
    run(Benchmark{std::move(description), std::move(func)});
  }

  template <typename Func> void run(const Benchmark<Func> &benchmark) {
    auto results = bench::timing::repeat_for_time(benchmark, runtime_ms);
    bench::display::print_run_results(benchmark.get_name(), runtime_ms, results);
  }
};

} // namespace bench