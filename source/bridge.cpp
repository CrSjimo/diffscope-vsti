#include"bridge.h"

using namespace std;

namespace OpenVpi {
    ErrorDisplay* errorDisplay = new ErrorDisplay;
    tresult processPlayback(ProcessContext *processContext, int32 numOutputs, AudioBusBuffers *output, int32 numSamples) {
        // TODO: invoke editor synchronously to process data
        if(processContext->state & ProcessContext::kPlaying)
            errorDisplay->showError("Playing");
        else
            errorDisplay->showError("Not Playing");
        return kResultOk;
    }
}
