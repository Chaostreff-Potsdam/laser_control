#pragma once

#ifdef _WIN32
#define EXPORT_LASER_CONTROL  __declspec(dllexport)
#else
#define EXPORT_LASER_CONTROL
#endif