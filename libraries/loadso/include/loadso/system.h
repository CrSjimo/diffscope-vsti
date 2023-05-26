#ifndef SYSTEM_H
#define SYSTEM_H

#include "loadso_global.h"

namespace LoadSO {

    class LOADSO_EXPORT System {
    public:
        static PathString ApplicationFileName();
        static PathString ApplicationDirectory();
        static PathString ApplicationPath();
        static PathString ApplicationName();

        /**
         * @param func Any static or global function in shared library
         * @return Absolute path of the shared library.
         */
        static PathString LibraryPath(EntryHandle &func);

        /**
         * Call SetDllDirectory on Windows, change LD_LIBRARY_PATH env on Unix.
         *
         * @param path Dll directory.
         * @return The previous library path.
         */
        static PathString SetLibraryPath(const PathString &path);

    public:
        static bool IsRelativePath(const PathString &path);
        static PathString PathToNativeSeparator(const PathString &path);

        /**
         * @param bytes Multi bytes string
         * @return Wide char string on Windows, the original string on Unix.
         */
        static PathString MultiToWide(const std::string &bytes);

        /**
         * @param str Wide char string
         * @return Multi bytes string on Windows, the original string on Unix.
         */
        static std::string WideToMulti(const PathString &str);

    public:
        static void PrintLine(const PathString &text);
        static void ShowError(const PathString &text);
    };

}

#endif // SYSTEM_H
