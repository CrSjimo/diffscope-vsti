#pragma once

#ifndef DIFFSCOPE_VSTI_API_H
#define DIFFSCOPE_VSTI_API_H

#include <cstdint>
#include <map>
#include <string>

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

    using SingletonChecker = bool (*)();
    using Initializer = Result (*)();
    using Terminator = Result (*)();
    using WindowOpener = Result (*)();
    using WindowCloser = void (*)();
    using PlaybackProcessor = Result (*)(const PlaybackParameters *playbackParameters, int32_t numOutputs, float*** output); // outputs[busId][channelId][sampleId]
    using StateChangedCallback = Result (*)(uint64_t size, const uint8_t* data);
    using StateWillSaveCallback = Result (*)(uint64_t& size, uint8_t*& data);
    using StateSavedAsyncCallBack = void (*)(uint8_t* dataToFree);
    class Api {
    public:

        static Api* getInstance();
        template<typename T>
        void addHandle(std::string name, T* handle);

        template<typename T>
        T* getHandle(std::string name);

        void setInitializationState(bool state);

        bool getInitializationState();

    protected:
        static Api* instance;
        Api();
        ~Api();
        std::map<std::string, void*>* apiHandlesDict;
        bool initializationState;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_API_H
