************************************************
****           ViennaCV Readme              ****
************************************************

Version: 0.0.1 Unpublished

# Table of Contents

1. Requirements
2. Quick Start
3. Installation
4. Project Layout
5. Authors and Contact
  A. License



## Requirements

ViennaCV is developed based on ViennaCL which requires the following:
 * A recent C++ compiler (e.g. gcc 4.x.x)
 * For accessing GPUs and multi-core CPUs: OpenCL (shared library and include files) or CUDA if only targeting NVIDIA GPUs
 * For building some of the tutorials: Armadillo, Eigen, MTL4, or uBLAS (shipped with the Boost libraries).



## Quick Start

If your are familiar with ViennaCL series of codes, just copy the folder `viennacl` & `viennacv` to your include path and then you can enjoy the speed up of various hardwares.

 

## Installation



1. include ViennaCL 

   Users need to prepare the [ViennaCL](https://github.com/viennacl/viennacl-dev) first to let the ViennaCV run. For those who are unfamiliar with ViennaCL series of codes, click the link to follow the it to install the library. In fact, ViennaCL is a header only library, so users only need to copy the ViennaCL-dev/viennacl folder to your system include path or project include path. In case users wanna try the examples of viennacl to get familiar with ViennaCL or not familiar with C++ include configuration on your system, users might need to follow the README of ViennaCL. 

2. include ViennaCV

   After that, users just need to also add viennacv to your include path.





## Project Plan Layout



---- ViennaCV-1.X.X
   |
   |-- auxiliary/ - (only in src-Edition) Auxiliary files (i.e. the OpenCL source code tree and the converter for the header files)
   |
   |-- build/ - Build directory for building the examples
   |
   |-- CL/ - The OpenCL headers
   |
   |-- cmake/ - Additional CMake configuration files
   |
   |-- doc/ - Documentation (LaTeX and doxygen)
   |
   |-- examples/ - Tutorial and benchmarking applications
        |
        |-- testdata/ - Test data for the tutorials and benchmarks
        |
        |-- benchmarks/ - A small benchmarking suite
        |
        |-- tutorial/ - Some tutorials explaining the usage of ViennaCL
        |
        |-- parameters/ - Parameter optimization environment
   |
   |-- external/ - External libraries
   |
   |-- libviennacl/ - Shared library for interfacing some BLAS functionality of ViennaCL from languages other than C++
   |
   |-- tests/ - Automated test suite using CTest
   |
   |-- viennacl/ - The viennacl library source code
   |
   |-- viennacv/ - The viennacv library source code



## Contact

For any technical questions related to ViennaCV, please use email Wenyin Wei: wenyin.wei.ww@gmail.com or just issue on [Github](https://github.com/WenyinWei/ViennaCV/issues).

ViennaCV was developed under the aegis of the 'Department of Engineering Physics' at the 'Tsinghua University'.

## License

ViennaCV is distributed under the MIT (X11) License. See file LICENSE.

