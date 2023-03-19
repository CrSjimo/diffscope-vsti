#pragma once
#ifndef DIFFSCOPE_VSTI_MYBUTTONCONTROLLER_H
#define DIFFSCOPE_VSTI_MYBUTTONCONTROLLER_H

#include "vstgui/plugin-bindings/vst3editor.h"

namespace OpenVpi {

class MyButtonController: public VSTGUI::IController {
public:
    VSTGUI::CView * verifyView(VSTGUI::CView *view, const VSTGUI::UIAttributes &attributes, const VSTGUI::IUIDescription *description) override;
    ~MyButtonController();
    void valueChanged (VSTGUI::CControl* pControl) override;
protected:
    VSTGUI::CMovieButton* button;
};

} // OpenVpi

#endif //DIFFSCOPE_VSTI_MYBUTTONCONTROLLER_H
