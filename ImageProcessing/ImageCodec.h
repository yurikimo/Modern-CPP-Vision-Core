#pragma once

#include <string>
#include "ImageBuffer.h"

class ImageCodec
{
public:
    static ImageBuffer LoadRgbFromFile(const std::string& inPath);
    static bool SaveRgbToPng(const ImageBuffer& inBuffer, const std::string& inPath);
};
