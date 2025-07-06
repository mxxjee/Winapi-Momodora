#include "pch.h"
#include "ImageManager.h"
#define new new


ImageManager* ImageManager::m_Instance = nullptr;



ImageManager::ImageManager() {}
ImageManager::~ImageManager() { Release(); }



void ImageManager::InsertImage(const std::wstring& key, const std::wstring& filepath) {
    if (m_Images.find(key) == m_Images.end()) {
        m_Images[key] = new Gdiplus::Image(filepath.c_str());
    }
}


Gdiplus::Image* ImageManager::GetImage(const std::wstring& key) {
    auto iter = m_Images.find(key);
    if (iter != m_Images.end()) return iter->second;
    return nullptr;
}


void ImageManager::DeleteImage(const std::wstring& key) {
    auto iter = m_Images.find(key);
    if (iter != m_Images.end()) {
        delete iter->second;
        m_Images.erase(iter);
    }
}


void ImageManager::Release() {
    for (auto& pair : m_Images) {
        if (pair.second != nullptr)
            delete pair.second;
    }
    m_Images.clear();
}

#define new DBG_NEW