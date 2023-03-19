//
// Created by Crs_1 on 2023/3/16.
//

#include "MyPluginEditor.h"
#include "bridge.h"
#include <cstring>

using namespace VSTGUI;
using namespace Steinberg::Vst;
using namespace std;

namespace OpenVpi {
    MyPluginEditor::MyPluginEditor(Steinberg::Vst::EditController *controller): VSTGUI::VST3Editor(controller, "view", "myplugineditor.uidesc") {
        myTextController = new MyTextController();
        myButtonController = new MyButtonController();
    }

    MyPluginEditor::~MyPluginEditor() {
        ErrorDisplay::getInstance()->removeTextController();
    }

    VSTGUI::IController * MyPluginEditor::createSubController(VSTGUI::UTF8StringPtr name, const VSTGUI::IUIDescription *description) {
        if(strcmp(name, "MyTextController") == 0) {
            ErrorDisplay::getInstance()->setTextController(myTextController);
            return myTextController;
        } else if(strcmp(name, "MyButtonController") == 0) {
            return myButtonController;
        } else {
            return nullptr;
        }
    }

    bool MyPluginEditor::open(void *parent, const VSTGUI::PlatformType &type) {
        showEditorWindow();
        return VST3Editor::open(parent, type);
    }

    void MyPluginEditor::close() {
        hideEditorWindow();
        VST3Editor::close();
    }
} // OpenVpi