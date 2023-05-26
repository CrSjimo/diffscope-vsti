#ifndef LOADSO_GLOBAL_H
#define LOADSO_GLOBAL_H

#include <iostream>

#ifdef _WIN32
#    define _LOADSO_DECL_EXPORT __declspec(dllexport)
#    define _LOADSO_DECL_IMPORT __declspec(dllimport)
#else
#    define _LOADSO_DECL_EXPORT
#    define _LOADSO_DECL_IMPORT
#endif

#ifndef LOADSO_EXPORT
#    ifdef LOADSO_STATIC
#        define LOADSO_EXPORT
#    else
#        ifdef LOADSO_LIBRARY
#            define LOADSO_EXPORT _LOADSO_DECL_EXPORT
#        else
#            define LOADSO_EXPORT _LOADSO_DECL_IMPORT
#        endif
#    endif
#endif

namespace LoadSO {

#ifdef _WIN32
    using PathChar = wchar_t;
    using PathString = std::wstring;
    using DllHandle = void *;
    using EntryHandle = void *;

#    define _LOADSO_STR(s) L##s

#    define LOADSO_STR(s) _LOADSO_STR(s)
#    define LOADSO_STRCPY wcscpy

    static const PathChar PathSeparator = L'\\';
#else
    using PathChar = char;
    using PathString = std::string;
    using DllHandle = void *;
    using EntryHandle = void *;

#    define LOADSO_STR(s) s
#    define LOADSO_STRCPY strcpy

    static const PathChar PathSeparator = '/';
#endif

}

#endif // LOADSO_GLOBAL_H
