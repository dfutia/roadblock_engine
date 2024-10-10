#pragma once
#ifndef TYPE_REGISTRY_H
#define TYPE_REGISTRY_H

#include "Reflection/reflectable.h"
#include "Scene/Nodes/Instance.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

class TypeRegistry {
public:
	template<typename T>
	void registerType(const std::string& name) {
		creators[name] = []() { return std::make_unique<T>(); };
        registeredTypeNames.push_back(name);
	}

    //std::unique_ptr<Reflectable> createInstance(const std::string& name) {
    //    auto it = creators.find(name);
    //    if (it != creators.end()) {
    //        return it->second();
    //    }
    //    return nullptr;
    //}
    std::unique_ptr<Instance> createInstance(const std::string& name) {
        auto it = creators.find(name);
        if (it != creators.end()) {
            return std::unique_ptr<Instance>(dynamic_cast<Instance*>(it->second().release()));
        }
        return nullptr;
    }

    const std::vector<std::string>& getRegisteredTypes() const {
        return registeredTypeNames;
    }
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<Reflectable>()>> creators;
    std::vector<std::string> registeredTypeNames;
};

extern TypeRegistry gTypeRegistry;

#define REGISTER_TYPE(TypeName) \
    gTypeRegistry.registerType<TypeName>(#TypeName)

#endif 
