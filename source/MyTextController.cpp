//
// Created by Crs_1 on 2023/3/17.
//

#include "MyTextController.h"
#include "ErrorDisplay.h"

using namespace VSTGUI;
using namespace std;

namespace OpenVpi {

    CView *MyTextController::verifyView(CView *view, const UIAttributes &attributes, const IUIDescription *description) {
        auto* textLabel = dynamic_cast<CTextLabel*>(view);
        this->textLabel = textLabel;
        setText(ErrorDisplay::getInstance()->stagedText.c_str());
        return textLabel;
    }

    void MyTextController::setText(const char* str) {
        if(textLabel){
            textLabel->setText(str);
        }
    }
    void MyTextController::valueChanged(VSTGUI::CControl *pControl) {}
} // OpenVpi