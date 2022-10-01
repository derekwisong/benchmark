#include "benchmarks.h"

#include <fstream>
#include <map>
#include <sys/stat.h>


namespace benchmarks {

void random_int() {
  auto val = std::rand();
  (void)val;
}

void random_double() {
  auto val = static_cast<double>(std::rand()) / RAND_MAX;
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

} // namespace benchmarks