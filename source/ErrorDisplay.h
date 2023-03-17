#pragma once
#ifndef DIFFSCOPE_VSTI_ERRORDISPLAY_H
#define DIFFSCOPE_VSTI_ERRORDISPLAY_H

#include "vstgui/plugin-bindings/vst3editor.h"
#include "MyTextController.h"
#include <string>

#define ERR_STR(error_code, error_msg) ("[Error " #error_code "] " error_msg)

#define ERR_LOAD_CONFIG ERR_STR(0, "Cannot load configuration")
#define ERR_LOAD_LIBRARY ERR_STR(1, "Cannot load library")
#define ERR_INITIALIZATION ERR_STR(2, "DiffScope library initialization failed")
#define ERR_SINGLETON ERR_STR(3, "Multiple DiffScope Editor/VSTi cannot be run simultaneously")

#define ERR_PLAYBACK ERR_STR(16, "Playback failed")
#define ERR_SET_STATE ERR_STR("17", "Cannot load state")
#define ERR_GET_STATE ERR_STR("18", "Cannot save state")

namespace OpenVpi {

    class ErrorDisplay {
    public:
        static MyTextController* myTextController;
        void showError(const char* errorMessage);
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_ERRORDISPLAY_H
