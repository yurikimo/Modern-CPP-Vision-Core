#include <ImageBuffer.h>
#include <ImageCodec.h>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "../Interop/OpenCVInterop.h"

int main()
{
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    // ------------------------------------------------------------
    // 1. Load image from disk into ImageBuffer (float RGB [0..1])
    // ------------------------------------------------------------
    std::optional<ImageBuffer> imageBuffer = ImageCodec::LoadRgbFromFile("Resources\\laika.png");
    
    if (!imageBuffer.has_value())
    {
        std::cout << "Failed to load image" << std::endl;
        return 1;
    }

    // ------------------------------------------------------------
    // 2. Create a zero-copy OpenCV view over ImageBuffer memory
    //
    // IMPORTANT:
    // - No data is copied here
    // - cv::Mat directly references ImageBuffer memory
    // - ImageBuffer must outlive cv::Mat
    // ------------------------------------------------------------
    const cv::Mat image = OpenCVInterop::ToCvMat(imageBuffer.value());
    
    // ------------------------------------------------------------
    // 3. Convert RGB → Grayscale
    //
    // NOTE:
    // - Input is RGB (not BGR)
    // - Output is 1-channel (CV_32FC1)
    // ------------------------------------------------------------
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    //Must be a odd number
    constexpr int kernelSize = 99;
    //Blur intensity    
    constexpr float sigma = 5.0f;
    
    // ------------------------------------------------------------
    // 4. Apply Gaussian Blur
    // This reduces noise before further processing
    // ------------------------------------------------------------
    cv::GaussianBlur(gray, gray, cv::Size(kernelSize, kernelSize), sigma);
    
    const ImageBuffer imageBufferGray = OpenCVInterop::FromCvMat(gray);
    // ------------------------------------------------------------
    // 6. Save result to disk
    //
    // NOTE:
    // - Supports 1-channel (grayscale) and 3-channel (RGB)
    // - Converts float [0..1] → uint8 [0..255]
    // ------------------------------------------------------------
    if (ImageCodec::SaveToPng(imageBufferGray, "Output\\gray_laika.png"))
    {
        std::cout << "Saved gray to png" << std::endl;
    }

    return 0;
}
