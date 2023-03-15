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

    };

    using PlaybackProcessor = Result (*)(const PlaybackParameters *playbackParameters, int32_t numOutputs, float*** output); // outputs[busId][channelId][sampleId]
    using StateChangedCallback = Result (*)(uint64_t size, const uint8_t* data);
    using StateWillSaveCallback = Result (*)(uint64_t& size, uint8_t*& data);
    using StateSavedAsyncCallBack = void (*)(uint8_t* dataToFree);

}