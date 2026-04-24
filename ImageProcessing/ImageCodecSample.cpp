#include <iostream>

#include "ImageBuffer.h"
#include "ImageCodec.h"

int main(int argc, char* argv[])
{
    try
    {
        std::optional<ImageBuffer> imageBuffer = ImageCodec::LoadRgbFromFile("laika.png");
        
        if (!imageBuffer.has_value())
        {
            return 1;
        }
        
        if (!ImageCodec::SaveRgbToPng(imageBuffer.value(), "output.png"))
        {
            std::cout << "Save failed" << '\n';
        }
        
        const int width = imageBuffer.value().GetWidth();
        const int height = imageBuffer.value().GetHeight();
        const int channelCount = imageBuffer.value().GetChannelCount();
        
        const int croppedHeight = height / 2;
        
        const size_t rowSize = static_cast<size_t>(width) * static_cast<size_t>(channelCount);
        const size_t croppedWidth = croppedHeight * rowSize;
        
        const std::optional<std::span<const float>> topHalf = imageBuffer.value().GetSubSpan(0, croppedWidth);
        
        if (!topHalf.has_value())
        {
            return 1;
        }
        
        if (!ImageCodec::SaveRgbToPng("output_top_half.png", topHalf.value(), width, croppedHeight, channelCount))
        {
            std::cout << "Save failed" << '\n';
        }
        
        std::cout << "Image loaded and saved successfully" << '\n';
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what() << '\n';    
        
        return 1;
    }
    
    return 0;
}
