#include "ImageCodec.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

constexpr int RgbChannelCount = 3;

struct StbiImageDeleter
{
    void operator() (unsigned char* p) const noexcept
    {
        stbi_image_free(p);
    }
};

using StbiImagePtr = std::unique_ptr<unsigned char, StbiImageDeleter>;

ImageBuffer ImageCodec::LoadRgbFromFile(const std::string& inPath)
{
    int width, height, channelCount;
    
    StbiImagePtr pixels(stbi_load(inPath.c_str(), &width, &height, &channelCount, RgbChannelCount));
    
    if (!pixels)
    {
        throw std::runtime_error("Failed to load image: " + inPath);
    }
    
    ImageBuffer buffer(width, height, RgbChannelCount);
    
    float* data = buffer.GetData();
    const size_t size = buffer.GetSize();
    const unsigned char* src = pixels.get();
    
    for (size_t i = 0; i < size; i++)
    {
        data[i] = static_cast<float>(src[i]) / 255.0f;
    }
    
    return buffer;
}

bool ImageCodec::SaveRgbToPng(const ImageBuffer& inBuffer, const std::string& inPath)
{
    return SaveRgbToPng(inPath, inBuffer.GetSpan(), inBuffer.GetWidth(), inBuffer.GetHeight(), RgbChannelCount);
}

bool ImageCodec::SaveRgbToPng(const std::string& inPath, const std::span<const float> inData, const int width, const int height, const int channelCount)
{
    if (inData.data() == nullptr)
    {
        std::cout << "Buffer not allocated" << '\n';
        
        return false;
    }
    
    if (channelCount != RgbChannelCount)
    {
        std::cout << "Save RGB requires a 3-channel buffer" << '\n';
        return false;
    }
    
    const size_t size = inData.size();
    
    const float* sourceData = inData.data();
    std::vector<unsigned char> outputData(size);
    
    for (size_t i = 0; i < size; i++)
    {
        float value = sourceData[i];
        
        if (value < 0.0f)
        {
            value = 0.0f;
        }
        else if (value > 1.0f)
        {
            value = 1.0f;
        }
        
        outputData[i] = static_cast<unsigned char>(value * 255.0f);
    }
    
    const unsigned int strideInBytes = width * channelCount;
    
    const int result = stbi_write_png(
        inPath.c_str(), width, height, channelCount, 
        outputData.data(), static_cast<int>(strideInBytes)
    );
    
    if (result == 0)
    {
        std::cout << "Failed to save image: " << inPath << '\n';
        return false;
    }
    
    return true;
}
