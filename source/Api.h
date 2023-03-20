#pragma once

#ifndef DIFFSCOPE_VSTI_API_H
#define DIFFSCOPE_VSTI_API_H

#include <cstdint>
#include <map>
#include <string>

#define OV_API_DEF(name, type) using name = type;
#define OV_API_CALL(name) (name)Api::getInstance()->getHandle(#name)
#define OV_API_SET(name, handle) Api::getInstance()->addHandle(#name, handle)

namespace OpenVpi {

    enum Result {
        Ok,
        Failed,
    };

    struct PlaybackParameters {

        double sampleRate;
        double tempo;

        union {
            int64_t projectTimeSamples;
            int64_t systemTimeMs;
        };
        double projectTimeMusic;
        double barPositionMusic;

        int32_t numSamples;

    };

    OV_API_DEF(SingletonChecker, bool (*)())
    OV_API_DEF(Initializer, Result (*)())
    OV_API_DEF(Terminator, Result (*)())
    OV_API_DEF(WindowOpener, void (*)())
    OV_API_DEF(WindowCloser, void (*)())
    OV_API_DEF(PlaybackProcessor, Result (*)(const PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const * outputs)) // outputs[busId][sampleId]
    OV_API_DEF(StateChangedCallback, Result (*)(uint64_t size, const uint8_t* data))
    OV_API_DEF(StateWillSaveCallback, Result (*)(uint64_t & size, uint8_t * & data))
    OV_API_DEF(StateSavedAsyncCallback, void (*)(uint8_t * dataToFree))

    class Api {
    public:

        static Api* getInstance();
        static void destroyInstance();

        void addHandle(const std::string& name, void* handle);

        void* getHandle(const std::string& name);

        void setInitializationState(bool state);

        bool getInitializationState() const;

    protected:
        static Api* instance;
        Api();
        ~Api();
        std::map<std::string, void*>* apiHandlesDict;
        bool initializationState;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_API_H
