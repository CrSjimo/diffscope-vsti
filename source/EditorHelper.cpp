//
// Created by Crs_1 on 2023/5/26.
//

#include "EditorHelper.h"

namespace OpenVpi {

    static AudioPluginAudioProcessorEditor *m_editor = nullptr;
    static std::string lazyStatus = "Not Connected";
    static std::string lazyError;


    void EditorHelper::setEditor(AudioPluginAudioProcessorEditor *editor) {
        m_editor = editor;
        editor->setStatus(lazyStatus);
        editor->setError(lazyError);
    }

    void EditorHelper::setStatus(const std::string &status) {
        if(m_editor) {
            m_editor->setStatus(status);
        }
        lazyStatus = status;
    }

    void EditorHelper::setError(const std::string &error) {
        if(m_editor) {
            m_editor->setError(error);
        }
        lazyError = error;
#ifdef DEBUG
        std::cerr << error << std::endl;
#endif
    }

    juce::String EditorHelper::status() {
        return lazyStatus;
    }

    juce::String EditorHelper::error() {
        return lazyError;
    }

    void EditorHelper::setDirty() {
        m_editor->processor.updateHostDisplay();
    }
} // OpenVpi