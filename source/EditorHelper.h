//
// Created by Crs_1 on 2023/5/26.
//

#ifndef AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H
#define AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H

#include <string>

#include <IEditorCallbacks.h>

class AudioPluginAudioProcessorEditor;

namespace OpenVpi {

    class EditorHelper: public IEditorCallbacks {
        friend class ::AudioPluginAudioProcessorEditor;
        void setEditor(AudioPluginAudioProcessorEditor *editor);
        AudioPluginAudioProcessorEditor *m_editor = nullptr;
        std::string lazyStatus = "Not Connected";
        std::string lazyError;
    public:
        void setStatus(const char *status) override;
        void setError(const char *error) override;
        void setDirty() override;
        std::string status();
        std::string error();
    };

} // OpenVpi

#endif //AUDIO_PLUGIN_EXAMPLE_EDITORHELPER_H
