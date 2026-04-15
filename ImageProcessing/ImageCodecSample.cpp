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
        
        std::cout << "Image loaded and saved successfully" << '\n';
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what() << '\n';    
        
        return 1;
    }
    
    return 0;
}
