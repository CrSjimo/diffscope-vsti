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

    struct PlaybackParameters {
        double sampleRate;
        int64_t projectTimeSamples;
        int32_t numSamples;
    };

    OV_API_DEF(SingletonChecker, bool (*)())
    OV_API_DEF(Initializer, bool (*)())
    OV_API_DEF(Terminator, bool (*)())
    OV_API_DEF(WindowOpener, void (*)())
    OV_API_DEF(WindowCloser, void (*)())
    OV_API_DEF(PlaybackProcessor, bool (*)(const PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const *const * outputs)) // outputs[busId][sampleId]
    OV_API_DEF(StateChangedCallback, bool (*)(uint64_t size, const uint8_t* data))
    OV_API_DEF(StateWillSaveCallback, bool (*)(uint64_t & size, uint8_t * & data))
    OV_API_DEF(StateSavedAsyncCallback, void (*)(uint8_t * dataToFree))
    OV_API_DEF(DirtySetterBinder, void (*)(void (*setDirty)(bool state)))

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
