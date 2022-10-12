#pragma once

#include "bench/timing.h"

#include <chrono>
#include <string>

namespace bench::display {

std::string format_cps(double cps);
std::string format_spc(double spc);
void print_run_results(const std::string& description, std::chrono::nanoseconds runtime, const bench::timing::TimedRunResults& results);
void print_run_results(const std::string& description, std::chrono::nanoseconds runtime, const bench::timing::TimedRunStats& stats);

} // namespace bench::display