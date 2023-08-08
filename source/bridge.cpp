#include "bridge.h"
#include "Api.h"
#include "libraryloader.h"

using namespace std;

namespace OpenVpi {

    Bridge::Bridge(EditorHelper *editorHelper): m_editorHelper(editorHelper), m_api(new Api), m_libLoader(new LibraryLoader(m_api.get())) {}

    bool
    Bridge::processPlayback(juce::AudioBuffer<float> &buffer, int64_t timeInSamples, bool isRealtime, bool isPlaying) {
        OV_API_CHECK(false);
        if(!m_handle->processPlayback(isRealtime, isPlaying, timeInSamples, buffer.getNumSamples(), buffer.getNumChannels(), buffer.getArrayOfWritePointers())) {
            return false;
        }
        return true;
    }

    void Bridge::loadData(int size, const char *data) {
        OV_API_CHECK(void());
        m_handle->stateChanged(size, data);
    }

    bool Bridge::saveData(int &size, const char *&data) {
        OV_API_CHECK(false);
        if(!m_handle->stateWillSave(size, data)) {
            return false;
        }
        return true;
    }

    void Bridge::freeDataBuffer(const char *data) {
        OV_API_CHECK(void());
        m_handle->stateSaved(data);
    }

    void Bridge::initialize() {
        if(!m_libLoader->loadConfig()) {
            m_editorHelper->setError("Cannot load VST config file.");
            return;
        }
        if(!m_libLoader->loadLibrary()) {
            auto errorMessage = "Cannot load VST bridge library: " + m_libLoader->getError();
            m_editorHelper->setError(errorMessage.c_str());
            return;
        }
        auto createInstance = OV_API_CALL(CreateInstance);
        m_handle = createInstance();
        if(!m_handle) {
            m_editorHelper->setError("VST bridge failed to create handle.");
            return;
        }
        m_handle->bindEditorCallbacks(m_editorHelper);
        this->m_api->setInitializationState(m_handle->initialize());
    }

    void Bridge::terminate() {
        if(this->m_api->getInitializationState()) {
            m_handle->terminate();
            this->m_api->setInitializationState(false);
        }
        auto deleteInstance = OV_API_CALL(DeleteInstance);
        if(deleteInstance) deleteInstance(m_handle);
    }

    void Bridge::showEditorWindow() {
        OV_API_CHECK(void());
        m_handle->showWindow();
    }

    void Bridge::hideEditorWindow() {
        OV_API_CHECK(void());
        m_handle->hideWindow();
    }

    bool Bridge::setupProcess(int32_t totalNumOutputChannels, double sampleRate, int32_t maxBlockSize) {
        OV_API_CHECK(false);
        return m_handle->initializeProcess(totalNumOutputChannels, maxBlockSize, sampleRate);
    }

    void Bridge::finalizeProcess() {
        OV_API_CHECK(void());
        m_handle->finalizeProcess();
    }
}
