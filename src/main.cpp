#include "bench.h"
#include "bench/cpu.h"
#include <chrono>
#include <span>
#include <vector>

struct TwoDoubles {
  double x = 0;
  double y = 0;
};

void randomize_seed() { std::srand(std::time(nullptr)); }

int main(int argc, char** argv) {
  const auto args = std::span(argv, static_cast<size_t>(argc));
  randomize_seed();

  // a vector of ints that will be used for a few benchmarks
  static const size_t NUM_INTS = 1'000'000;
  static const std::string INTS_DESC = "a million ints";
  static const int INT_FILL = 100;
  auto integers = std::vector<int>(NUM_INTS);

  // convenient named constants
  static const unsigned int BENCHMARK_RUNTIME = 100; // milliseconds
  static const char* THIS_FILE = args[0];
  static const std::chrono::duration ONE_NANO = std::chrono::nanoseconds(1);
  static const std::chrono::duration ONE_MICRO = std::chrono::microseconds(1);
  static const std::chrono::duration HUNDRED_MICROS = std::chrono::microseconds(100);
  static const std::chrono::duration ONE_MILLI = std::chrono::milliseconds(1);
  static const std::chrono::duration TWO_SEC = std::chrono::seconds(2);

  bench::BenchmarkRunner runner(BENCHMARK_RUNTIME);
  runner.run("lambda no-op", [] {});
  runner.run("sleep 2 seconds", bench::cpu::Sleeper(TWO_SEC));
  runner.run("sleep 1 ms", bench::cpu::Sleeper(ONE_MILLI));
  runner.run("sleep 100 us", bench::cpu::Sleeper(HUNDRED_MICROS));
  runner.run("busyloop 100 us", bench::cpu::BusyLoop(HUNDRED_MICROS));
  runner.run("sleep 1 us", bench::cpu::Sleeper(ONE_MICRO));
  runner.run("busyloop 1 us", bench::cpu::BusyLoop(ONE_MICRO));
  runner.run("sleep 1 ns", bench::cpu::Sleeper(ONE_NANO));
  runner.run("busyloop 1 ns", bench::cpu::BusyLoop(ONE_NANO));
  runner.run("malloc/free (2 doubles)", bench::memory::malloc_free<sizeof(TwoDoubles)>);
  runner.run("new/delete (2 doubles)", bench::memory::new_delete<TwoDoubles>);
  runner.run("malloc/free (" + INTS_DESC + ")", bench::memory::malloc_free<sizeof(int) * NUM_INTS>);
  runner.run("new/delete (" + INTS_DESC + ")", bench::memory::new_delete_a<int, NUM_INTS>);
  runner.run("square int", bench::cpu::square<2>);
  runner.run("draw random int", bench::cpu::random_int);
  runner.run("draw random double", bench::cpu::random_double);
  runner.run("open/close file (fstream)", [&] { bench::fileio::open_close_fstream(THIS_FILE); });
  runner.run("open/close file (fopen)", [&] { bench::fileio::open_close_fopen(THIS_FILE); });
  runner.run("stat file", [&] { bench::fileio::stat_file(THIS_FILE); });
  runner.run("fill array of " + INTS_DESC, [&] { std::fill(integers.begin(), integers.end(), INT_FILL); });
  runner.run("const random access from " + INTS_DESC,
             [&] { bench::memory::random_access(integers.data(), NUM_INTS); });
  runner.run(".size() random access from " + INTS_DESC,
             [&] { bench::memory::random_access(integers.data(), integers.size()); });
  runner.run(".size() random draw from " + INTS_DESC,
             [&] { bench::memory::random_draw(integers.data(), integers.size()); });
  runner.run(".size() random draw from span of " + INTS_DESC,
             [&] { bench::memory::random_span_draw(integers.data(), integers.size()); });
}
