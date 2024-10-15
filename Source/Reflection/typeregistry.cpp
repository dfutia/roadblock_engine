#include "Reflection/typeregistry.h"
//#include "Reflection/reflectable.h"
//#include "Reflection/typedproperty.h"
#include "Reflection/property.h"
#include "Scene/Nodes/Instance.h"

TypeRegistry gTypeRegistry;

std::unique_ptr<Instance> TypeRegistry::createInstance(const std::string& name) {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return std::unique_ptr<Instance>(dynamic_cast<Instance*>(it->second().release()));
    }
    return nullptr;
}

const std::vector<std::string>& TypeRegistry::getRegisteredTypes() const {
    return registeredTypeNames;
}

const std::vector<std::unique_ptr<Property>>& TypeRegistry::getProperties(const std::string& typeName) const {
    static std::vector<std::unique_ptr<Property>> empty;
    auto it = properties.find(typeName);
    return (it != properties.end()) ? it->second : empty;
}
