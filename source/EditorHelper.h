//
// Created by Crs_1 on 2023/5/26.
//

#ifndef AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H
#define AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H

#include <string>

class AudioPluginAudioProcessorEditor;

namespace OpenVpi {

    class EditorHelper {
        friend class ::AudioPluginAudioProcessorEditor;
        void setEditor(AudioPluginAudioProcessorEditor *editor);
        AudioPluginAudioProcessorEditor *m_editor = nullptr;
        std::string lazyStatus = "Not Connected";
        std::string lazyError;
    public:
        void setStatus(const std::string &status);
        void setError(const std::string &error);
        void setDirty();
        std::string status();
        std::string error();
    };

} // OpenVpi

#endif //AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H
