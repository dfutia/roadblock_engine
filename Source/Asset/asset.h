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

protected:
    std::string m_name;
    //std::chrono::system_clock::time_point m_lastLoadTime;
    bool m_isLoaded = false;
};

template<typename T>
class TypedAssetHandle : public AssetHandle {
public:
    TypedAssetHandle(T* resource) : m_resource(resource) {
        m_isLoaded = true;
        //m_lastLoadTime = std::chrono::system_clock::now();
    }

    T* Get() { return m_resource.get(); }
    const T* Get() const { return m_resource.get(); }

private:
    std::unique_ptr<T> m_resource;
};

#endif 
