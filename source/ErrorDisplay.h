#pragma once
#ifndef DIFFSCOPE_VSTI_ERRORDISPLAY_H
#define DIFFSCOPE_VSTI_ERRORDISPLAY_H

#include "vstgui/plugin-bindings/vst3editor.h"
#include "MyTextController.h"
#include <string>

#define ERR_STR(error_code, error_msg) ("[Error " #error_code "] " error_msg)

#define ERR_LOAD_CONFIG ERR_STR(0, "Cannot load configuration.")
#define ERR_LOAD_LIBRARY ERR_STR(1, "Cannot load library. ")
#define ERR_INITIALIZATION ERR_STR(2, "Initialization failed. See logs for details.")
#define ERR_SINGLETON ERR_STR(3, "Multiple DiffScope Editor/VSTi cannot be run simultaneously.")

#define ERR_PLAYBACK ERR_STR(16, "Playback failed.")
#define ERR_GET_STATE ERR_STR(17, "Cannot save state.")
#define ERR_SET_STATE ERR_STR(18, "Cannot load state.")
#define ERR_SHOW_EDITOR ERR_STR(19, "Cannot show editor window.")
#define ERR_HIDE_EDITOR ERR_STR(20, "Cannot hide editor window.")

namespace OpenVpi {

    class ErrorDisplay {
    public:
        static ErrorDisplay* getInstance();
        static void destroyInstance();
        void setTextController(MyTextController* textController);
        void removeTextController();
        void showError(const char* errorMessage);
        friend class MyTextController;
    private:
        ErrorDisplay();
        static ErrorDisplay* instance;
        MyTextController* myTextController;
        std::string stagedText;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_ERRORDISPLAY_H
