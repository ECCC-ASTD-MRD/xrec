add_definitions(-DAMD64 -Di386 -DLINUX_X86_64 -DLittle_Endian)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_Fortran_COMPILER "gfortran")

set(CMAKE_C_FLAGS "-O2 -g -w -D_REENTRANT" CACHE STRING "C compiler flags" FORCE)
set(CMAKE_Fortran_FLAGS "-O2 -fdump-core -fbacktrace -g -w -fcray-pointer -fconvert=big-endian -frecord-marker=4 -fno-second-underscore -ffree-line-length-none -fpic -fopenmp" CACHE STRING "Fortran compiler flags" FORCE)

set(CMAKE_EXE_LINKER_FLAGS "-s -fopenmp -fpic")

set(LAPACK_LIBRARIES "-llapack")
set(BLAS_LIBRARIES "-lblas")
