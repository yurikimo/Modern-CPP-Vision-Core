#pragma once
#include <cstddef>

class ImageBuffer
{
private:
    size_t size_ = 0;
    float* data_ = nullptr;
    
    int width_ = 0;
    int height_ = 0;
    int channelCount_ = 0;
    
    void Release();
public:
    ImageBuffer();
    //Constructor allocates memory
    explicit ImageBuffer(int inWidth, int inHeight, int inChannelCount);
    
    //Destructor (RAII ensures no leaks)
    ~ImageBuffer();
    
    //Rule: No accidental copy of large buffers
    ImageBuffer(const ImageBuffer&) = delete;
    ImageBuffer& operator=(const ImageBuffer&) = delete;
    
    //Moves: steal data from source
    ImageBuffer(ImageBuffer&& other) noexcept;
    ImageBuffer& operator=(ImageBuffer&& other) noexcept;
    
    void Fill(float inValue);
    
    float* GetData();
    const float* GetData() const;
    
    float& At(int x, int y, int c);
    const float& At(int x, int y, int c) const;
    
    float& AtSafe(int x, int y, int c);
    const float& AtSafe(int x, int y, int c) const;
    
    size_t GetSize() const;
   
    int GetWidth() const;
    int GetHeight() const;
    int GetChannelCount() const;
};
