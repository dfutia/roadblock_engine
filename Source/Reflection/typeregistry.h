#pragma once
#ifndef TYPE_REGISTRY_H
#define TYPE_REGISTRY_H

#include "Reflection/typedproperty.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>

class Reflectable;
class Property;
class Instance;

class TypeRegistry {
public:
	template<typename T>
	void registerType(const std::string& name) {
		creators[name] = []() { return std::make_unique<T>(); };
        registeredTypeNames.push_back(name);
	}

    template<typename Class, typename T>
    void registerProperty(const std::string& className, const std::string& propName, const std::string& section,
        T(Class::* getter)() const, void (Class::* setter)(const T&)) {
        properties[className].push_back(std::make_unique<TypedProperty<Class, T>>(propName, section, getter, setter));
    }

    std::unique_ptr<Instance> createInstance(const std::string& name);

    const std::vector<std::string>& getRegisteredTypes() const;
    const std::vector<std::unique_ptr<Property>>& getProperties(const std::string& typeName) const;
private:
    std::unordered_map<std::string, std::function<std::unique_ptr<Reflectable>()>> creators;
    std::vector<std::string> registeredTypeNames;
    std::unordered_map<std::string, std::vector<std::unique_ptr<Property>>> properties;
};

extern TypeRegistry gTypeRegistry;

#define REGISTER_TYPE(TypeName) \
    gTypeRegistry.registerType<TypeName>(#TypeName)

#define REGISTER_PROPERTY(ClassName, PropName, Section, GetterName, SetterName) \
    gTypeRegistry.registerProperty<ClassName>(#ClassName, #PropName, Section, &ClassName::GetterName, &ClassName::SetterName)

#endif 
