#pragma once

#ifndef DIFFSCOPE_VSTI_BRIDGE_H
#define DIFFSCOPE_VSTI_BRIDGE_H

#include <cstdint>
#include <juce_audio_processors/juce_audio_processors.h>

namespace OpenVpi {
	bool processPlayback(juce::AudioBuffer<float> &buffer, int64_t timeInSamples, int32_t totalNumOutputChannels, bool isRealtime, bool isPlaying);
    bool loadData(uint64_t size, const uint8_t* data);
    bool saveData(uint64_t& size, const uint8_t*& data);
    void freeDataBuffer(const uint8_t* data);
    void initialize();
    void terminate();
    void showEditorWindow();
    void hideEditorWindow();
    bool setupProcess(int32_t totalNumOutputChannels, double sampleRate, int32_t maxBlockSize);
}

#endif