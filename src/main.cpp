#include <bits/chrono.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <tuple>

#include <sys/stat.h>

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

template <size_t size> void malloc_free() {
  auto ptr = std::malloc(size);
  std::free(ptr);
}

template <typename Type> void new_delete() {
  auto obj = new Type();
  delete obj;
}

template <typename Type, size_t count> void new_delete_a() {
  auto obj = new Type[count];
  delete[] obj;
}

template <int val> void square() {
  int result = val * val;
  (void)result;
}

void random_int() {
  auto val = std::rand();
  (void)val;
}

void random_double() {
  auto val = static_cast<double>(std::rand()) / RAND_MAX;
  (void)val;
}

template <typename Ptr> void random_access(const Ptr *array, size_t size) {
  auto val = array[std::rand() % size];
  (void)val;
}

void open_close_fstream(const char *path) {
  std::fstream file;
  file.open(path, std::ios::in);
  file.close();
}

void open_close_fopen(const char *path) {
  FILE *file = fopen(path, "rb");
  fclose(file);
}

void stat_file(const char *path) {
  struct stat st;
  stat(path, &st);
}

template <typename Func> std::tuple<double, double, double> repeat_for_time(Func &&func, unsigned int milliseconds) {
  namespace chrono = std::chrono;
  auto start = chrono::system_clock::now();
  auto scheduled_duration = chrono::milliseconds(milliseconds);
  auto scheduled_end = start + scheduled_duration;
  size_t counter = 0;

  while (chrono::system_clock::now() < scheduled_end) {
    func();
    ++counter;
  }

  auto actual_end = chrono::system_clock::now();
  auto actual_duration = actual_end - start;
  auto seconds = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(actual_duration).count()) / 1000;

  double calls_per_second = static_cast<double>(counter) / seconds;
  double seconds_per_call = static_cast<double>(seconds) / counter;
  double scaled_counter = static_cast<double>(counter) / (actual_duration / scheduled_duration);

  return std::make_tuple(scaled_counter, calls_per_second, seconds_per_call);
}

std::string format_cps(double cps) {
  static const std::map<double, std::string> scales = {
      {1e3, "ms"},
      {1e6, "us"},
      {1e9, "ns"},
  };

  for (auto iter = scales.rbegin(); iter != scales.rend(); ++iter) {
    const auto scale = iter->first;
    const auto &name = iter->second;
    if (cps >= scale) {
      return std::to_string(cps / scale) + " calls/" + name;
    }
  }

  return std::to_string(cps) + " calls/sec";
}

std::string format_spc(double spc) {
  static const std::map<double, std::string> scales = {
      {1e-9, "ns"},
      {1e-6, "us"},
      {1e-3, "ms"},
  };

  for (auto iter = scales.begin(); iter != scales.end(); ++iter) {
    const auto scale = iter->first;
    const auto &name = iter->second;
    if (spc <= scale) {
      return std::to_string(spc / scale) + " " + name + "/call";
    }
  }

  return std::to_string(spc) + " sec/call";
}

template <typename Func> void benchmark(unsigned int milliseconds, std::string description, Func &&func) {
  auto [count, calls_per_sec, sec_per_call] = repeat_for_time(func, milliseconds);
  std::cout << std::left << std::setw(35) << description << std::right << std::setw(15) << count << " calls in "
            << milliseconds << "ms " << std::right << std::setw(25) << format_cps(calls_per_sec) << std::right
            << std::setw(25) << format_spc(sec_per_call) << std::endl;
}

int main(int, char **argv) {
  std::srand(std::time(nullptr));
  // runtime of the test, repeat the benchmark as may times in this many milliseconds
  static const unsigned int RUNTIME = 100;
  // for tests that allocate integer array
  using array_t = int;
  static const size_t NUM_INTS = 1000000;

  benchmark(RUNTIME, "malloc/free (2 doubles)", malloc_free<sizeof(TwoDoubles)>);
  benchmark(RUNTIME, "new/delete (2 doubles)", new_delete<TwoDoubles>);
  benchmark(RUNTIME, "malloc/free (" + std::to_string(NUM_INTS) + " ints)", malloc_free<sizeof(array_t) * NUM_INTS>);
  benchmark(RUNTIME, "new/delete (" + std::to_string(NUM_INTS) +" ints)", new_delete_a<array_t, NUM_INTS>);
  benchmark(RUNTIME, "square int", square<20>);
  benchmark(RUNTIME, "random int", random_int);
  benchmark(RUNTIME, "random double", random_double);
  benchmark(RUNTIME, "open/close file (fstream)", [f = argv[0]] { open_close_fstream(f); });
  benchmark(RUNTIME, "open/close file (fopen)", [f = argv[0]] { open_close_fopen(f); });
  benchmark(RUNTIME, "stat file", [f = argv[0]] { stat_file(f); });
  benchmark(RUNTIME, "fill array of " + std::to_string(NUM_INTS) + " ints",
            [integers = Array<array_t>(NUM_INTS).array] { std::fill(integers, integers + NUM_INTS, 100); });
  benchmark(RUNTIME, "random access from " + std::to_string(NUM_INTS) + " ints",
            [integers = Array<array_t>(NUM_INTS).array] { random_access(integers, NUM_INTS); });
  benchmark(RUNTIME, "sleep 2 seconds", [] { std::this_thread::sleep_for(std::chrono::seconds(2)); });
}
