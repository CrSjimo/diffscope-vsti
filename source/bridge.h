#pragma once

#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"

using namespace Steinberg;
using namespace Vst;

namespace OpenVpi {

	tresult bridge(ProcessContext * processContext, int32 numOutputs, AudioBusBuffers* output, int32 numSamples);

}