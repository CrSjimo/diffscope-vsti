#include "ErrorDisplay.h"
#include <string>

using namespace std;

namespace OpenVpi {

    ErrorDisplay* ErrorDisplay::instance = nullptr;

    void ErrorDisplay::showError(const char *errorMessage) {
        if(myTextController) {
            myTextController->setText(errorMessage);
        }
    }

    ErrorDisplay *ErrorDisplay::getInstance() {
        if(instance) {
            return instance;
        } else {
            return instance = new ErrorDisplay;
        }
    }

    void ErrorDisplay::destroyInstance() {
        if(instance) {
            delete instance;
        }
    }

    ErrorDisplay::ErrorDisplay() {
        this->myTextController = nullptr;
    }

    void ErrorDisplay::setTextController(MyTextController *textController) {
        this->myTextController = textController;
    }
} // OpenVpi