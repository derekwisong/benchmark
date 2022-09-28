#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <tuple>

#include <sys/stat.h>

struct TwoDoubles {
  double x = 0;
  double y = 0;
};

template <size_t size> void malloc_free() {
  auto ptr = std::malloc(size);
  std::free(ptr);
}

template <typename Type> void new_delete() {
  auto obj = new Type();
  delete obj;
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

template <typename Ptr> void random_access(const Ptr* array, size_t size) {
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

template <typename Func> std::tuple<size_t, double, double> repeat_for_time(Func &&func, unsigned int milliseconds) {
  auto start = std::chrono::system_clock::now();
  auto duration = std::chrono::milliseconds(milliseconds);
  auto end = start + duration;
  size_t counter = 0;

  while (std::chrono::system_clock::now() < end) {
    func();
    ++counter;
  }

  auto seconds = static_cast<double>(milliseconds) / 1000;
  double calls_per_second = static_cast<double>(counter) / seconds;
  double seconds_per_call = static_cast<double>(seconds) / counter;
  return std::make_tuple(counter, calls_per_second, seconds_per_call);
}

template <typename Func> void benchmark(std::string description, unsigned int milliseconds, Func &&func) {
  auto [count, calls_per_sec, sec_per_call] = repeat_for_time(func, milliseconds);
  std::cout << std::left << std::setw(30) << description << std::right << std::setw(12) << count << " calls in "
            << milliseconds << "ms " << std::setw(15) << calls_per_sec << " calls/sec " << std::setw(15) << sec_per_call
            << " sec/call" << std::endl;
}

int main(int, char **argv) {
  std::srand(std::time(nullptr));
  static const unsigned int RUNTIME = 1000;
  static const size_t NUM_INTEGERS = 1000000;
  int* integers = new int[NUM_INTEGERS];

  benchmark("new/delete (2 doubles)", RUNTIME, new_delete<TwoDoubles>);
  benchmark("malloc/free (2 doubles)", RUNTIME, malloc_free<sizeof(TwoDoubles)>);
  benchmark("malloc/free (1 million ints)", RUNTIME, malloc_free<NUM_INTEGERS * sizeof(int)>);
  benchmark("square int", RUNTIME, square<20>);
  benchmark("random int", RUNTIME, random_int);
  benchmark("random double", RUNTIME, random_double);
  benchmark("open/close file (fstream)", RUNTIME, [f = argv[0]] { open_close_fstream(f); });
  benchmark("open/close file (fopen)", RUNTIME, [f = argv[0]] { open_close_fopen(f); });
  benchmark("stat file", RUNTIME, [f = argv[0]] { stat_file(f); });
  benchmark("fill array with values", RUNTIME, [&]{std::fill(integers, integers + NUM_INTEGERS, 100);});
  benchmark("array random access", RUNTIME, [&]{random_access(integers, NUM_INTEGERS);});

  delete[] integers;

}
