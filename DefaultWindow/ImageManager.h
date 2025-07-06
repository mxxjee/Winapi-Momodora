#pragma once
class ImageManager
{
private:
    ImageManager();
    ~ImageManager();
    ImageManager(const ImageManager& cpy) = delete;
    ImageManager& operator=(const ImageManager& cpy) = delete;

public:

    static  ImageManager* GetInstance()
    {
        if (m_Instance == nullptr)
        {
            m_Instance = new ImageManager;
        }
        return m_Instance;
    }

    static void DestroyInstance()
    {
        if (m_Instance != nullptr)
        {
            delete m_Instance;
            m_Instance = nullptr;
        }
        return;
    }



public:
    void InsertImage(const std::wstring& key, const std::wstring& filepath);
    Gdiplus::Image* GetImage(const std::wstring& key);
    void DeleteImage(const std::wstring& key);
    void Release();

private:
    static ImageManager* m_Instance;

    std::map<std::wstring, Gdiplus::Image*> m_Images;
};

