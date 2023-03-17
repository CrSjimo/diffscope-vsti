#include"bridge.h"

using namespace std;

namespace OpenVpi {
    tresult processPlayback(ProcessContext *processContext, int32 numOutputs, AudioBusBuffers *output, int32 numSamples) {
        // TODO: invoke editor synchronously to process data
        if(processContext->state & ProcessContext::kPlaying)
            ErrorDisplay::getInstance()->showError("Playing");
        else
            ErrorDisplay::getInstance()->showError("Not Playing");
        return kResultOk;
    }
}
