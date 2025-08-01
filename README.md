# Blas testing
# BLAS Testing with Various Backends

This project demonstrates how to use BLAS libraries with different backends (MKL, OpenBLAS, etc.) in C++ and Fortran applications.

## Requirements

- pixi package manager
- CMake 3.10+
- C++ and Fortran compilers

## Structure

```
./
├── CMakeLists.txt        # Top-level CMake configuration
├── cmake/                # CMake modules
│   └── FindMKL.cmake     # MKL finder module
├── pixi.toml             # Pixi configuration
└── src/                  # Source code
    ├── cpp/              # C++ examples
    │   └── dgemm/        # DGEMM example
    └── fortran/          # Fortran examples
        └── dgemm/        # DGEMM example
```

## Setup

Create and activate the MKL environment:

```bash
pixi install
pixi shell mklenv
```

## Build

Build all examples:

```bash
pixi run configure
pixi run build
```

## Run

Run specific examples:

```bash
pixi run run-cpp-dgemm
pixi run run-fortran-dgemm
```

Or run all examples at once:

```bash
pixi run run-all
```

## Adding New Backends

To add a new backend (e.g., OpenBLAS):

1. Add a new feature in `pixi.toml`
2. Update CMake configuration if needed
3. Ensure proper linking in the build system

## Performance Comparison

You can compare the performance of different BLAS backends by running the examples with different environment configurations and comparing the GFLOPS output.
in this repo using pixi as the 