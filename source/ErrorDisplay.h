#pragma once
#ifndef DIFFSCOPE_VSTI_ERRORDISPLAY_H
#define DIFFSCOPE_VSTI_ERRORDISPLAY_H

#include "vstgui/plugin-bindings/vst3editor.h"
#include "MyTextController.h"
#include <string>

#define ERR_STR(error_code, error_msg) ("[Error " #error_code "] " error_msg)

#define ERR_LOAD_CONFIG ERR_STR(0, "Cannot load configuration.")
#define ERR_LOAD_LIBRARY ERR_STR(reason) (1, ("Cannot load library." reason))
#define ERR_INITIALIZATION ERR_STR(2, "Initialization failed. See logs for details.")
#define ERR_SINGLETON ERR_STR(3, "Multiple DiffScope Editor/VSTi cannot be run simultaneously.")

namespace OpenVpi {

    class ErrorDisplay {
    public:
        static ErrorDisplay* getInstance();
        void setTextController(MyTextController* textController);
        void showError(const char* errorMessage);
    private:
        ErrorDisplay();
        static ErrorDisplay* instance;
        MyTextController* myTextController;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_ERRORDISPLAY_H
