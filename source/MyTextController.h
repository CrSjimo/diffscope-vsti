#pragma once

#ifndef DIFFSCOPE_VSTI_MYTEXTCONTROLLER_H
#define DIFFSCOPE_VSTI_MYTEXTCONTROLLER_H

#include "vstgui/plugin-bindings/vst3editor.h"
#include <string>

namespace OpenVpi {
    class MyTextController: public VSTGUI::IController {
    public:
        VSTGUI::CView * verifyView(VSTGUI::CView *view, const VSTGUI::UIAttributes &attributes, const VSTGUI::IUIDescription *description) override;
        void setText(const char* str);
        void valueChanged (VSTGUI::CControl* pControl) override;
    protected:
        VSTGUI::CTextLabel* textLabel;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_MYTEXTCONTROLLER_H
