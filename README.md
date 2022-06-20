# Instructions in a nutshell

(See below for extended instructions)

Installing and running xrec

```
git clone --branch dev_rmn20 https://github.com/ECCC-ASTD-MRD/xrec
cd xrec
git -c submodule."ci-env".update=none submodule update --init --recursive
./download-xrec-utilities.sh .
mkdir -p build
cd build
cmake ..
make -j
make work
cd ../work-[OS_NAME-COMPILER_NAME]
. ./setup-xrec.dot [language_of_your_choice: english or francais]
./xrec -imflds [rpn_std_file]
```

# Extended instructions:

## Requirements

To compile and run xrec, you will need:

- Fortran and C compilers,
- libXm (motif), libXt, expat libraries (with development package),
- basic Unix utilities such as cmake (version 3.10 minimum), bash, etc.

## Installing xrec

- clone or download the git tar file of xrec at GitLab:  https://github.com/ECCC-ASTD-MRD/xrec
  
```
git clone --branch dev_rmn20 https://github.com/ECCC-ASTD-MRD/xrec
cd xrec
git -c submodule."ci-env".update=none submodule update --init --recursive
```

- execute the script named download-xrec-utilities.sh:

  ```./download-xrec-utilities.sh  .```
  
  or download directly the file at the following address, and untar it:
  
  http://collaboration.cmc.ec.gc.ca/science/outgoing/goas/xrec-utilities.tar.gz
  
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
cmake ..
make -j
make work
```

You can add extra CMake arguments such as```-DCMAKE_VERBOSE_MAKEFILE=ON``` to your **cmake** command
line to generate verbose makefiles which will print the exact compiler command lines issued.

You can also add ```-j``` to **make** to launch multiple compile tasks in
parallel.

The default compiler suite is GNU.  If you want to compile with other compilers,
you may have to add ```-DCOMPILER_SUITE=<compiler suite name (gnu|intel|...)>``` to the CMake
command line.

This release has been tested with GNU and Intel compilers on Linux x86_64.
Other compilers have also been used in the past, but have not been tested
with the current release.  You will likely have to modify the *.cmake files
in the **cmake_rpn/ec_compiler_presets/default/** folder.

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
```work-Fedora-34-x86_64-gnu-11.3.1``` would be created in the main directory,
and the following files installed in it: 

- binary named xrec
- setup file named setup-xrec.dot
- XREC-config file. It displays a summary of the compiler and flags used

## Running xrec

xrec interface and error messages are available in English and French.
Choose your language of choice with the setup file.
This setup file also sets TMPDIR and ARMNLIB variables.

```
cd ../work-[OS_NAME-COMPILER_NAME]
. ./setup-xrec.dot [language_of_your_choice: english or francais]
./xrec -imflds [rpn_std_file]
```

User guide in pdf format describes in more details how to use XREC.
