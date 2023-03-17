//
// Created by Crs_1 on 2023/3/17.
//

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

    template<typename T>
    void Api::addHandle(std::string name, T *handle) {
        this->apiHandlesDict->insert(name, handle);
    }

    template<typename T>
    T* Api::getHandle(std::string name) {
        auto handleIter = this->apiHandlesDict->find(name);
        if(handleIter == this->apiHandlesDict->end()) return nullptr;
        else return dynamic_cast<T*>(*handleIter);
    }

    void Api::setInitializationState(bool state) {
        this->initializationState = state;
    }

    bool Api::getInitializationState() {
        return this->initializationState;
    }
} // OpenVpi