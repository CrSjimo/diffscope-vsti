//------------------------------------------------------------------------
// Copyright(c) 2023 OpenVpi Team.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace MyCompanyName {
//------------------------------------------------------------------------
static const Steinberg::FUID kDiffscopeVstiPluginProcessorUID (0xA87D935E, 0xAD995113, 0x9D734650, 0x28C89294);
static const Steinberg::FUID kDiffscopeVstiPluginControllerUID (0x3962124C, 0xAA555E81, 0xB0917FFD, 0x86F74E88);

#define DiffscopeVstiPluginVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace MyCompanyName
