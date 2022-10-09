#pragma once

#include "bench/display.h"

#include "bench/timing.h"

namespace bench {

template <typename Func> class Benchmark {
  const std::string name;
  Func&& func;

public:
  Benchmark(std::string name, Func&& func) : name(std::move(name)), func(std::forward<Func>(func)) {}
  Benchmark(std::string name, Func& func) : name(std::move(name)), func(std::forward<Func>(func)) {}
  const std::string& get_name() const { return name; }
  void run() const { func(); };
  void operator()() const { run(); }
};

class BenchmarkRunner {
  static const int DEFAULT_RUNTIME_MS = 100;
  int runtime_ms;

public:
  BenchmarkRunner(int runtime_ms) : runtime_ms(runtime_ms) {}
  BenchmarkRunner() : BenchmarkRunner(DEFAULT_RUNTIME_MS){};

  template <typename Func> bench::timing::TimedRunResults run(std::string description, Func&& func) {
    return run(Benchmark{std::move(description), std::forward<Func>(func)});
  }

  template <typename Func> bench::timing::TimedRunResults run(const Benchmark<Func>& benchmark, bool display = true) {
    auto results = bench::timing::repeat_for_time(benchmark, runtime_ms);
    if (display) {
      bench::display::print_run_results(benchmark.get_name(), runtime_ms, results);
    }
    return results;
  }
};

} // namespace bench