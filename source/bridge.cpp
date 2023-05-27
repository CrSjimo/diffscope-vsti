#include "bridge.h"
#include "Api.h"
#include "libraryloader.h"
#include "EditorHelper.h"

using namespace std;

namespace OpenVpi {

    static float ***myOutputs = nullptr;

    bool processPlayback(juce::AudioBuffer<float> &buffer, int64_t timeInSamples, int32_t totalNumOutputChannels, bool isRealtime, bool isPlaying) {
        if(!myOutputs) return false;
        if(!Api::getInstance()->getInitializationState()) return false;
        auto playbackProcessor = OV_API_CALL(PlaybackProcessor);
        if(!playbackProcessor) {
            return false;
        }
        PlaybackParameters playbackParameters = {
            isRealtime,
            isPlaying,
            timeInSamples,
            buffer.getNumSamples(),
            totalNumOutputChannels / 2,
        };
        for(int i = 0; i < playbackParameters.numOutputs; i++) {
            myOutputs[i][0] = buffer.getWritePointer(i * 2);
            myOutputs[i][1] = buffer.getWritePointer(i * 2 + 1);
        }
        if(!playbackProcessor(playbackParameters, myOutputs)) {
            return false;
        }
        return true;
    }

    void loadData(uint64_t size, const uint8_t* data) {
        if(!Api::getInstance()->getInitializationState()) return;
        auto stateChangedCallback = OV_API_CALL(StateChangedCallback);
        if(!stateChangedCallback) {
            return;
        }
        stateChangedCallback(size, data);
    }

    bool saveData(uint64_t& size, const uint8_t*& data) {
        if(!Api::getInstance()->getInitializationState()) return false;
        auto stateWillSaveCallback = OV_API_CALL(StateWillSaveCallback);
        if(!stateWillSaveCallback) {
            return false;
        }
        if(!stateWillSaveCallback(size, data)) {
            return false;
        }
        return true;
    }

    void freeDataBuffer(const uint8_t* data) {
        if(!Api::getInstance()->getInitializationState()) return;
        auto stateSavedAsyncCallback = OV_API_CALL(StateSavedAsyncCallback);
        if(!stateSavedAsyncCallback) {
            return;
        }
        stateSavedAsyncCallback(data);
    }

    void initialize() {
        if(!LibraryLoader::getInstance()->loadConfig()) {
            EditorHelper::setError("Cannot load vst config file.");
            return;
        }
        if(LibraryLoader::getInstance()->isAlreadyLoaded()) {
            EditorHelper::setError("Another DiffScope VSTi is already running in the same host.");
            return;
        }
        if(!LibraryLoader::getInstance()->loadLibrary()) {
            auto errorMessage = "Cannot load vst bridge library: " + LibraryLoader::getInstance()->getError();
            EditorHelper::setError(errorMessage);
            return;
        }
        auto callbacksBinder = OV_API_CALL(CallbacksBinder);
        Callbacks callbacks = {
            .setDirty = [](){
                EditorHelper::setDirty();
            },
            .setError = [](const char *error) {
                EditorHelper::setError(error);
            },
            .setStatus = [](const char *status) {
                EditorHelper::setStatus(status);
            },
        };
        callbacksBinder(callbacks);
        auto initializer = OV_API_CALL(Initializer);
        Api::getInstance()->setInitializationState(initializer());
    }

    void terminate() {
        auto terminator = OV_API_CALL(Terminator);
        terminator();
        Api::getInstance()->setInitializationState(false);
    }

    void showEditorWindow() {
        if(!Api::getInstance()->getInitializationState()) return;
        auto windowOpener = OV_API_CALL(WindowOpener);
        windowOpener();
    }

    void hideEditorWindow() {
        if(!Api::getInstance()->getInitializationState()) return;
        auto windowCloser = OV_API_CALL(WindowCloser);
        windowCloser();
    }

    bool setupProcess(int32_t totalNumOutputChannels, double sampleRate, int32_t maxBlockSize) {
        auto oldMyOutput = myOutputs;
        bool success = false;
        if(!Api::getInstance()->getInitializationState()) return false;
        auto processInitializer = OV_API_CALL(ProcessInitializer);
        if(!processInitializer(totalNumOutputChannels, maxBlockSize, sampleRate)) {
            myOutputs = nullptr;
            goto finalize;
        }
        myOutputs = new float**[totalNumOutputChannels / 2];
        for(int i = 0; i < totalNumOutputChannels / 2; i++) {
            myOutputs[i] = new float*[2];
//            myOutputs[i][0] = new float[maxBlockSize];
//            myOutputs[i][1] = new float[maxBlockSize];
        }
        success = true;
        finalize:
        if(oldMyOutput != nullptr) {
            for(int i = 0; i < totalNumOutputChannels / 2; i++) {
                delete[] oldMyOutput[i];
            }
            delete[] oldMyOutput;
        }
        return success;
    }
}
