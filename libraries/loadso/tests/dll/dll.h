#ifndef DLL_H
#define DLL_H

#if _WIN32
#    define DLL_EXPORT __declspec(dllexport)
#else
#    define DLL_EXPORT
#endif

extern "C" DLL_EXPORT int add(int x, int y);

#endif // DLL_H
