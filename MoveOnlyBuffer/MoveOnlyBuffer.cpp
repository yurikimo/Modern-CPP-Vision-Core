#include <iostream>
#include <ostream>

#include "ImageBuffer.h"

// NOTE: This function intentionally takes ImageBuffer by value to demonstrate move semantics (ownership transfer via std::move). 
// Not intended for production use.
static void ProcessBuffer(ImageBuffer imageBuffer)
{
    std::cout << "Buffer size: " << imageBuffer.GetSize() << '\n';
}

int main(int argc, char* argv[])
{
    ImageBuffer imageBuffer(1024);
    imageBuffer.Fill(1.0);
    
    //This call will fail because the constructor was deleted
    //ProcessBuffer(imageBuffer);
    
    //This "steal" the data end gives it to the method
    ProcessBuffer(std::move(imageBuffer));
    
    //Validating if the data was cleaned
    // Not intended for production use.
    if (imageBuffer.GetData() == nullptr)
    {
        std::cout << "Buffer is cleaned" << '\n';
    }
    
    return 0;
}
