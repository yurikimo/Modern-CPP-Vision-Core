#include <iostream>

#include "ImageBuffer.h"
#include "ImageCodec.h"

int main(int argc, char* argv[])
{
    try
    {
        const ImageBuffer imageBuffer = ImageCodec::LoadRgbFromFile("laika.png");
        
        if (!ImageCodec::SaveRgbToPng(imageBuffer, "output.png"))
        {
            std::cout << "Save failed" << '\n';
        }
        
        const int width = imageBuffer.GetWidth();
        const int height = imageBuffer.GetHeight();
        const int channelCount = imageBuffer.GetChannelCount();
        
        const int croppedHeight = height / 2;
        
        const size_t rowSize = static_cast<size_t>(width) * static_cast<size_t>(channelCount);
        const size_t croppedWidth = croppedHeight * rowSize;
        
        const std::span<const float> topHalf = imageBuffer.GetSubSpan(0, croppedWidth);
        
        if (!ImageCodec::SaveRgbToPng("output_top_half.png", topHalf, width, croppedHeight, channelCount))
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
