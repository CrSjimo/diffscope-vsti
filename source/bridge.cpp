#include"bridge.h"
#include<fstream>
#include<stdlib.h>

using namespace std;

namespace OpenVpi {
    void bridge(ProcessContext *processContext, int32 numChannels, Sample32 **input, Sample32 **output, int32 numSamples) {
        ofstream outFile;
        outFile.open("C:\\Users\\Crs_1\\1.test", ios::out);
        if (!outFile.is_open())
            exit(-1);
        outFile << processContext->projectTimeSamples << "," 
                << processContext->projectTimeMusic << ","
                << processContext->barPositionMusic << "," 
                << numChannels << ","
                << processContext->sampleRate << "," 
                << numSamples << ","
                << processContext->samplesToNextClock << ","
                << processContext->tempo << endl
                << "playing" << (processContext->state & ProcessContext::kPlaying) << ","
                << "cycle" << (processContext->state & ProcessContext::kCycleActive) << ","
                << endl;
        ::printf("test\n");
        outFile.close();
        for (size_t i = 0; i < numSamples; i++) {
            output[0][i] = input[0][i] * 0.1;
            output[1][i] = input[1][i];
        }

    }
}
