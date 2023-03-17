//
// Created by Crs_1 on 2023/3/16.
//

#include "MyButtonController.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "bridge.h"

using namespace VSTGUI;

namespace OpenVpi {
    CView *MyButtonController::verifyView(CView *view, const UIAttributes &attributes, const IUIDescription *description) {
        auto* button = dynamic_cast<CMovieButton*>(view);
        if(button) {
            button->registerControlListener(this);
            return button;
        }
        return nullptr;
    }

    void MyButtonController::valueChanged(CControl *pControl) {
        // TODO: Show editor window
        errorDisplay->showError("Show editor window");
    }
} // OpenVpi