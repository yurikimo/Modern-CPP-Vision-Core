#pragma once
#include <iostream>
#include <ostream>
#include <span>
#include <memory>
#include <optional>

class ImageBuffer
{
private:
    size_t size_ = 0;
    std::unique_ptr<float[]> data_;
    
    int width_ = 0;
    int height_ = 0;
    int channelCount_ = 0;
    
public:
    ImageBuffer();
    //Constructor allocates memory
    explicit ImageBuffer(int inWidth, int inHeight, int inChannelCount);
    
    //Destructor (RAII ensures no leaks)
    ~ImageBuffer() = default;
    
    //Rule: No accidental copy of large buffers
    ImageBuffer(const ImageBuffer&) = delete;
    ImageBuffer& operator=(const ImageBuffer&) = delete;
    
    //Moves: steal data from source
    ImageBuffer(ImageBuffer&& other) noexcept = default;
    ImageBuffer& operator=(ImageBuffer&& other) noexcept = default;
    
    void Fill(float inValue);
    
    float* GetData();
    const float* GetData() const;
    
    std::span<float> GetSpan() {return std::span<float>(data_.get(), size_);}
    std::span<const float> GetSpan() const {return std::span<const float>(data_.get(), size_);}
    
    std::optional<std::span<float>> GetSubSpan(size_t offset, size_t count)
    {
        if (offset > size_ || count > size_ - offset)
        {
            std::cout << "Subspan out of range" << '\n';
            
            return std::nullopt;
        }
        
        return std::span<float>(data_.get() + offset, count);
    }
    
    std::optional<std::span<const float>> GetSubSpan(size_t offset, size_t count) const
    {
        if (offset > size_ || count > size_ - offset)
        {
            std::cout << "Subspan out of range" << '\n';
            
            return std::nullopt;
        }
        
        return std::span<float>(data_.get() + offset, count);
    }
    
    float& At(int x, int y, int c);
    const float& At(int x, int y, int c) const;
    
    float& AtSafe(int x, int y, int c);
    const float& AtSafe(int x, int y, int c) const;
    
    size_t GetSize() const;
   
    int GetWidth() const;
    int GetHeight() const;
    int GetChannelCount() const;
};
