del /F /Q Z:\lib_msvc11\LaserControl\include\*
del /F /Q Z:\lib_msvc11\LaserControl\bin\*
del /F /Q Z:\lib_msvc11\LaserControl\lib\*

copy src\*.h Z:\lib_msvc11\LaserControl\include\
copy src\laser_calibration\*.h Z:\lib_msvc11\LaserControl\include\
copy src\etherdream\common\*.h Z:\lib_msvc11\LaserControl\include\
copy src\etherdream\driver-win\*.h Z:\lib_msvc11\LaserControl\include\

copy build\Debug\laserwrapper.dll Z:\lib_msvc11\LaserControl\bin\
copy build\lib\Debug\laserwrapper.lib Z:\lib_msvc11\LaserControl\lib\
