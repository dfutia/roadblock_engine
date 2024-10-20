#pragma once
#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <memory>

class AssetHandle {
public:
    virtual ~AssetHandle() = default;

    const std::string& GetName() const { return m_name; }
    //std::chrono::system_clock::time_point GetLastLoadTime() const { return m_lastLoadTime; }
    bool IsLoaded() const { return m_isLoaded; }
    void setUserAsset(bool isUserAsset) { m_isUserAsset = isUserAsset; }
    bool isUserAsset() const { return m_isUserAsset; }
protected:
    std::string m_name;
    //std::chrono::system_clock::time_point m_lastLoadTime;
    bool m_isLoaded = false;
    bool m_isUserAsset = true;
};

template<typename T>
class TypedAssetHandle : public AssetHandle {
public:
    TypedAssetHandle(std::shared_ptr<T> resource) : m_resource(std::move(resource)) {
        m_isLoaded = true;
        //m_lastLoadTime = std::chrono::system_clock::now();
    }
    std::shared_ptr<T> Get() { return m_resource; }
    std::shared_ptr<const T> Get() const { return m_resource; }
private:
    std::shared_ptr<T> m_resource;
};

#endif 
