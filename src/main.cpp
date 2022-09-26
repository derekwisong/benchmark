#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>

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
  (void) result;
}

void random_int() {
  auto val = std::rand();
  (void) val;
}

void random_double() {
  auto val = static_cast<double>(std::rand()) / RAND_MAX;
  (void) val;
}

void open_close_fstream(const char* path) {
  std::fstream file;
  file.open(path, std::ios::in);
  file.close();
}

void open_close_fopen(const char* path) {
  FILE* file = fopen(path, "rb");
  fclose(file);
}

void stat_file(const char* path) {
  struct stat st;
  stat(path, &st);
}

template <typename Func> std::pair<size_t, double> repeat_for_time(Func &&func, unsigned int milliseconds) {
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
  return std::make_pair(counter, calls_per_second);
}

template <typename Func> void benchmark(std::string description, unsigned int milliseconds, Func &&func) {
  auto [count, calls_per_sec] = repeat_for_time(func, milliseconds);
  std::cout << std::left << std::setw(20) << description << std::right << std::setw(12) << count << " calls in "
            << milliseconds << "ms " << std::setw(20) << calls_per_sec << " calls/sec" << std::endl;
}

int main(int, char ** argv) {
  static const unsigned int RUNTIME = 1000;
  benchmark("new/delete", RUNTIME, new_delete<TwoDoubles>);
  benchmark("malloc/free", RUNTIME, malloc_free<sizeof(TwoDoubles)>);
  benchmark("square", RUNTIME, square<20>);
  benchmark("random int", RUNTIME, random_int);
  benchmark("random double", RUNTIME, random_double);
  benchmark("open/close (fstream)", RUNTIME, [f = argv[0]]{open_close_fstream(f);});
  benchmark("open/close (fopen)", RUNTIME, [f = argv[0]]{open_close_fopen(f);});
  benchmark("stat", RUNTIME, [f = argv[0]]{stat_file(f);});

}
