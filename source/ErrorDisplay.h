#pragma once
#ifndef DIFFSCOPE_VSTI_ERRORDISPLAY_H
#define DIFFSCOPE_VSTI_ERRORDISPLAY_H

#include "vstgui/plugin-bindings/vst3editor.h"

namespace OpenVpi {

    class ErrorDisplay {
    public:

        enum ErrorType {
            errLoadConfig,
            errLoadLibrary,
            errSingleton,
            errPlayback,
            errSetState,
            errGetState
        };

        void setView(VSTGUI::VST3Editor* view);

        bool hasView();

        bool showError(ErrorType errorType);

    protected:
        VSTGUI::VST3Editor* view;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_ERRORDISPLAY_H
