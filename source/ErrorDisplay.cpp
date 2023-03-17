#include "ErrorDisplay.h"
#include <string>

using namespace std;

namespace OpenVpi {
    MyTextController* ErrorDisplay::myTextController = nullptr;
    void ErrorDisplay::showError(const char *errorMessage) {
        if(ErrorDisplay::myTextController) {
            ErrorDisplay::myTextController->setText(errorMessage);
        }
    }
} // OpenVpi