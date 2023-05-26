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
#    include <stdio.h>
#    define OS_MAX_PATH PATH_MAX
#endif

namespace LoadSO {
    static PathChar __app_path[OS_MAX_PATH + 1] = {0};
    static size_t __app_path_len = 0;
    static size_t __app_path_slash_idx = 0;
    static size_t __app_path_dot_idx = 0;

    static bool __get_app_path() {
#ifdef _WIN32
        if (!::GetModuleFileNameW(nullptr, __app_path, MAX_PATH)) {
            return false;
        }
#else
#    ifdef __APPLE__
// To do ...
#    else
        if (!realpath("/proc/self/exe", __app_path)) {
            return {};
        }
#    endif
#endif
        // Search last slash
        {
            auto p = __app_path;

            // Iterate forward to '\0'
            for (; *p; ++p) {
            }
            auto last = p;

            // Iterate backward to the last slash
            PathChar *dot_ptr = __app_path;
            for (; p >= __app_path; --p) {
                if (*p == PathSeparator) {
                    break;
                } else if (*p == LOADSO_STR('.')) {
                    dot_ptr = p;
                }
            }

            if (p < __app_path) {
                return false;
            }
            __app_path_len = last - __app_path;
            __app_path_slash_idx = p - __app_path;
            __app_path_dot_idx = (dot_ptr > p) ? (dot_ptr - __app_path) : __app_path_len;
        }
        return true;
    }

    PathString System::ApplicationFileName() {
        if (!(*__app_path) && !__get_app_path()) {
            return {};
        }
        return __app_path + __app_path_slash_idx + 1;
    }

    PathString System::ApplicationDirectory() {
        if (!(*__app_path) && !__get_app_path()) {
            return {};
        }
        return {__app_path, __app_path_slash_idx};
    }

    PathString System::ApplicationPath() {
        if (!(*__app_path) && !__get_app_path()) {
            return {};
        }
        return __app_path;
    }

    PathString System::ApplicationName() {
        if (!(*__app_path) && !__get_app_path()) {
            return {};
        }
        return {__app_path + __app_path_slash_idx + 1, __app_path_dot_idx - __app_path_slash_idx - 1};
    }

    PathString System::LibraryPath(EntryHandle &func) {
#ifdef _WIN32
        wchar_t buf[OS_MAX_PATH + 1] = {0};
        HMODULE hm = nullptr;
        if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                                (LPCWSTR) &func, &hm) ||
            !GetModuleFileNameW(hm, buf, sizeof(buf))) {
            return {};
        }
#else
        Dl_info dl_info;
        dladdr((void *) func, &dl_info);
        auto buf = dl_info.dli_fname;
#endif
        return buf;
    }

    PathString System::SetLibraryPath(const LoadSO::PathString &path) {
#if _WIN32
        std::wstring org;
        wchar_t buf[OS_MAX_PATH + 1];
        if (::GetDllDirectoryW(OS_MAX_PATH, buf)) {
            org = buf;
        }
        SetDllDirectoryW(path.data());
#else
        std::string org = getenv("LD_LIBRARY_PATH");
        putenv((char *) ("LD_LIBRARY_PATH=" + path).data());
#endif
        return org;
    }

    bool System::IsRelativePath(const PathString &path) {
#if _WIN32
        return ::PathIsRelativeW(path.data());
#else
        auto p = path.data();
        while (p && *p == ' ') {
            p++;
        }
        return !p || *p != '/';
#endif
    }

    PathString System::PathToNativeSeparator(const PathString &path) {
        auto res = path;
        for (auto &ch : res) {
#if _WIN32
            if (ch == L'/')
#else
            if (ch == '\\')
#endif
                ch = PathSeparator;
        }
        return res;
    }

    PathString System::MultiToWide(const std::string &bytes) {
#ifdef _WIN32
        int len = MultiByteToWideChar(CP_UTF8, 0, bytes.data(), (int) bytes.size(), nullptr, 0);
        auto buf = new wchar_t[len + 1];
        MultiByteToWideChar(CP_UTF8, 0, bytes.data(), (int) bytes.size(), buf, len);
        buf[len] = '\0';

        std::wstring res(buf);
        delete[] buf;
        return res;
#else
        return bytes;
#endif
    }

    std::string System::WideToMulti(const PathString &str) {
#ifdef _WIN32
        int len = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int) str.size(), nullptr, 0, nullptr, nullptr);
        auto buf = new char[len + 1];
        WideCharToMultiByte(CP_UTF8, 0, str.data(), (int) str.size(), buf, len, nullptr, nullptr);
        buf[len] = '\0';

        std::string res(buf);
        delete[] buf;
        return res;
#else
        return str;
#endif
    }

#ifdef _WIN32
#    include <fcntl.h>
#    include <io.h>

    struct LocaleGuard {
        LocaleGuard() {
            mode = _setmode(_fileno(stdout), _O_U16TEXT);
        }
        ~LocaleGuard() {
            _setmode(_fileno(stdout), mode);
        }
        int mode;
    };
#endif

    void System::PrintLine(const PathString &text) {
#ifdef _WIN32
        LocaleGuard guard;
        //std::wcout << text << std::endl;
#else
        std::cout << text << std::endl;
#endif
    }

    void System::ShowError(const PathString &text) {
#ifdef _WIN32
        auto AppName = ApplicationName();
        ::MessageBoxW(nullptr, text.data(), AppName.empty() ? L"Fatal Error" : AppName.data(),
                      MB_OK
#    ifdef CONFIG_WIN32_MSGBOX_TOPMOST
                          | MB_TOPMOST
#    endif
                          | MB_SETFOREGROUND | MB_ICONERROR);
#else
        fprintf(stderr, "%s\n", text.data());
#endif
    }
}