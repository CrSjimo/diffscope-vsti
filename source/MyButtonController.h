#pragma once
#ifndef DIFFSCOPE_VSTI_MYBUTTONCONTROLLER_H
#define DIFFSCOPE_VSTI_MYBUTTONCONTROLLER_H

#include "vstgui/plugin-bindings/vst3editor.h"

namespace OpenVpi {

class MyButtonController: public VSTGUI::IController {
public:
    VSTGUI::CView * verifyView(VSTGUI::CView *view, const VSTGUI::UIAttributes &attributes, const VSTGUI::IUIDescription *description) override;
    void valueChanged (VSTGUI::CControl* pControl) override;
};

} // OpenVpi

#endif //DIFFSCOPE_VSTI_MYBUTTONCONTROLLER_H
