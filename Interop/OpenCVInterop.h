#pragma once
#include <ImageBuffer.h>
#include <opencv2/core/mat.hpp>

class OpenCVInterop
{
    public:
        static cv::Mat ToCvMat(ImageBuffer& imageBuffer);
    
        static ImageBuffer FromCvMat(const cv::Mat& mat);
};
