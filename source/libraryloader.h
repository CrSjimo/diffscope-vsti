#pragma once

#include <memory>
#include <string>
#include <loadso/loadso_global.h>
#include "Api.h"


namespace OpenVpi {

    class LibraryLoaderPrivate;
    class LibraryLoader {
    public:
        explicit LibraryLoader(Api *api);
        ~LibraryLoader();
        bool loadConfig();
        bool isAlreadyLoaded();
        bool loadLibrary();
        std::string getError();
    protected:
        Api *m_api;
        std::unique_ptr<LibraryLoaderPrivate> d;
        LoadSO::PathString dllPath;
        LoadSO::PathString libraryDirPath;
        static LibraryLoader* instance;
    };

}
