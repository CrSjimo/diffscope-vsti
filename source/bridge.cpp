#include "bridge.h"
#include "Api.h"
#include "libraryloader.h"
#include <fstream>

using namespace std;

namespace OpenVpi {
    tresult processPlayback(const ProcessContext *processContext, int32 numOutputs, AudioBusBuffers *outputs, int32 numSamples) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto playbackProcessor = OV_API_CALL(PlaybackProcessor);
        if(!playbackProcessor) {
            ErrorDisplay::getInstance()->showError(ERR_PLAYBACK);
            return kNoInterface;
        }
        bool isPlaying = (processContext->state & ProcessContext::kPlaying) != 0;
        PlaybackParameters playbackParameters = {
                .sampleRate = processContext->sampleRate,
                .tempo = processContext->tempo,
                .projectTimeSamples = processContext->projectTimeSamples,
                .projectTimeMusic = processContext->projectTimeMusic,
                .barPositionMusic = processContext->barPositionMusic,
                .numSamples = numSamples,
        };
        if(!isPlaying) playbackParameters.systemTimeMs = processContext->systemTime / 1000000;
        auto** myOutputs = new Sample32*[numOutputs];
        for(int i = 0; i < numOutputs; i++) myOutputs[i] = outputs[i].channelBuffers32[0];
        if(playbackProcessor(&playbackParameters, isPlaying, numOutputs, myOutputs) == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_PLAYBACK);
            return kInternalError;
        }
        for(int busId = 0; busId < numOutputs; busId++) {
            for(int channelId = 1; channelId < outputs[busId].numChannels; channelId++) {
                memcpy_s(outputs[busId].channelBuffers32[channelId], numSamples * sizeof(Sample32), outputs[busId].channelBuffers32[0], numSamples * sizeof(Sample32));
            }
        }
        delete[] myOutputs;
        ErrorDisplay::getInstance()->showError("");
        return kResultOk;
    }

    tresult putData(uint64_t size, const uint8_t* data) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto stateChangedCallback = OV_API_CALL(StateChangedCallback);
        if(!stateChangedCallback) {
            ErrorDisplay::getInstance()->showError(ERR_SET_STATE);
            return kNoInterface;
        }
        if(stateChangedCallback(size, data) == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_SET_STATE);
            return kInternalError;
        }
        ErrorDisplay::getInstance()->showError("");
        return kResultOk;
    }

    tresult saveData(uint64_t& size, uint8_t*& data) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto stateWillSaveCallback = OV_API_CALL(StateWillSaveCallback);
        if(!stateWillSaveCallback) {
            ErrorDisplay::getInstance()->showError(ERR_GET_STATE);
            return kNoInterface;
        }
        if(stateWillSaveCallback(size, data) == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_GET_STATE);
            return kInternalError;
        }
        ErrorDisplay::getInstance()->showError("");
        return kResultOk;
    }

    tresult freeData(uint8_t* data) {
        if(!Api::getInstance()->getInitializationState()) return kNotInitialized;
        auto stateSavedAsyncCallback = OV_API_CALL(StateSavedAsyncCallback);
        if(!stateSavedAsyncCallback) {
            ErrorDisplay::getInstance()->showError(ERR_SET_STATE);
            return kNoInterface;
        }
        stateSavedAsyncCallback(data);
        ErrorDisplay::getInstance()->showError("");
        return kResultOk;
    }

    void initialize(void (*setDirty)(bool)) {
        if(!LibraryLoader::getInstance()->loadConfig()) {
            ErrorDisplay::getInstance()->showError(ERR_LOAD_CONFIG);
            return;
        }
        if(!LibraryLoader::getInstance()->loadLibrary()) {
            auto errorMessage = ERR_LOAD_LIBRARY + LibraryLoader::getInstance()->getError();
            ErrorDisplay::getInstance()->showError(errorMessage.c_str());
            return;
        }
        auto singletonChecker = OV_API_CALL(SingletonChecker);
        if(!singletonChecker) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return;
        }
        if(!singletonChecker()) {
            ErrorDisplay::getInstance()->showError(ERR_SINGLETON);
            return;
        }
        auto initializer = OV_API_CALL(Initializer);
        if(!initializer) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return;
        }
        if(initializer() == Failed) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return;
        }
        auto dirtySetterBinder = OV_API_CALL(DirtySetterBinder);
        if(!dirtySetterBinder) {
            ErrorDisplay::getInstance()->showError(ERR_INITIALIZATION);
            return;
        }
        dirtySetterBinder(setDirty);
        Api::getInstance()->setInitializationState(true);
    }

    void terminate() {
        auto terminator = OV_API_CALL(Terminator);
        if(!terminator) return;
        if(terminator() == Failed) return;
    }

    void showEditorWindow() {
        if(!Api::getInstance()->getInitializationState()) return;
        auto windowOpener = OV_API_CALL(WindowOpener);
        if(!windowOpener) {
            ErrorDisplay::getInstance()->showError(ERR_SHOW_EDITOR);
            return;
        }
        windowOpener();
        ErrorDisplay::getInstance()->showError("");
    }

    void hideEditorWindow() {
        if(!Api::getInstance()->getInitializationState()) return;
        auto windowCloser = OV_API_CALL(WindowCloser);
        if(!windowCloser) {
            ErrorDisplay::getInstance()->showError(ERR_HIDE_EDITOR);
            return;
        }
        windowCloser();
        ErrorDisplay::getInstance()->showError("");
    }
}
