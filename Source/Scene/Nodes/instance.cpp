#include "Scene/Nodes/instance.h"
#include "Scene/Nodes/model.h"
#include "Scene/Nodes/part.h"

Instance::Instance() {}
Instance::~Instance() {}

std::string Instance::getTypeName() const {
    return "Instance";
}

void Instance::setParent(Instance* parent) {
    if (this->parent) {
        auto it = std::find(this->parent->children.begin(), this->parent->children.end(), this);
        if (it != this->parent->children.end()) {
            this->parent->children.erase(it);
        }
    }
    this->parent = parent;
    if (parent) {
        parent->children.push_back(this);
    }
}

std::vector<Instance*> Instance::getChildren() {
    return children;
}

std::vector<Instance*> Instance::getDescendants() {
    std::vector<Instance*> descendants;
    for (auto child : children) {
        descendants.push_back(child);
        auto childDescendants = child->getDescendants();
        descendants.insert(descendants.end(), childDescendants.begin(), childDescendants.end());
    }
    return descendants;
}

void Instance::createBindings(sol::state& lua) {
    lua.new_usertype<Instance>("Instance",
        "new", sol::no_constructor,
        "SetParent", &Instance::setParent,
        "GetChildren", &Instance::getChildren,
        "GetDescendants", &Instance::getDescendants
    );
}

glm::mat4 Instance::getTransform() { return transform; }