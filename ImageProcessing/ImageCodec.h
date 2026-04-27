#pragma once

#include <optional>

#include "ImageBuffer.h"

#include <string>
#include <span>

class ImageCodec
{
public:
    static std::optional<ImageBuffer> LoadRgbFromFile(const std::string& inPath);
    static bool SaveToPng(const ImageBuffer& inBuffer, const std::string& inPath);
    
    static bool SaveToPng(const std::string& inPath, std::span<const float> inData, int width, int height, int channelCount);
};
