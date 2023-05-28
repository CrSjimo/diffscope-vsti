#pragma once

#ifndef DIFFSCOPE_VSTI_API_H
#define DIFFSCOPE_VSTI_API_H

#include <cstdint>
#include <map>
#include <string>
#include "ParameterTypes.h"

#define OV_API_DEF(name, type) using name = type;
#define OV_API_CALL(name) (name) this->m_api->getHandle(#name)
#define OV_API_SET(name, handle) this->m_api->addHandle(#name, handle)
#define OV_API_CHECK(ret) if(!this->m_api->getInitializationState()) return ret

namespace OpenVpi {

    OV_API_DEF(Initializer, bool (*)(void *h))
    OV_API_DEF(Terminator, void (*)(void *h))
    OV_API_DEF(WindowOpener, void (*)(void *h))
    OV_API_DEF(WindowCloser, void (*)(void *h))
    OV_API_DEF(PlaybackProcessor, bool (*)(void *h, const PlaybackParameters &parameters, float *const *const * outputs))
    OV_API_DEF(StateChangedCallback, void (*)(void *h, uint64_t size, const uint8_t *data))
    OV_API_DEF(StateWillSaveCallback, bool (*)(void *h, uint64_t &size, const uint8_t *&data))
    OV_API_DEF(StateSavedAsyncCallback, void (*)(void *h, const uint8_t * dataToFree))
    OV_API_DEF(CallbacksBinder, void (*)(void *h, void *editorHelper, const Callbacks &callbacks))
    OV_API_DEF(ProcessInitializer, bool(*)(void *h, int32_t totalNumOutputChannels, int32_t maxBufferSize, double sampleRate))
    OV_API_DEF(ProcessFinalizer, void(*)(void *h))
    OV_API_DEF(HandleCreator, void *(*)())

    class Api {
    public:

        Api();
        ~Api();

        void addHandle(const std::string& name, void* handle);

        void* getHandle(const std::string& name);

        void setInitializationState(bool state);

        bool getInitializationState() const;

    protected:
        std::map<std::string, void*>* apiHandlesDict;
        bool initializationState;
    };

} // OpenVpi

#endif //DIFFSCOPE_VSTI_API_H
