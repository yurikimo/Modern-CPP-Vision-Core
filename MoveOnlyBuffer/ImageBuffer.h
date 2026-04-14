#pragma once
#include <cstddef>

class ImageBuffer
{
private:
    size_t size_ = 0;
    float* data_ = nullptr;
    
public:
    
    //Constructor allocates memory
    explicit ImageBuffer(size_t inSize);
    
    //Destructor (RAII ensures no leaks)
    ~ImageBuffer();
    
    //Rule: No accidental copy of large buffers
    ImageBuffer(const ImageBuffer&) = delete;
    ImageBuffer& operator=(const ImageBuffer&) = delete;
    
    //Moves: steal data from source
    ImageBuffer(ImageBuffer&& other) noexcept;
    ImageBuffer& operator=(ImageBuffer&& other) noexcept;
    
    float* GetData() const;
    
    void Fill(float inValue) const;
    
    size_t GetSize() const;
};
