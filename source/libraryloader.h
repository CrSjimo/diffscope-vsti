#pragma once

# ifdef _WIN32
#include<windows.h>
typedef ::HINSTANCE LibraryHandle;
#else
#include<dlfch.h>
typedef void* LibraryHandle;
#endif

#include<string>

namespace OpenVpi {

	std::wstring getEditorLibraryPath();

	LibraryHandle loadLibrary(const std::wstring& libraryPath);

	void* invokeLibrary(const LibraryHandle library, const char* symbolName);

}
