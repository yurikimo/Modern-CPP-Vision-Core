#include "ImageBuffer.h"
#include "SafeImageSize.h"

#include <iostream>
#include <algorithm>

#define IMAGE_BUFFER_INDEX(x, y, c) ((static_cast<size_t>(y) * static_cast<size_t>(width_) + static_cast<size_t>(x)) * static_cast<size_t>(channelCount_) + static_cast<size_t>(c))

ImageBuffer::ImageBuffer()
{
    std::cout << "Buffer created" << '\n';
}

ImageBuffer::ImageBuffer(const int inWidth, const int inHeight, const int inChannelCount)
{
    std::cout << "Buffer created with parameters" << '\n';
    
    size_ = SafeImageSize(inWidth, inHeight, inChannelCount);
    
    width_ = inWidth;
    height_ = inHeight;
    channelCount_ = inChannelCount;
    
    data_ = new float[size_]();
}

//Destructor (RAII ensures no leaks)
ImageBuffer::~ImageBuffer()
{
    Release();
    
    std::cout << "Buffer deleted" << '\n';
}

//Moves: steal data from source
ImageBuffer::ImageBuffer(ImageBuffer&& other) noexcept: size_(other.size_), width_(other.width_), height_(other.height_), 
                                                        channelCount_(other.channelCount_), data_(other.data_)
{
    std::cout << "Buffer created for moving" << '\n';
    
    other.size_ = 0;
    other.width_ = 0;
    other.height_ = 0;
    other.channelCount_ = 0;
    other.data_ = nullptr;
}
    
void ImageBuffer::Release()
{
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    width_ = 0;
    height_ = 0;
    channelCount_ = 0;
}
ImageBuffer& ImageBuffer::operator=(ImageBuffer&& other) noexcept
{
    if (this != &other)
    {
        Release();
            
        size_ = other.size_;
        width_ = other.width_;
        height_ = other.height_;
        channelCount_ = other.channelCount_;
        data_ = other.data_;
            
        
        other.size_ = 0;
        other.width_ = 0;
        other.height_ = 0;
        other.channelCount_ = 0;
        other.data_ = nullptr;
    }
        
    return *this;
}
    
void ImageBuffer::Fill(const float inValue)
{
    if (!data_)
    {
        std::cout << "Buffer not allocated" << '\n';
        return;
    }
        
    std::fill_n(data_, size_, inValue);
}
    
float* ImageBuffer::GetData()
{
    return data_;
}

const float* ImageBuffer::GetData() const
{
    return data_;
}

float& ImageBuffer::At(const int x, const int y, const int c)
{
    const size_t index = IMAGE_BUFFER_INDEX(x, y, c);
    
    return data_[index];
}

const float& ImageBuffer::At(const int x, const int y, const int c) const
{
    const size_t index = IMAGE_BUFFER_INDEX(x, y, c);
    
    return data_[index];
}

float& ImageBuffer::AtSafe(const int x, const int y, const int c)
{
    if (!data_)
    {
        throw std::runtime_error("Buffer not allocated");
    }
    
    if (x < 0 || x >= width_ || y < 0 || y >= height_ || c < 0 || c >= channelCount_)
    {
        throw std::out_of_range("Pixel index out of range");
    }
    
    return At(x, y, c);
}

const float& ImageBuffer::AtSafe(const int x, const int y, const int c) const
{
    if (!data_)
    {
        throw std::runtime_error("Buffer not allocated");
    }
    
    if (x < 0 || x >= width_ || y < 0 || y >= height_ || c < 0 || c >= channelCount_)
    {
        throw std::out_of_range("Pixel index out of range");
    }
    
    return At(x, y, c);
}

size_t ImageBuffer::GetSize() const
{
    return size_;
}

int ImageBuffer::GetWidth() const
{
    return width_;
}

int ImageBuffer::GetHeight() const
{
    return height_;
}

int ImageBuffer::GetChannelCount() const
{
    return channelCount_;
}

#undef IMAGE_BUFFER_INDEX