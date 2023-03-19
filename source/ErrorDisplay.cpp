#include "ErrorDisplay.h"
#include <string>

using namespace std;

namespace OpenVpi {

    ErrorDisplay* ErrorDisplay::instance = nullptr;

    void ErrorDisplay::showError(const char *errorMessage) {
        if(myTextController) {
            myTextController->setText(errorMessage);
        }
        stagedText = errorMessage;
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
            instance = nullptr;
        }
    }

    ErrorDisplay::ErrorDisplay() {
        this->myTextController = nullptr;
    }

    void ErrorDisplay::setTextController(MyTextController *textController) {
        this->myTextController = textController;
    }

    void ErrorDisplay::removeTextController() {
        this->myTextController = nullptr;
    }
} // OpenVpi