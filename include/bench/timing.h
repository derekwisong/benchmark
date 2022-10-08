#pragma once

#include <chrono>
#include <iostream>
#include <tuple>

namespace bench::timing {
namespace chrono = std::chrono;


class TimedRunResults {
  public:
  const unsigned int duration_milliseconds;
  const double counter;
  const double calls_per_second;
  const double seconds_per_call;

  TimedRunResults(unsigned int duration_milliseconds, double counter, double calls_per_second, double seconds_per_call)
    : duration_milliseconds(duration_milliseconds), counter(counter), calls_per_second(calls_per_second), seconds_per_call(seconds_per_call)
    {}
};

template <typename Func> TimedRunResults repeat_for_time(Func&& func, unsigned int milliseconds) {
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
  static const double NANOS_PER_SECOND = 1e9;
  auto seconds = static_cast<double>(actual_ns) / NANOS_PER_SECOND;

  double calls_per_second = static_cast<double>(counter) / seconds;
  double seconds_per_call = static_cast<double>(seconds) / counter;
  double scaled_counter = static_cast<double>(counter) / overage_ratio;

  return {milliseconds, scaled_counter, calls_per_second, seconds_per_call};
}
} // namespace bench::timing