#include "bridge.h"
#include "Api.h"

using namespace std;

namespace OpenVpi {
    tresult processPlayback(const ProcessContext *processContext, int32 numOutputs, AudioBusBuffers *output, int32 numSamples) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto playbackProcessor = API_CALL(PlaybackProcessor);
        if(!playbackProcessor) {
            ErrorDisplay::getInstance()->showError(ERR_PLAYBACK);
            return kNoInterface;
        }
        PlaybackParameters playbackParameters = {
                .sampleRate = processContext->sampleRate,
                .tempo = processContext->tempo,
                .projectTimeSamples = processContext->projectTimeSamples,
                .projectTimeMusic = processContext->projectTimeMusic,
                .barPositionMusic = processContext->barPositionMusic,
                .numChannels = 2, // reserved for future
                .numSamples = numSamples,
        };
        auto*** myOutputs = new Sample32**[numOutputs];
        for(int i = 0; i < numOutputs; i++) myOutputs[i] = output[i].channelBuffers32;
        if(playbackProcessor(&playbackParameters, numOutputs, myOutputs) == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_PLAYBACK);
            return kInternalError;
        }
        return kResultOk;
    }

    tresult putData(uint64_t size, const uint8_t* data) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto stateChangedCallback = API_CALL(StateChangedCallback);
        if(!stateChangedCallback) {
            ErrorDisplay::getInstance()->showError(ERR_SET_STATE);
            return kNoInterface;
        }
        if(stateChangedCallback(size, data) == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_SET_STATE);
            return kInternalError;
        }
        return kResultOk;
    }

    tresult saveData(uint64_t& size, uint8_t*& data) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto stateWillSaveCallback = API_CALL(StateWillSaveCallback);
        if(!stateWillSaveCallback) {
            ErrorDisplay::getInstance()->showError(ERR_GET_STATE);
            return kNoInterface;
        }
        if(stateWillSaveCallback(size, data) == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_GET_STATE);
            return kInternalError;
        }
        return kResultOk;
    }

    tresult freeData(uint8_t* data) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto stateSavedAsyncCallback = API_CALL(StateSavedAsyncCallBack);
        if(!stateSavedAsyncCallback) {
            ErrorDisplay::getInstance()->showError(ERR_SET_STATE);
            return kNoInterface;
        }
        stateSavedAsyncCallback(data);
        return kResultOk;
    }

    tresult initialize() {
        // TODO: load dll
        ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
        auto singletonChecker = API_CALL(SingletonChecker);
        if(!singletonChecker) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return kNoInterface;
        }
        if(!singletonChecker()) {
            ErrorDisplay::getInstance()->showError(ERR_SINGLETON);
            return kInternalError;
        }
        auto initializer = API_CALL(Initializer);
        if(!initializer) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return kNoInterface;
        }
        if(initializer() == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return kInternalError;
        }
        Api::getInstance()->setInitializationState(true);
        return kResultOk;
    }

    tresult terminate() {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto terminator = API_CALL(Terminator);
        if(!terminator) return kNoInterface;
        if(terminator() == Failed) return kInternalError;
        return kResultOk;

    }
}
