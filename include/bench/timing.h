#pragma once

#include <chrono>
#include <iostream>
#include <tuple>

namespace bench::timing {
namespace chrono = std::chrono;

using timed_run_t = std::tuple<double, double, double>;

template <typename Func> timed_run_t repeat_for_time(Func &&func, unsigned int milliseconds) {
  using chrono::duration_cast;
  using clock_t = chrono::steady_clock;

  auto start = clock_t::now();
  auto scheduled_duration = chrono::milliseconds(milliseconds);
  auto scheduled_end = start + scheduled_duration;

  size_t counter;
  clock_t::time_point now = start;

  for (counter = 0; (now = clock_t::now()) < scheduled_end; ++counter) {
    func();
  }

  auto actual_duration = now - start;
  auto overage_ratio = actual_duration / scheduled_duration;

  auto actual_ns = duration_cast<chrono::nanoseconds>(actual_duration).count();
  auto seconds = static_cast<double>(actual_ns) / 1e9;

  double calls_per_second = static_cast<double>(counter) / seconds;
  double seconds_per_call = static_cast<double>(seconds) / counter;
  double scaled_counter = static_cast<double>(counter) / overage_ratio;

  return std::make_tuple(scaled_counter, calls_per_second, seconds_per_call);
}
} // namespace bench::timing