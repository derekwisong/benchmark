#include "bench/fileio.h"

#include <cstdio>
#include <fstream>

#include <sys/stat.h>

namespace bench::fileio {

void open_close_fstream(const char *path) {
  std::fstream file;
  file.open(path, std::ios::in);
  file.close();
}

void open_close_fopen(const char *path) {
  FILE* file = nullptr;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  errno_t err = fopen_s(&file, path, "rb");

  if (err != 0) {
    // the file was not opened
    return;
  }
#else
  file = std::fopen(path, "rb");
  if (!file) {
    return;
  }
#endif

  fclose(file);
}

void stat_file(const char *path) {
  struct stat st;
  stat(path, &st);
}

} // namespace bench::fileio