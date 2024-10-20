#pragma once
#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <memory>

class AssetHandle {
public:
    AssetHandle(const std::string& name) : m_name(name) {}
    virtual ~AssetHandle() = default;

    //std::chrono::system_clock::time_point GetLastLoadTime() const { return m_lastLoadTime; }
    bool IsLoaded() const { return m_isLoaded; }

    void setUserAsset(bool isUserAsset) { m_isUserAsset = isUserAsset; }
    bool isUserAsset() const { return m_isUserAsset; }

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
protected:
    //std::chrono::system_clock::time_point m_lastLoadTime;
    std::string m_name;
    bool m_isLoaded = false;
    bool m_isUserAsset = true;
};

template<typename T>
class TypedAssetHandle : public AssetHandle {
public:
    TypedAssetHandle(std::shared_ptr<T> resource, const std::string& name = "Unnamed Asset") : AssetHandle(name), m_resource(std::move(resource)) {
        m_isLoaded = true;
        //m_lastLoadTime = std::chrono::system_clock::now();
    }
    std::shared_ptr<T> Get() { return m_resource; }
    std::shared_ptr<const T> Get() const { return m_resource; }
private:
    std::shared_ptr<T> m_resource;
};

#endif 
