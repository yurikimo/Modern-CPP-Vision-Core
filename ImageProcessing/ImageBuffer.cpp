//AtSafe() checks bounds; At() does not.

#include "ImageBuffer.h"
#include "SafeImageSize.h"

#include <iostream>
#include <algorithm>

#define IMAGE_BUFFER_INDEX(x, y, c) ((static_cast<size_t>(y) * static_cast<size_t>(width_) + static_cast<size_t>(x)) * static_cast<size_t>(channelCount_) + static_cast<size_t>(c))

ImageBuffer::ImageBuffer()
{
    std::cout << "Buffer created" << '\n';
}

ImageBuffer::ImageBuffer(const int inWidth, const int inHeight, const int inChannelCount): size_(SafeImageSize(inWidth, inHeight, inChannelCount)), data_(std::make_unique<float[]>(size_)), width_(inWidth), height_(inHeight), channelCount_(inChannelCount)
{
    std::cout << "Buffer created with parameters" << '\n';
}
    
void ImageBuffer::Fill(const float inValue)
{
    if (!data_)
    {
        std::cout << "Buffer not allocated" << '\n';
        return;
    }
        
    std::fill_n(data_.get(), size_, inValue);
}
    
float* ImageBuffer::GetData()
{
    return data_.get();
}

const float* ImageBuffer::GetData() const
{
    return data_.get();
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