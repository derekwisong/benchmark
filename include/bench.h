#pragma once

#include "bench/display.h"

// include the rest of the benchmark headers for convenience to users
#include "bench/cpu.h"
#include "bench/fileio.h"
#include "bench/memory.h"

namespace bench {

class BenchmarkRunner {
  int runtime_ms;

public:
  BenchmarkRunner(int runtime_ms) : runtime_ms(runtime_ms) {}
  BenchmarkRunner() : BenchmarkRunner(1000){};

  template <typename Func> void run(std::string description, Func &&func) {
    auto results = bench::timing::repeat_for_time(func, runtime_ms);
    bench::display::print_run_results(description, runtime_ms, results);
  }
};

} // namespace bench