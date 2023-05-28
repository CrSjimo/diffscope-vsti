#include "bridge.h"
#include "Api.h"
#include "libraryloader.h"

using namespace std;

namespace OpenVpi {

    Bridge::Bridge(EditorHelper *editorHelper): m_editorHelper(editorHelper), m_api(new Api), m_libLoader(new LibraryLoader(m_api.get())) {}

    bool Bridge::processPlayback(juce::AudioBuffer<float> &buffer, int64_t timeInSamples, int32_t totalNumOutputChannels, bool isRealtime, bool isPlaying) {
        if(!myOutputs) return false;
        OV_API_CHECK(false);
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
        if(!playbackProcessor(m_handle, playbackParameters, myOutputs)) {
            return false;
        }
        return true;
    }

    void Bridge::loadData(uint64_t size, const uint8_t* data) {
        OV_API_CHECK(void());
        auto stateChangedCallback = OV_API_CALL(StateChangedCallback);
        if(!stateChangedCallback) {
            return;
        }
        stateChangedCallback(m_handle, size, data);
    }

    bool Bridge::saveData(uint64_t& size, const uint8_t*& data) {
        OV_API_CHECK(false);
        auto stateWillSaveCallback = OV_API_CALL(StateWillSaveCallback);
        if(!stateWillSaveCallback) {
            return false;
        }
        if(!stateWillSaveCallback(m_handle, size, data)) {
            return false;
        }
        return true;
    }

    void Bridge::freeDataBuffer(const uint8_t* data) {
        OV_API_CHECK(void());
        auto stateSavedAsyncCallback = OV_API_CALL(StateSavedAsyncCallback);
        if(!stateSavedAsyncCallback) {
            return;
        }
        stateSavedAsyncCallback(m_handle, data);
    }

    void Bridge::initialize() {
        if(!m_libLoader->loadConfig()) {
            m_editorHelper->setError("Cannot load VST config file.");
            return;
        }
        if(!m_libLoader->loadLibrary()) {
            auto errorMessage = "Cannot load VST bridge library: " + m_libLoader->getError();
            m_editorHelper->setError(errorMessage);
            return;
        }
        auto handleCreator = OV_API_CALL(HandleCreator);
        m_handle = handleCreator();
        if(!m_handle) {
            m_editorHelper->setError("VST bridge failed to create handle.");
            return;
        }
        auto callbacksBinder = OV_API_CALL(CallbacksBinder);
        Callbacks callbacks = {
            .setDirty = [](void *editorHelper){
                static_cast<EditorHelper *>(editorHelper)->setDirty();
            },
            .setError = [](void *editorHelper, const char *error) {
                static_cast<EditorHelper *>(editorHelper)->setError(error);
            },
            .setStatus = [](void *editorHelper, const char *status) {
                static_cast<EditorHelper *>(editorHelper)->setStatus(status);
            },
        };
        callbacksBinder(m_handle, m_editorHelper, callbacks);
        auto initializer = OV_API_CALL(Initializer);
        this->m_api->setInitializationState(initializer(m_handle));
    }

    void Bridge::terminate() {
        OV_API_CHECK(void());
        auto terminator = OV_API_CALL(Terminator);
        terminator(m_handle);
        if(this->m_api->getInitializationState()) {
            this->m_api->setInitializationState(false);
        }
    }

    void Bridge::showEditorWindow() {
        OV_API_CHECK(void());
        auto windowOpener = OV_API_CALL(WindowOpener);
        windowOpener(m_handle);
    }

    void Bridge::hideEditorWindow() {
        OV_API_CHECK(void());
        auto windowCloser = OV_API_CALL(WindowCloser);
        windowCloser(m_handle);
    }

    bool Bridge::setupProcess(int32_t totalNumOutputChannels, double sampleRate, int32_t maxBlockSize) {
        auto oldMyOutput = myOutputs;
        bool success = false;
        OV_API_CHECK(false);
        auto processInitializer = OV_API_CALL(ProcessInitializer);
        if(!processInitializer(m_handle, totalNumOutputChannels, maxBlockSize, sampleRate)) {
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

    void Bridge::finalizeProcess() {
        OV_API_CHECK(void());
        auto processFinalizer = OV_API_CALL(ProcessFinalizer);
        processFinalizer(m_handle);
    }
}
