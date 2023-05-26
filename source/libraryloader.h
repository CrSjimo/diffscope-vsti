#pragma once

#include <memory>
#include <string>
#include <loadso/loadso_global.h>


namespace OpenVpi {

    class LibraryLoaderPrivate;
    class LibraryLoader {
    public:
        static LibraryLoader* getInstance();
        static void destroyInstance();
        bool loadConfig();
        bool loadLibrary();
        std::string getError();
    protected:
        LibraryLoader();
        ~LibraryLoader();
        std::unique_ptr<LibraryLoaderPrivate> d;
        LoadSO::PathString dllPath;
        LoadSO::PathString libraryDirPath;
        static LibraryLoader* instance;
    };

}
