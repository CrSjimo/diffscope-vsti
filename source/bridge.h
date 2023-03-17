#pragma once

#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"
#include "ErrorDisplay.h"

using namespace Steinberg;
using namespace Vst;

namespace OpenVpi {
	tresult processPlayback(ProcessContext * processContext, int32 numOutputs, AudioBusBuffers* output, int32 numSamples);
}