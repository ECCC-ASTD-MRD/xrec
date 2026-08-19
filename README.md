xrec: 2D visualisation program

# Instructions in a nutshell

# At CMC only (external users: see below)

## Build dependencies

- CMake 3.20+
- librmn

## Environment

Load the right environment, depending on the architecture you need.  This
will load the specified compiler and its parameters, and set the
`EC_CMAKE_MODULE_PATH` variable for the `cmake_rpn` modules.

- Example for ppp7/sc7 and graniterapids specific architecture:

```
. r.load.dot mrd/rpn/code-tools/latest/env/rhel-9-graniterapids-64@inteloneapi-2025.1.0
```

- Example for generic architecture on ppp7/sc7

```
. r.load.dot mrd/rpn/code-tools/latest/env/rhel-9-amd64-64@inteloneapi-2025.1.0
```

- Example for GNU on any architecture:

```
. r.load.dot mrd/rpn/code-tools/latest/env/gnu
```

You will also need a version of librmn: either load it from ssm
(`. r.load.dot rpn/libs/...`) or provide the path to cmake install directory
if you compiled it.

```
mkdir -p build
cd build
cmake ..
make -j
# to install and use xrec, use make work
make work
cd ../work-[OS_NAME-COMPILER_NAME]
./bin/xrec -imflds [rpn_std_file]
```

If you are reading large files, you may have to increase the stack size:  
```
ulimit -s unlimited
```

# Outside CMC (external users)

(See below for extended instructions)

## Installing and running xrec

- First, clone and install librmn: https://github.com/ECCC-ASTD-MRD/librmn/
  (use master branch for the stable release, or alpha branch if you want to
  read files in fst24 format)

```
# clone xrec repository:
git clone --recurse https://github.com/ECCC-ASTD-MRD/xrec
cd xrec
./download-xrec-utilities.sh .
mkdir -p build
cd build
cmake -Drmn_ROOT=[librmn_install_path] ..
make -j
make work
cd ../work-[OS_NAME-COMPILER_NAME]
. ./setup-xrec.dot [language_of_your_choice: english or francais]
./bin/xrec -imflds [rpn_std_file]
```

# Extended instructions:

## Requirements

To compile and run xrec, you will need:

- CMake 3.20+
- librmn: https://github.com/ECCC-ASTD-MRD/librmn/
- Fortran and C compilers,
- libXm (motif), libXt, expat libraries (with development package),
- basic Unix utilities such as cmake (version 3.10 minimum), bash, etc.

## Installing xrec

- First, clone and install librmn: https://github.com/ECCC-ASTD-MRD/librmn/
  (use master branch for the stable release, or alpha branch if you want to
  read files in fst24 format)
- clone or download xrec git repository:  https://github.com/ECCC-ASTD-MRD/xrec
  
```
git clone --recurse https://github.com/ECCC-ASTD-MRD/xrec
cd xrec
git submodule update --init --recursive
```

If you have already cloned without the abovementioned option, run
`git submodule update --init --recursive` in the xrec directory

- execute the script named download-xrec-utilities.sh:

  ```./download-xrec-utilities.sh  .```
  
  or download directly the file at the following address, and untar it:
  
  https://hpfx.collab.science.gc.ca/~sixf000/xrec/xrec-utilities.tar.gz
  
  It contains data files, and a manual in pdf format.

## GNU compilers

- xrec is configured to use GNU compilers
- make sure the compilers and libraries paths are set in the appropriate
  environment variables (PATH and LD_LIBRARY_PATH)
- changes to the C and Fortran flags can be done in **cmake_rpn/ec_compiler_presets/default/Linux-x86_64/gnu.cmake**

## Intel COMPILER
- make sure the compilers and libraries paths are set in the appropriate
  environment variables (PATH and LD_LIBRARY_PATH)
- you may have to add ```-DCOMPILER_SUITE=intel``` to the CMake command line
- changes to the C and Fortran flags can be done in the  **cmake_rpn/ec_compiler_presets/default/Linux-x86_64/intel.cmake**

## Compiling and installing xrec

```
mkdir -p build
cd build
cmake -Drmn_ROOT=[librmn_install_path] ..
make
make work
```

You can add extra CMake arguments such as ```-DCMAKE_VERBOSE_MAKEFILE=ON``` to your `cmake` command
line to generate verbose makefiles which will print the exact compiler command lines issued.

You can also add ```-j``` to `make` commands to launch multiple compile tasks in
parallel.

The default compiler suite is GNU.  If you want to compile with other compilers,
you may have to add ```-DCOMPILER_SUITE=<compiler suite name (gnu|intel|...)>``` to the CMake
command line.

This release has been tested with GNU and Intel compilers on Linux x86_64.
Other compilers have also been used in the past, but have not been tested
with the current release.  You will likely have to modify the `.cmake` files
in the `cmake_rpn/ec_compiler_presets/default/` folder.

If you get error messages, make sure that the ```PATH``` and
```LD_LIBRARY_PATH``` environment variables contain the appropriate paths.

If the compiler or compile options are not right:

- Remove the content of the build directory
- Make appropriate changes to the cmake files corresponding to the
  compilers you are using
- Re-launch the commands starting at cmake

The installation process will create a directory named after the operating system
on which the compilation was executed, and the compiler you used
(work-${OS_NAME}-${COMPILER_NAME}). For example
```work-Fedora-44-x86_64-gnu-16.1.1``` would be created in the main directory,
and the following files installed in it: 

- binary named xrec
- setup file named setup-xrec.dot
- xrec-config file. It displays a summary of the compiler and flags used

## Running xrec

xrec interface and error messages are available in English and French.
Set your language of choice with the setup file.
This setup file also sets TMPDIR and ARMNLIB variables.

```
cd ../work-[OS_NAME-COMPILER_NAME]
. ./setup-xrec.dot [language_of_your_choice: english or francais]
./bin/xrec -imflds [rpn_std_file]
```

If you are reading large files, you may have to increase the stack size:  
```
ulimit -s unlimited
```

User guide in pdf format in utilities directory describes in more details how to use xrec.
