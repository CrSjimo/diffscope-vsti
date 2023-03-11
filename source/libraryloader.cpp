#include"libraryloader.h"
#include<fstream>
#include<codecvt>
#include<locale>

namespace OpenVpi {

	std::wstring getEditorLibraryPath() {
#ifdef _WIN32
		std::string configPath("%LOCALAPPDATA%\\DiffScope\\VST_LIB_PATH");
#elif __linux__
		std::string configPath("~/.local/share/DiffScope/VST_LIB_PATH");
#else
		std::string configPath("~/Library/Application Support/DiffScope/VST_LIB_PATH");
#endif
		std::wifstream configFile;
		configFile.open(configPath, std::ios_base::in);
		if (!configFile.is_open()) {
			return L"";
		}
		std::wstring result;
		configFile >> result;
		return result;
	}

	LibraryHandle loadLibrary(const std::wstring& libraryPath) {
#ifdef _WIN32
		auto splitPos = libraryPath.find_last_of('\\');
		if (splitPos == libraryPath.npos) {
			return nullptr;
		}
		::AddDllDirectory(libraryPath.substr(0, splitPos).c_str());
		return ::LoadLibraryW(libraryPath.c_str());
#else
		std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> converter(new std::string(std::setlocale(LC_ALL, "")));
		return ::dlopen(converter.to_bytes(libraryPath).c_str(), RTLD_LAZY);
#endif
	}

	void* invokeLibrary(const LibraryHandle library, const char* symbolName) {
#ifdef _WIN32
		return ::GetProcAddress(library, symbolName);
#else
		return ::dlsym(library, symbolName)
#endif
	}

}
