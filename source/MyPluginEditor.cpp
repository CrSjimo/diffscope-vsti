//
// Created by Crs_1 on 2023/3/16.
//

#include "MyPluginEditor.h"
#include "MyButtonController.h"
#include "MyTextController.h"
#include "bridge.h"
#include <cstring>

using namespace VSTGUI;
using namespace Steinberg::Vst;
using namespace std;

namespace OpenVpi {
    MyPluginEditor::MyPluginEditor(Steinberg::Vst::EditController *controller): VSTGUI::VST3Editor(controller, "view", "myplugineditor.uidesc") {

    }

    VSTGUI::IController * MyPluginEditor::createSubController(VSTGUI::UTF8StringPtr name, const VSTGUI::IUIDescription *description) {
        if(strcmp(name, "MyTextController") == 0) {
            auto* myTextController = new MyTextController();
            ErrorDisplay::getInstance()->setTextController(myTextController);
            return myTextController;
        } else if(strcmp(name, "MyButtonController") == 0) {
            return new MyButtonController();
        } else {
            return nullptr;
        }
    }

    bool MyPluginEditor::open(void *parent, const VSTGUI::PlatformType &type) {
        return VST3Editor::open(parent, type);
    }

    void MyPluginEditor::close() {
        VST3Editor::close();
    }
} // OpenVpi