#include "Scene/Nodes/instance.h"
#include "Scene/Nodes/model.h"
#include "Scene/Nodes/part.h"

void Instance::createBindings(sol::state& lua) {
    lua.new_usertype<Instance>("Instance",
        "new", sol::no_constructor,
        "SetParent", &Instance::setParent,
        "GetChildren", &Instance::getChildren,
        "GetDescendants", &Instance::getDescendants
    );
}

Instance::Instance(const std::string& name) : m_name(name) {}
Instance::~Instance() {}

std::string Instance::getTypeName() const {
    return "Instance";
}

Instance* Instance::getParent() {
    return m_parent;
}

void Instance::setParent(Instance* parent) {
    if (this->m_parent) {
        auto it = std::find(this->m_parent->m_children.begin(), this->m_parent->m_children.end(), this);
        if (it != this->m_parent->m_children.end()) {
            this->m_parent->m_children.erase(it);
        }
    }
    this->m_parent = parent;
    if (parent) {
        parent->m_children.push_back(this);
    }
}

std::vector<Instance*> Instance::getChildren() {
    return m_children;
}

std::vector<Instance*> Instance::getDescendants() {
    std::vector<Instance*> descendants;
    for (auto child : m_children) {
        descendants.push_back(child);
        auto childDescendants = child->getDescendants();
        descendants.insert(descendants.end(), childDescendants.begin(), childDescendants.end());
    }
    return descendants;
}