#pragma once

#include "ImageBuffer.h"

#include <string>
#include <span>

class ImageCodec
{
public:
    static ImageBuffer LoadRgbFromFile(const std::string& inPath);
    static bool SaveRgbToPng(const ImageBuffer& inBuffer, const std::string& inPath);
    
    static bool SaveRgbToPng(const std::string& inPath, std::span<const float> inData, int width, int height, int channelCount);
};
