#include "loadso/library.h"
#include "loadso/system.h"

#ifdef _WIN32
#    include <Windows.h>
// 12345
#    include <Shlwapi.h>
#    define OS_MAX_PATH MAX_PATH
#else
#    include <dlfcn.h>
#    include <limits.h>
#    include <string.h>
#    define OS_MAX_PATH PATH_MAX
#endif

#ifdef _WIN32

static std::wstring WinGetLastErrorString(DWORD *errNum = nullptr) {
    // Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return {}; // No error message has been recorded
    }

    if (errNum) {
        *errNum = errorMessageID;
    }

    LPWSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us
    // (because we don't yet know how long the message string will be).
    size_t size = ::FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::wstring message(messageBuffer, size);

    // Free the Win32's string's buffer.
    ::LocalFree(messageBuffer);

    return message;
}

#endif

namespace LoadSO {

    using DllHandlePrivate =
#ifdef _WIN32
        HMODULE
#else
        void *
#endif
        ;

    using EntryHandlePrivate =
#ifdef _WIN32
        FARPROC
#else
        void *
#endif
        ;

    class Library::Impl {
    public:
        DllHandlePrivate hDll;
        PathString errorMessage;

        Impl() {
            hDll = nullptr;
        }

        void getErrMsg() {
           
#ifdef _WIN32
                errorMessage = WinGetLastErrorString();
#else
                auto err = dlerror();
                if (err) {
                    errorMessage = err;
                }
#endif
                
        }
    };

    Library::Library() {
        _impl = new Impl();
    }

    Library::~Library() {
//        close(); //not to close after deleted
        delete _impl;
    }

    bool Library::open(const PathString &path) {
        PathString absPath;
        if (System::IsRelativePath(path)) {
            absPath = System::ApplicationDirectory() + PathSeparator + path;
        } else {
            absPath = path;
        }

        auto handle =
#ifdef _WIN32
            ::LoadLibraryW(absPath.data())
#else
            dlopen(absPath.data(), RTLD_NOW)
#endif
            ;
        if (!handle) {
            _impl->getErrMsg();
            return false;
        }

        _impl->hDll = handle;
        return true;
    }

    bool Library::isOpen(const PathString &path) {
        auto handle =
#ifdef _WIN32
            ::GetModuleHandleW(path.data())
#else
            dlopen(path.data(), RTLD_NOLOAD)
#endif
            ;
        return handle != NULL;
    }

    bool Library::close() {
        auto handle = _impl->hDll;
        if (!handle) {
            return true;
        }

        if (!
#ifdef _WIN32
            ::FreeLibrary(handle)
#else
            (dlclose(handle) == 0)
#endif
        ) {
            _impl->getErrMsg();
            return false;
        }
        return true;
    }

    DllHandle Library::handle() const {
        return _impl->hDll;
    }

    EntryHandle Library::entry(const std::string &name) {
        auto handle = _impl->hDll;
        if (!handle) {
            return nullptr;
        }

        auto addr = (EntryHandle)
#ifdef _WIN32
            ::GetProcAddress(handle, name.data())
#else
            dlsym(handle, name.data())
#endif
            ;
        if (!addr) {
            _impl->getErrMsg();
        }
        return addr;
    }

    PathString Library::lastError() const {
        return _impl->errorMessage;
    }
}