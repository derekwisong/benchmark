#pragma once

#include <chrono>
#include <tuple>

namespace bench::timing {
namespace chrono = std::chrono;

using timed_run_t = std::tuple<double, double, double>;

template <typename Func> timed_run_t repeat_for_time(Func &&func, unsigned int milliseconds) {
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
} // namespace bench::timing