#pragma once
#include "pluginterfaces/vst/vsttypes.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"

using namespace Steinberg;
using namespace Vst;

namespace OpenVpi {

	void bridge(ProcessContext * processContext, int32 numChannels, Sample32 * *input, Sample32 * *output, int32 numSamples);

}