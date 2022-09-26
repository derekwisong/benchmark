#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <utility>

class TwoDoubles {
    double x = 0;
    double y = 0;
};

template <typename Type>
void malloc_free() {
  auto ptr = std::malloc(sizeof(Type));
  std::free(ptr);
}

template <typename Type> void new_delete() {
  auto obj = new Type();
  delete obj;
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

template <typename Func>
void benchmark(Func &&func, unsigned int milliseconds, std::string description) {
  auto [count, calls_per_sec] = repeat_for_time(func, milliseconds);
  std::cout << description << ": " << count << " calls in " << milliseconds << "ms (" << calls_per_sec << " calls/sec"
            << std::endl;
}

int main(int, char **) {
    static const unsigned int RUNTIME = 500;
    benchmark(malloc_free<TwoDoubles>, RUNTIME, "malloc/free (size of TwoDoubles)");
    benchmark(new_delete<TwoDoubles>, RUNTIME, "new/delete of TwoDoubles");
}
