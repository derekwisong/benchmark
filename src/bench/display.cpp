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

  for (auto iter = scales.rbegin(); iter != scales.rend(); ++iter) { // NOLINT (modernize-loop-convert)
    const auto scale = iter->first;
    const auto& name = iter->second;
    if (cps >= scale) {
      return std::to_string(cps / scale) + " calls/" + name;
    }
  }

  return std::to_string(cps) + " calls/sec";
}

std::string format_spc(double spc) {
  static const std::map<double, std::pair<double, std::string>> scales = {
      {1e-6, {1e-9, "ns"}},
      {1e-3, {1e-6, "us"}},
      {1e-0, {1e-3, "ms"}},
  };

  for (const auto& iter : scales) {
    const auto level = iter.first;
    const auto& [scale, name] = iter.second;
    if (spc < level) {
      return std::to_string(spc / scale) + " " + name + "/call";
    }
  }

  return std::to_string(spc) + " sec/call";
}

void print_run_results(const std::string& description, const std::chrono::nanoseconds runtime,
                       const bench::timing::TimedRunResults& results) {
  static const int WIDTH_DESC_COL = 50;
  static const int WIDTH_COUNT_COL = 15;
  static const int WIDTH_CPS_COL = 25;
  static const int WIDTH_SPC_COL = 25;
  const auto runtime_ms = std::chrono::duration_cast<std::chrono::milliseconds>(runtime);
  std::cout << std::left << std::setw(WIDTH_DESC_COL) << description;
  std::cout << std::right << std::setw(WIDTH_COUNT_COL) << results.scaled_iterations() << " calls in "
            << runtime_ms.count() << "ms ";
  std::cout << std::right << std::setw(WIDTH_CPS_COL) << format_cps(results.cps());
  std::cout << std::right << std::setw(WIDTH_SPC_COL) << format_spc(results.spc());
  std::cout << std::endl;
}

// void print_run_results(const std::string& description, const std::chrono::nanoseconds runtime,
//                        const bench::timing::TimedRunStats& stats) {
//   static const int WIDTH_DESC = 50;
//   constexpr int WIDTH = 15;
//   auto print = [&](int width, const auto& val) { std::cout << std::right << std::setw(width) << val; };
//   std::cout << std::left << std::setw(WIDTH_DESC) << description;
//   print(WIDTH, stats.iterations);
//   print(WIDTH, stats.scaled_iterations());
//   print(WIDTH, stats.min());
//   print(WIDTH, stats.max());
//   print(WIDTH, stats.median());
//   print(WIDTH, stats.avg());
//   print(WIDTH, stats.stdev());
//   std::cout << std::endl;
// }

} // namespace bench::display