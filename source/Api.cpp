#include "Api.h"

using namespace std;

namespace OpenVpi {
    Api::Api() {
        this->apiHandlesDict = new map<string, void*>;
        this->initializationState = false;
    }
    Api::~Api() {
        delete this->apiHandlesDict;
    }

    Api* Api::instance = nullptr;

    Api *Api::getInstance() {
        if(instance) {
            return instance;
        } else {
            return instance = new Api;
        }
    }

    void Api::destroyInstance() {
        delete instance;
        instance = nullptr;
    }

    void Api::addHandle(const std::string& name, void* handle) {
        this->apiHandlesDict->insert(map<string, void*>::value_type(name, handle));
    }

    void* Api::getHandle(const std::string& name) {
        auto handleIter = this->apiHandlesDict->find(name);
        if(handleIter == this->apiHandlesDict->end()) return nullptr;
        else return handleIter->second;
    }

    void Api::setInitializationState(bool state) {
        this->initializationState = state;
    }

    bool Api::getInitializationState() const {
        return this->initializationState;
    }
} // OpenVpi