cmake_minimum_required(VERSION 3.10)
project(cs426_project3_code C)

set(CMAKE_C_STANDARD 99)

# Basic compilation flags (no AddressSanitizer)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fopenmp -fno-omit-frame-pointer -fno-optimize-sibling-calls")

add_executable(lbp_seq lbp_seq.c util.c util.h)
add_executable(lbp_omp lbp_omp.c util.c util.h)
add_executable(IO_test IO_test.c util.c util.h)

# Link OpenMP properly
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
