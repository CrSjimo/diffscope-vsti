#include "ErrorDisplay.h"
#include <string>

using namespace std;

namespace OpenVpi {

    ErrorDisplay* ErrorDisplay::instance = nullptr;

    void ErrorDisplay::showError(const char *errorMessage) {
        if(ErrorDisplay::myTextController) {
            ErrorDisplay::myTextController->setText(errorMessage);
        }
    }

    ErrorDisplay *ErrorDisplay::getInstance() {
        if(instance) {
            return instance;
        } else {
            return instance = new ErrorDisplay;
        }
    }

    ErrorDisplay::ErrorDisplay() {
        this->myTextController = nullptr;
    }

    void ErrorDisplay::setTextController(MyTextController *textController) {
        this->myTextController = textController;
    }
} // OpenVpi