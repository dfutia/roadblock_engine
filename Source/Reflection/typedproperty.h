#pragma once
#ifndef TYPED_PROPERTY_H
#define TYPED_PROPERTY_H

#include "Reflection/property.h"
#include "Asset/assetmanager.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <typeinfo>
#include <string>

//using Position = glm::vec3;
//using Rotation = glm::vec3;
//using Scale = glm::vec3;
//using Color3 = glm::vec3;
//using Color4 = glm::vec4;

template<typename Class, typename T>
class TypedProperty : public Property {
public:
    using GetterFunc = T(Class::*)() const;
    using SetterFunc = void (Class::*)(const T&);

    TypedProperty(const std::string& name, const std::string& section, GetterFunc getter, SetterFunc setter) : 
        Property(name, section), 
        getter(getter), setter(setter) {}

    const std::string& getName() const override { return name; }

    const std::string& getTypeName() const override {
        static std::string typeName = getDetailedTypeName();
        return typeName;
    }

    void* getValue(const Reflectable* object) const override {
        const Class* obj = dynamic_cast<const Class*>(object);
        if (obj) {
            static T value;
            value = (obj->*getter)();
            return &value;
        }
        return nullptr;
    }

    void setValue(Reflectable* object, const void* value) const override {
        Class* obj = dynamic_cast<Class*>(object);
        if (obj && value) {
            (obj->*setter)(*static_cast<const T*>(value));
        }
    }
private:
    static std::string getDetailedTypeName() {
        if (std::is_same<T, std::string>::value) return "std::string";
        if (std::is_same<T, float>::value) return "float";
        if (std::is_same<T, glm::vec3>::value) return "glm::vec3";
        if (std::is_same<T, glm::vec4>::value) return "glm::vec4";
        if (std::is_same<T, std::shared_ptr<MeshHandle>>::value) return "std::shared_ptr<MeshHandle>";
        return "unknown";
    }

    std::string name;
    GetterFunc getter;
    SetterFunc setter;
};

#endif 