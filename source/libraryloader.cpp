#include "libraryloader.h"

#include <fstream>
#include <locale>
#include <sstream>

#include <loadso/library.h>
#include <loadso/system.h>

#include "Api.h"
#include "ErrorDisplay.h"

namespace OpenVpi {

    class LibraryLoaderPrivate {
    public:
        LoadSO::Library lib;

        static LoadSO::PathString configPath() {
            return
#ifdef _WIN32
                LOADSO_STR("%LOCALAPPDATA%\\ChorusKit\\DiffScope\\vstconfig.txt")
#elif __linux__
                LOADSO_STR("~/.local/share/ChorusKit/DiffScope/vstconfig.txt")
#else
                LOADSO_STR("~/Library/Application Support/ChorusKit/DiffScope/vstconfig.txt")
#endif
                    ;
        }
    };

    LibraryLoader::LibraryLoader() : d(std::make_unique<LibraryLoaderPrivate>()) {
        auto path = d->configPath();
#ifdef _WIN32
        std::wfstream
#else
        std::fstream
#endif
            fs;

        // Read configuration
        fs.open(path, std::ios::in);
        if (fs.fail()) {
            std::cerr << "Open configuration failed!" << std::endl;
            return;
        }

        std::stringstream ss;
        ss << fs.rdbuf();

        std::string content;
        ss >> content;

        // Trim path
        while (content.size() && (content.back() == '\n' || content.back() == '\r')) {
            content.pop_back();
        }

        // Load library
        auto dllPath = LoadSO::System::MultiToWide(content);
        LoadSO::System::PrintLine(LOADSO_STR("Dll: ") + dllPath);

        if (!d->lib.open(dllPath)) {
            LoadSO::System::ShowError(d->lib.lastError());
            return;
        }
    }

    LibraryLoader::~LibraryLoader() {
    }

}