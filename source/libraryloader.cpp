#include "libraryloader.h"

#include <fstream>
#include <locale>
#include <streambuf>

#include <loadso/library.h>
#include <loadso/system.h>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <shlobj.h>
#endif

#include "Api.h"
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
#ifdef _WIN32

            PWSTR appDataPath; //%AppData%\DiffScope\vstconfig.txt
            if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath))) {
                PathString filePath = appDataPath;
                filePath += L"\\ChorusKit\\DiffScope\\vstconfig.txt";
                CoTaskMemFree(appDataPath);
                return filePath;
            }
            return L"";
#else
            return LOADSO_STR("~/.config/ChorusKit/DiffScope/vstconfig.txt");
#endif
        }
    };

    LibraryLoader::LibraryLoader(Api *api) : d(std::make_unique<LibraryLoaderPrivate>()), m_api(api) {
    }

    bool LibraryLoader::loadConfig() {
        auto path = d->configPath();

        std::fstream fs;

        // Read configuration
        fs.open(path, std::ios::in);
        if (fs.fail()) {
            return false;
        }

        string dir;
        fs >> dir;
        if(dir.empty()) return false;
        while (!dir.empty() && (dir.back() == '\n' || dir.back() == '\r')) {
            dir.pop_back();
        }
//        MessageBoxA(nullptr, dir.c_str(), "111", MB_OK);
        string fileName;
        fs >> fileName;
        if(fileName.empty()) return false;
        while (!fileName.empty() && (fileName.back() == '\n' || fileName.back() == '\r')) {
            fileName.pop_back();
        }
        //MessageBoxA(nullptr, (dir+fileName).c_str(), "111", MB_OK);
        // Load library
        dllPath = System::MultiToWide(dir + fileName);
        libraryDirPath = System::MultiToWide(dir);
        return true;
    }

    string LibraryLoader::getError() {
        return System::WideToMulti(d->lib.lastError());
    }

    bool LibraryLoader::loadLibrary() {
        auto prev = System::SetLibraryPath(libraryDirPath); //TODO
        if (!d->lib.open(dllPath)) {
            System::SetLibraryPath(prev);
            return false;
        }
        System::SetLibraryPath(prev);
        ADD_HANDLE(Initializer, d->lib)
        ADD_HANDLE(Terminator, d->lib)
        ADD_HANDLE(WindowOpener, d->lib)
        ADD_HANDLE(WindowCloser, d->lib)
        ADD_HANDLE(PlaybackProcessor, d->lib)
        ADD_HANDLE(StateChangedCallback, d->lib)
        ADD_HANDLE(StateWillSaveCallback, d->lib)
        ADD_HANDLE(StateSavedAsyncCallback, d->lib)
        ADD_HANDLE(CallbacksBinder, d->lib)
        ADD_HANDLE(ProcessInitializer, d->lib)
        ADD_HANDLE(ProcessFinalizer, d->lib)
        ADD_HANDLE(HandleCreator, d->lib)
        ADD_HANDLE(HandleDeleter, d->lib)
        return true;
    }

    bool LibraryLoader::isAlreadyLoaded() {
        return Library::isOpen(dllPath);
    }


    LibraryLoader::~LibraryLoader() = default;

}