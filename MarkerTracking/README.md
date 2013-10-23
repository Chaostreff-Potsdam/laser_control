TopTracking
===========

Software component responsible for tracking players from above and then streaming their positions to a game client via UDP.  
This manual is for **Windows**. Scroll down for instructions for Mac OS X and Ubuntu.

*Compiling this project is not easy, so please follow the instructions in this file very carefully*

0. Requirements
---------------

To build this project on Windows, you'll need:

* **Visual Studio 2010**
* **CMake >=2.8.4**, e.g. [http://www.cmake.org/files/v2.8/cmake-2.8.7-win32-x86.exe]()
* **Boost >=1.4.7**, e.g. [http://boostpro.com/download/boost_1_47_setup.exe]()
* **OpenCV >=2.3.0**, e.g. [http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.3.1/OpenCV-2.3.1-win-superpack.exe/download]()
* **SDL >=1.2.14**, e.g. [http://www.libsdl.org/release/SDL-devel-1.2.14-VC8.zip]()
* **Apple Bonjour SDK >=2.0.4**, e.g. [https://developer.apple.com/downloads/download.action?path=Developer_Tools/bonjour_sdk_for_windows_v2.0.4/bonjoursdksetup.exe]()


1. Environment Variables
------------------------

Set the following environment variables:

Example:

    SDLDIR=C:\SDL-1.2.14
    BONJOUR_SDK_HOME="c:\Program Files\Bonjour SDK"
    PATH=%PATH%:c:\boost_1_47_0\lib;c:\OpenCV2.3.1\build\x86\vc10\bin;c:\OpenCV2.3.1\build\common\tbb\ia32\vc10;C:\SDL-1.2.14\lib


2. Create Visual Studio Solution
--------------------------------

    cd build/
    cmake .. -G "Visual Studio 10" -DOpenCV_INCLUDE_DIR=(your opencv include/ dir) -DOpenCV_LIB_DIR=(your opencv lib/ dir) -DBOOST_ROOT=(boost root dir)

OpenCV include dir is typically something like `c:\OpenCV2.3.1\build\include` and OpenCV lib dir is `c:\OpenCV2.3.1\build\x86\vc10\lib`.

This creates a Visual Studio solution: `TopTracking.sln`.

3. Building
-----------

**Compile this solution in the `Release` configuration**. This is important because OpenCV is prone to crashing when you compile with debug symbols.


4. Running
-----------

Run `Release\TopTracking.exe`

5. Troubleshooting
------------------

Q: Either CMake or Visual Studio complains that it can't find one or more files
A: Make sure you actually pointed all CMake variables to the correct top level directories of the libraries: Sometimes it happens that e.g. `C:\boost_1_48_0` contains nothing but another folder called `boost`. In this case, the correct value for `BOOST_ROOT` would be `C:\boost_1_48_0\boost`!

Q: Visual Studio can't find OpenCV .hpp files
A: It's easy to get the OpenCV include dir wrong. If you have installed the Superpack, `OpenCV_INCLUDE_DIR` for example should be `C:\OpenCV2.3\build\include`. The `build` subdirectory in there is crucial.


A1. Building on Mac OS X
------------------------

Install Xcode to get `make` and `g++`.  
Use [Homebrew](http://mxcl.github.com/homebrew/) to get the required libraries:

    brew install cmake boost opencv sdl

Build with:

    cd build/
    cmake ..
    make

A2. Building on Ubuntu
----------------------

Install the required packages:

    sudo apt-get install build-essential cmake libboost-dev libcv-dev libsdl-dev libavahi-compat-libdnssd1

Build with:

    cd build/
    cmake ..
    make
