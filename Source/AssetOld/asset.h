#pragma once
#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <memory>

class AssetBase {
public:
    virtual ~AssetBase() = default;
    virtual const std::string& getName() const = 0;
};

template<typename T>
class Asset : public AssetBase {
public:
    Asset(const std::string& name, std::shared_ptr<T> resource)
        : m_name(name), m_resource(resource) {}

    const std::string& getName() const override { return m_name; }
    std::shared_ptr<T> getResource() const { return m_resource; }

private:
    std::string m_name;
    std::shared_ptr<T> m_resource;
};
#endif 