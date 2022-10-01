#include "bench/display.h"
#include "bench/timing.h"

#include <iomanip>
#include <iostream>
#include <map>

namespace bench::display {
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

void print_run_results(const std::string &description, int runtime_ms, const bench::timing::timed_run_t &results) {
  auto [count, calls_per_sec, sec_per_call] = results;
  std::cout << std::left << std::setw(35) << description << std::right << std::setw(15) << count << " calls in "
            << runtime_ms << "ms " << std::right << std::setw(25) << format_cps(calls_per_sec) << std::right
            << std::setw(25) << format_spc(sec_per_call) << std::endl;
}

} // namespace bench::display