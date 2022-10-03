#include "bench.h"

#include <thread>

struct TwoDoubles {
  double x = 0;
  double y = 0;
};

template <typename Type> class Array {
public:
  Type *const array;

  Array(size_t size) : array(new Type[size]) {}
  ~Array() { delete[] array; }
};

int main(int, char **argv) {
  std::srand(std::time(nullptr));
  // runtime of the test, repeat the benchmark as may times in this many
  // milliseconds
  static const unsigned int RUNTIME = 100;
  // for tests that allocate integer array
  using array_t = int;
  static const size_t NUM_INTS = 1000000;
  auto array_description = std::to_string(NUM_INTS) + " ints";
  auto integers = Array<array_t>(NUM_INTS);
  auto array_filler = [&integers] { std::fill(integers.array, integers.array + NUM_INTS, 100); };
  auto random_access = [&integers] { bench::memory::random_access(integers.array, NUM_INTS); };

  bench::BenchmarkRunner runner(RUNTIME);
  runner.run("sleep 2 seconds", [] { std::this_thread::sleep_for(std::chrono::seconds(2)); });
  runner.run("sleep 1 ms", [] { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
  runner.run("sleep 100 us", [] { std::this_thread::sleep_for(std::chrono::microseconds(100)); });
  runner.run("busyloop 100 us", bench::cpu::BusyLoop(std::chrono::microseconds(100)));
  runner.run("sleep 1 us", [] { std::this_thread::sleep_for(std::chrono::microseconds(1)); });
  runner.run("busyloop 1 us", bench::cpu::BusyLoop(std::chrono::microseconds(1)));
  runner.run("sleep 1 ns", [] { std::this_thread::sleep_for(std::chrono::nanoseconds(1)); });
  runner.run("busyloop 1 ns", bench::cpu::BusyLoop(std::chrono::nanoseconds(1)));
  runner.run("malloc/free (2 doubles)", bench::memory::malloc_free<sizeof(TwoDoubles)>);
  runner.run("new/delete (2 doubles)", bench::memory::new_delete<TwoDoubles>);
  runner.run("malloc/free (" + array_description + " ints)", bench::memory::malloc_free<sizeof(array_t) * NUM_INTS>);
  runner.run("new/delete (" + array_description + " ints)", bench::memory::new_delete_a<array_t, NUM_INTS>);
  runner.run("square int", bench::cpu::square<20>);
  runner.run("draw random int", bench::cpu::random_int);
  runner.run("draw random double", bench::cpu::random_double);
  runner.run("open/close file (fstream)", [f = argv[0]] { bench::fileio::open_close_fstream(f); });
  runner.run("open/close file (fopen)", [f = argv[0]] { bench::fileio::open_close_fopen(f); });
  runner.run("stat file", [f = argv[0]] { bench::fileio::stat_file(f); });
  runner.run("fill array of " + array_description, array_filler);
  runner.run("random access from " + array_description, random_access);
}
