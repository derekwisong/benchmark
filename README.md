# Benchmark Library

## Compiling

The Benchmark Library project is built with CMake.

To add this to your own CMake project use `add_subdirectory(<bench subdirectory>)` to your own `CMakeLists.txt` and
link against the library `bench`.

### Prerequisites

- A C++ compiler capable of the C++20 standard
- CMake

### Build Instructions

```bash
$ mkdir build
$ cd build
$ cmake ../
$ cmake build
$ make
```

The following files will be produced:

- `benchmark`: An executable which will run a set of sample benchmarks.
- `libbench.a`: Static library (use with headers in the repo `includes/` directory).
