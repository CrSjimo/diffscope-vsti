#include "ErrorDisplay.h"
#include <string>

using namespace std;

namespace OpenVpi {
    void ErrorDisplay::setView(VSTGUI::VST3Editor *view) {
        this->view = view;
    }
    bool ErrorDisplay::hasView() {
        return this->view != nullptr;
    }
    bool ErrorDisplay::showError(ErrorType errorType) {
        if(!this->hasView()) return false;
        string viewName = "err";
        viewName += to_string(errorType);
        return this->view->exchangeView(viewName.c_str());
    }
} // OpenVpi