#include "libraryloader.h"

#include <fstream>
#include <locale>
#include <streambuf>

#include <loadso/library.h>
#include <loadso/system.h>
#define NOMINMAX
#include <Windows.h>

#include "Api.h"
#include "ErrorDisplay.h"
#define ADD_HANDLE(name, lib)                                                                                          \
    {                                                                                                                  \
        auto handle = lib.entry(#name);                                                                                \
        if (!handle)                                                                                                   \
            return false;                                                                                              \
        OV_API_SET(name, handle);                                                                                      \
    }

using namespace std;
using namespace LoadSO;


namespace OpenVpi {

    class LibraryLoaderPrivate {
    public:
        Library lib;

        static PathString configPath() {
            return
#ifdef _WIN32
                LOADSO_STR("D:\\dstest\\vstconfig.txt") // TODO
#elif __linux__
                LOADSO_STR("~/.local/share/ChorusKit/DiffScope/vstconfig.txt")
#else
                LOADSO_STR("~/Library/Application Support/ChorusKit/DiffScope/vstconfig.txt")
#endif
                ;
        }
    };

    LibraryLoader::LibraryLoader() : d(std::make_unique<LibraryLoaderPrivate>()) {
    }

    LibraryLoader *LibraryLoader::instance = nullptr;

    LibraryLoader *LibraryLoader::getInstance() {
        if (!instance) {
            return instance = new LibraryLoader;
        } else {
            return instance;
        }
    }

    void LibraryLoader::destroyInstance() {
        delete instance;
        instance = nullptr;
    }

    bool LibraryLoader::loadConfig() {
        auto path = d->configPath();

        std::fstream fs;

        // Read configuration
        fs.open(path, std::ios::in | std::ios::binary);
        if (fs.fail()) {
            return false;
        }

        std::string content((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());
        fs.close();

        // Trim path
        while (content.size() && (content.back() == '\n' || content.back() == '\r')) {
            content.pop_back();
        }

        // Load library
        dllPath = System::MultiToWide(content);
        return true;
    }

    string LibraryLoader::getError() {
        return System::WideToMulti(d->lib.lastError());
    }

    bool LibraryLoader::loadLibrary() {
        // MessageBoxW(nullptr, dllPath.c_str(), L"DiffScope VSTi", MB_OK);
        if (!d->lib.open(dllPath)) {
            return false;
        }
        ADD_HANDLE(SingletonChecker, d->lib)
        ADD_HANDLE(Initializer, d->lib)
        ADD_HANDLE(Terminator, d->lib)
        ADD_HANDLE(WindowOpener, d->lib)
        ADD_HANDLE(WindowCloser, d->lib)
        ADD_HANDLE(PlaybackProcessor, d->lib)
        ADD_HANDLE(StateChangedCallback, d->lib)
        ADD_HANDLE(StateWillSaveCallback, d->lib)
        ADD_HANDLE(StateSavedAsyncCallback, d->lib)
        ADD_HANDLE(DirtySetterBinder, d->lib)
        return true;
    }



    LibraryLoader::~LibraryLoader() = default;

}