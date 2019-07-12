# laser_control

This component provides a simple wrapper to the [Etherdream DAC][ed].

It was originally written as graphical instruction output for the TurkDeck VR experience:

> Cheng, L. P., Roumen, T., Rantzsch, H., Köhler, S., Schmidt, P., Kovacs, R., Jasper, J., Kemper, J., & Baudisch, P. (2015, November). Turkdeck: Physical virtual reality based on people. In *Proceedings of the 28th Annual ACM Symposium on User Interface Software & Technology* (pp. 417-426). ACM.
> [doi:10.1145/2807442.2807463](https://doi.org/10.1145/2807442.2807463)



## 0. Requirements

### A compiler that understands C++11

For *nix OSs, this will be gcc >= 4.7.

Visual Studio supports C++11 since its 2012 incarnation.

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

To run it from VS, it has to find openCV DLLs:
Add to path: Z:/lib_msvc11/opencv/2.4.8/build/x64/vc11/bin;

### Unity (Mac and Linux)

    cp lib/laserwrappercs.so [path-to-Unity-project]/Assets/Plugin/
    cp lib/laserwrappercs.bundle [path-to-Unity-project]/Assets/Plugin/
    cp lib/LaserWrapperCS/ [path-to-Unity-project]/Assets/Plugin/


[swig]: http://swig.org/
[ed]: http://www.ether-dream.com/
