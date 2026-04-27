#include "ImageCodec.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

constexpr int RgbChannelCount = 3;
constexpr int GrayChannelCount = 1;

struct StbiImageDeleter
{
    void operator() (unsigned char* p) const noexcept
    {
        stbi_image_free(p);
    }
};

using StbiImagePtr = std::unique_ptr<unsigned char, StbiImageDeleter>;

std::optional<ImageBuffer> ImageCodec::LoadRgbFromFile(const std::string& inPath)
{
    int width, height, channelCount;
    
    StbiImagePtr pixels(stbi_load(inPath.c_str(), &width, &height, &channelCount, RgbChannelCount));
    
    if (!pixels)
    {
        std::cout << "Failed to load image: " << inPath << '\n';
        
        return std::nullopt;
    }
    
    ImageBuffer buffer(width, height, RgbChannelCount);
    
    float* data = buffer.GetData();
    const size_t size = buffer.GetSize();
    const unsigned char* sourceData = pixels.get();
    
    auto byteToFloat = [](const unsigned char p)
    {
        return static_cast<float>(p) / 255.0f;
    };
    
    std::transform(sourceData, sourceData + size, data, byteToFloat);
    
    return buffer;
}

bool ImageCodec::SaveToPng(const ImageBuffer& inBuffer, const std::string& inPath)
{
    return SaveToPng(inPath, inBuffer.GetSpan(), inBuffer.GetWidth(), inBuffer.GetHeight(), inBuffer.GetChannelCount());
}

bool ImageCodec::SaveToPng(const std::string& inPath, const std::span<const float> inData, const int width, const int height, const int channelCount)
{
    if (inData.data() == nullptr)
    {
        std::cout << "Buffer not allocated" << '\n';
        
        return false;
    }
    
    if (channelCount != GrayChannelCount && channelCount != RgbChannelCount)
    {
        std::cout << "Save RGB requires a 1-channel or 3-channel buffer" << '\n';
        return false;
    }
    
    const size_t size = inData.size();
    
    const float* sourceData = inData.data();
    
    std::vector<unsigned char> outputData(size);
    
    auto floatToByte = [](const float value)
    {
        const float clamp = std::clamp(value, 0.f, 1.f);
        
        return static_cast<unsigned char>(clamp * 255.0f);
    };
    
    std::transform(sourceData, sourceData + size, outputData.begin(), floatToByte);
    
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
