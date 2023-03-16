#include"bridge.h"

using namespace std;

namespace OpenVpi {
    ErrorDisplay* errorDisplay = new ErrorDisplay;
    tresult processPlayback(ProcessContext *processContext, int32 numOutputs, AudioBusBuffers *output, int32 numSamples) {
        // TODO: invoke editor synchronously to process data
        return kResultOk;
    }
    ErrorDisplay* getErrorDisplay() {
        return errorDisplay;
    }
}
