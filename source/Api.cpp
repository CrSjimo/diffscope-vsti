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
        if(instance) {
            delete instance;
        }
    }

    void Api::addHandle(std::string name, void* handle) {
        this->apiHandlesDict->insert(map<string, void*>::value_type(name, handle));
    }

    void* Api::getHandle(std::string name) {
        auto handleIter = this->apiHandlesDict->find(name);
        if(handleIter == this->apiHandlesDict->end()) return nullptr;
        else return handleIter->second;
    }

    void Api::setInitializationState(bool state) {
        this->initializationState = state;
    }

    bool Api::getInitializationState() {
        return this->initializationState;
    }
} // OpenVpi