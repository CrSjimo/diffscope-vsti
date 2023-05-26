#ifndef LIBRARY_H
#define LIBRARY_H

#include "loadso_global.h"

namespace LoadSO {

    class LOADSO_EXPORT Library {
    public:
        Library();
        ~Library();

    public:
        /**
         * @brief Load a library with a path, evaluated relative to the executable path if the path is relative.
         *
         * @param path
         * @return DllHandle
         */
        bool open(const PathString &path);
        bool close();

        DllHandle handle() const;
        EntryHandle entry(const std::string &name);

        /**
         * @return Last error message if an operation fails
         */
        PathString lastError() const;

    protected:
        class Impl;
        Impl *_impl;
    };

}

#endif // LIBRARY_H
