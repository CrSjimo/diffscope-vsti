#pragma once
#include<cstdint>

namespace OpenVpi {

    enum Result {
        Ok,
        Failed,
    };

    struct PlaybackParameters {

        double sampleRate;
        double tempo;

        int64_t projectTimeSamples;
        double projectTimeMusic;
        double barPositionMusic;

        int32_t numChannels;
        int32_t numSamples;

    }

    using PlaybackProcessor = Result (*)(const PlaybackParameters *playbackParameters, float **outputs);
    using StateChangedCallback = Result (*)(const uint8_t* data, size_t size);

}