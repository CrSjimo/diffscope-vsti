#pragma once

#ifndef DIFFSCOPE_VSTI_API_H
#define DIFFSCOPE_VSTI_API_H

#include <cstdint>
#include <map>
#include <string>

#define API_DEF(name, type) using name = type;
#define API_CALL(name) (name)Api::getInstance()->getHandle(#name)
#define API_SET(name, handler) Api::getInstance()->adddHandler(#name, handler)

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

    API_DEF(SingletonChecker, bool (*)())
    API_DEF(Initializer, Result (*)())
    API_DEF(Terminator, Result (*)())
    API_DEF(WindowOpener, Result (*)())
    API_DEF(WindowCloser, void (*)())
    API_DEF(PlaybackProcessor, Result (*)(const PlaybackParameters *playbackParameters, int32_t numOutputs, float*** outputs)) // outputs[busId][channelId][sampleId]
    API_DEF(StateChangedCallback, Result (*)(uint64_t size, const uint8_t* data))
    API_DEF(StateWillSaveCallback, Result (*)(uint64_t& size, uint8_t*& data))
    API_DEF(StateSavedAsyncCallBack, void (*)(uint8_t* dataToFree))

    class Api {
    public:

        static Api* getInstance();
        static void destroyInstance();

        void addHandle(std::string name, void* handle);

        void* getHandle(std::string name);

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
