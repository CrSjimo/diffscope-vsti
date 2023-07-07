//
// Created by Crs_1 on 2023/5/26.
//

#include "EditorHelper.h"

#include "PluginEditor.h"

namespace OpenVpi {

    void EditorHelper::setEditor(AudioPluginAudioProcessorEditor *editor) {
        m_editor = editor;
        if(m_editor) {
            editor->setStatus(lazyStatus);
            editor->setError(lazyError);
        }
    }

    void EditorHelper::setStatus(const char *status) {
        if(m_editor) {
            m_editor->setStatus(status);
        }
        lazyStatus = status;
    }

    void EditorHelper::setError(const char *error) {
        if(m_editor) {
            m_editor->setError(error);
        }
        lazyError = error;
        std::cerr << error << std::endl;
    }

    std::string EditorHelper::status() {
        return lazyStatus;
    }

    std::string EditorHelper::error() {
        return lazyError;
    }

    void EditorHelper::setDirty() {
        m_processor->updateHostDisplay();
    }

    EditorHelper::EditorHelper(AudioPluginAudioProcessor *processor): m_processor(processor) {

    }
} // OpenVpi