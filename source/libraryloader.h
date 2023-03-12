#pragma once

#include <memory>
#include <string>


namespace OpenVpi {

    class LibraryLoaderPrivate;
    class LibraryLoader {
    public:
        LibraryLoader();
        ~LibraryLoader();

    protected:
        std::unique_ptr<LibraryLoaderPrivate> d;
    };

}
