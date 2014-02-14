del /F /Q Z:\lib_msvc11\LaserControl\include\*
del /F /Q Z:\lib_msvc11\LaserControl\bin\*
del /F /Q Z:\lib_msvc11\LaserControl\lib\*

copy src\*.h Z:\lib_msvc11\LaserControl\include\
copy src\laser_calibration\*.h Z:\lib_msvc11\LaserControl\include\
copy src\etherdream\common\*.h Z:\lib_msvc11\LaserControl\include\
copy src\etherdream\driver-win\*.h Z:\lib_msvc11\LaserControl\include\

copy build\Debug\laserwrapper.dll Z:\lib_msvc11\LaserControl\bin\laserwrapperd.dll
copy build\Release\laserwrapper.dll Z:\lib_msvc11\LaserControl\bin\laserwrapper.dll

copy build\lib\Debug\laserwrapper.lib Z:\lib_msvc11\LaserControl\lib\laserwrapperd.lib
copy build\lib\Release\laserwrapper.lib Z:\lib_msvc11\LaserControl\lib\laserwrapper.lib
