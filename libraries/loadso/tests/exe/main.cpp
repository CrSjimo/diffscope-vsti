#include <iostream>

#include <loadso/library.h>
#include <loadso/system.h>

using namespace LoadSO;

int main(int argc, char *argv[]) {
    System::PrintLine(LOADSO_STR("[Test Path]"));
    System::PrintLine(LOADSO_STR("App Path       : ") + System::ApplicationPath());
    System::PrintLine(LOADSO_STR("App File Name  : ") + System::ApplicationFileName());
    System::PrintLine(LOADSO_STR("App Directory  : ") + System::ApplicationDirectory());
    System::PrintLine(LOADSO_STR("App Name       : ") + System::ApplicationName());

    // Load library
    System::PrintLine(LOADSO_STR("[Test Load Library]"));
    Library lib;

    if (!lib.open(
#ifdef _WIN32
            LOADSO_STR(DLL_NAME)
#else
            LOADSO_STR("../lib/" DLL_NAME)
#endif
                )) {
        System::ShowError(lib.lastError());
        return -1;
    }
    System::PrintLine(LOADSO_STR("OK"));

    // Get function
    System::PrintLine(LOADSO_STR("[Test Get Function]"));
    using AddFunc = int (*)(int, int);
    auto add_func = (AddFunc) lib.entry("add");
    if (!add_func) {
        System::ShowError(lib.lastError());
        return -1;
    }
    System::PrintLine(LOADSO_STR("OK"));

    // Call function
    std::cout << add_func(1, 3) << std::endl;

    return 0;
}