#pragma once

#ifndef DIFFSCOPE_VSTI_BRIDGE_H
#define DIFFSCOPE_VSTI_BRIDGE_H

#include <cstdint>

#include <juce_audio_processors/juce_audio_processors.h>

#include <IVstBridge.h>

#include "EditorHelper.h"
#include "Api.h"
#include "libraryloader.h"

namespace OpenVpi {

    class Bridge {

        friend class ::AudioPluginAudioProcessorEditor;

        EditorHelper *m_editorHelper;
        std::unique_ptr<Api> m_api;
        std::unique_ptr<LibraryLoader> m_libLoader;
        IVstBridge *m_handle = nullptr;

        float ***myOutputs = nullptr;

    public:

        explicit Bridge(EditorHelper *editorHelper);

        bool processPlayback(juce::AudioBuffer<float> &buffer, int64_t timeInSamples, bool isRealtime, bool isPlaying);

        void loadData(int size, const char *data);

        bool saveData(int &size, const char *&data);

        void freeDataBuffer(const char *data);

        void initialize();

        void terminate();

        void showEditorWindow();

        void hideEditorWindow();

        bool setupProcess(int32_t totalNumOutputChannels, double sampleRate, int32_t maxBlockSize);

        void finalizeProcess();

    };
}

#endif