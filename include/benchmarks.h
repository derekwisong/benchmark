#pragma once

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>

namespace benchmarks {

namespace chrono = std::chrono;

std::string format_cps(double cps);
std::string format_spc(double spc);
void random_int();
void random_double();
void open_close_fstream(const char *path);
void open_close_fopen(const char *path);
void stat_file(const char *path);


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

template <typename Ptr> void random_access(const Ptr *array, size_t size) {
  auto val = array[std::rand() % size];
  (void)val;
}

template <typename Func> std::tuple<double, double, double> repeat_for_time(Func &&func, unsigned int milliseconds) {
  using chrono::duration_cast;
  using chrono::system_clock;

  auto start = system_clock::now();
  auto scheduled_duration = chrono::milliseconds(milliseconds);
  auto scheduled_end = start + scheduled_duration;
  size_t counter = 0;

  while (system_clock::now() < scheduled_end) {
    func();
    ++counter;
  }

  auto actual_end = system_clock::now();
  auto actual_duration = actual_end - start;
  auto actual_milliseconds = duration_cast<chrono::milliseconds>(actual_duration).count();
  auto seconds = static_cast<double>(actual_milliseconds) / 1000;

  double calls_per_second = static_cast<double>(counter) / seconds;
  double seconds_per_call = static_cast<double>(seconds) / counter;
  double scaled_counter = static_cast<double>(counter) / (actual_duration / scheduled_duration);

  return std::make_tuple(scaled_counter, calls_per_second, seconds_per_call);
}

class BenchmarkRunner {
  int runtime_ms;

public:
  BenchmarkRunner(int runtime_ms) : runtime_ms(runtime_ms) {}
  BenchmarkRunner() : BenchmarkRunner(1000){};

  template <typename Func> void run(std::string description, Func &&func) {
    auto [count, calls_per_sec, sec_per_call] = repeat_for_time(func, runtime_ms);
    std::cout << std::left << std::setw(35) << description << std::right << std::setw(15) << count << " calls in "
              << runtime_ms << "ms " << std::right << std::setw(25) << format_cps(calls_per_sec) << std::right
              << std::setw(25) << format_spc(sec_per_call) << std::endl;
  }
};

} // namespace benchmarks