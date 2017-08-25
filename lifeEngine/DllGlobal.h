#ifndef DLL_GLOBAL_H
#define DLL_GLOBAL_H

#ifdef COMPILING_LIBRARY
#define DLL_API __declspec( dllexport )
#else
#define DLL_API __declspec( dllimport )
#endif

#endif // DLL_GLOBAL_H
