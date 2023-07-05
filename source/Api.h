#pragma once

#ifndef DIFFSCOPE_VSTI_API_H
#define DIFFSCOPE_VSTI_API_H

#include <cstdint>
#include <map>
#include <string>

#include <IVstBridge.h>

#include "ParameterTypes.h"

#define OV_API_DEF(name, type) using name = type;
#define OV_API_CALL(name) (name) this->m_api->getHandle(#name)
#define OV_API_SET(name, handle) this->m_api->addHandle(#name, handle)
#define OV_API_CHECK(ret) if(!this->m_api->getInitializationState()) return ret

namespace OpenVpi {

    OV_API_DEF(CreateInstance, IVstBridge *(*)())
    OV_API_DEF(DeleteInstance, void (*)(IVstBridge *h))

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
