#pragma once

#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "ErrorDisplay.h"

using namespace Steinberg;
using namespace Vst;

namespace OpenVpi {
	tresult processPlayback(const ProcessContext * processContext, int32 numOutputs, AudioBusBuffers* outputs, int32 numSamples);
    tresult putData(uint64_t size, const uint8_t* data);
    tresult saveData(uint64_t& size, uint8_t*& data);
    tresult freeData(uint8_t* data);
    void initialize();
    void terminate();
    void showEditorWindow();
    void hideEditorWindow();
}