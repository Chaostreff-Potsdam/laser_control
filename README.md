# laser_control

This component provides a simple wrapper to the [Etherdream DAC][ed].

## 0. Requirements

### A compiler that understands C++11

For *nix OSs, this will be gcc >= 4.7.

Visual Studio supports C++11 since its 2012 incarnation.

### SWIG

If you want to wrap this wrapper for other programming languages just like C#, you will need to install [SWIG][swig].

## 1. Building

### *nixes

This should be pretty straightforward:

    cd laser_control
    mkdir build
    cd build
    cmake ..
    make

### Windows

Use cmake to generate a VS project. Compile it.


[swig]: http://swig.org/
[ed]: http://www.ether-dream.com/
