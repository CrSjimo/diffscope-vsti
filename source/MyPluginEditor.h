#pragma once

#ifndef DIFFSCOPE_VSTI_MYPLUGINEDITOR_H
#define DIFFSCOPE_VSTI_MYPLUGINEDITOR_H

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "MyButtonController.h"
#include "MyTextController.h"

namespace OpenVpi {

class MyPluginEditor: public VSTGUI::VST3Editor {
public:
    MyPluginEditor(Steinberg::Vst::EditController* controller);
    ~MyPluginEditor();
    bool open(void *parent, const VSTGUI::PlatformType &type) override;
    void close() override;
    VSTGUI::IController * createSubController(VSTGUI::UTF8StringPtr name, const VSTGUI::IUIDescription *description) override;
protected:
    MyButtonController* myButtonController;
    MyTextController* myTextController;
};

} // OpenVpi

#endif //DIFFSCOPE_VSTI_MYPLUGINEDITOR_H
