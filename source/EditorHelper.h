//
// Created by Crs_1 on 2023/5/26.
//

#ifndef AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H
#define AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H

#include "PluginEditor.h"

namespace OpenVpi {

    class EditorHelper {
        friend class ::AudioPluginAudioProcessorEditor;
        static void setEditor(AudioPluginAudioProcessorEditor *editor);
    public:
        static void setStatus(const std::string &status);
        static void setError(const std::string &error);
        static void setDirty();
        static juce::String status();
        static juce::String error();
    };

} // OpenVpi

#endif //AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H
