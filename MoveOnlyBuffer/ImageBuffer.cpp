#include "ImageBuffer.h"

#include <iostream>

ImageBuffer::ImageBuffer(const size_t inSize): size_(inSize), data_(new float[inSize])
{
    std::cout << "Buffer created" << '\n';
}

//Destructor (RAII ensures no leaks)
ImageBuffer::~ImageBuffer()
{
    delete[] data_;
    std::cout << "Buffer deleted" << '\n';
}

//Moves: steal data from source
ImageBuffer::ImageBuffer(ImageBuffer&& other) noexcept: size_(other.size_), data_(other.data_)
{
    std::cout << "Buffer created for moving" << '\n';
    other.data_ = nullptr;
    other.size_ = 0;
}
    
ImageBuffer& ImageBuffer::operator=(ImageBuffer&& other) noexcept
{
    if (this != &other)
    {
        delete[] data_;
            
        size_ = other.size_;
        data_ = other.data_;
            
        other.data_ = nullptr;
        other.size_ = 0;
    }
        
    return *this;
}
    
float* ImageBuffer::GetData() const
{
    return data_;
}
    
void ImageBuffer::Fill(const float inValue) const
{
    if (!data_)
    {
        std::cout << "Buffer not allocated" << '\n';
        return;
    }
        
    std::fill_n(data_, size_, inValue);
}
    
size_t ImageBuffer::GetSize() const
{
    return size_;
}