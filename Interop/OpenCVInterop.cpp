#include "OpenCVInterop.h"

// Returns a zero-copy, non-owning cv::Mat view over ImageBuffer memory.
// The ImageBuffer must outlive the returned cv::Mat.
// Modifying the cv::Mat modifies the underlying ImageBuffer.

cv::Mat OpenCVInterop::ToCvMat(ImageBuffer& imageBuffer)
{
    if (imageBuffer.GetChannelCount() != 1 && imageBuffer.GetChannelCount() != 3)
    {
        throw std::runtime_error("Save RGB requires a 1-channel or 3-channel buffer");
        
    }
    
    return cv::Mat(imageBuffer.GetHeight(), imageBuffer.GetWidth(),  imageBuffer.GetChannelCount() == 1 ? CV_32FC1 : CV_32FC3, imageBuffer.GetData());
}

ImageBuffer OpenCVInterop::FromCvMat(const cv::Mat& mat)
{
    if (mat.empty())
    {
        throw std::invalid_argument("Empty Mat");
    }
    
    if (mat.depth() != CV_32F)
    {
        throw std::invalid_argument("Mat type not supported");
    }
    
    const int channelCount = mat.channels();
    
    if (channelCount != 1 && channelCount != 3)
    {
        throw std::invalid_argument("Mat type not supported");
    }
    
    ImageBuffer imageBuffer(mat.cols, mat.rows, channelCount);
    
    if (mat.isContinuous())
    {
        std::memcpy(imageBuffer.GetData(), mat.ptr<float>(), imageBuffer.GetSize() * sizeof(float));
    }
    else
    {
        const size_t rowElementCount = static_cast<size_t>(mat.cols) * static_cast<size_t>(channelCount);
        
        for (int row = 0; row < mat.rows; row++)
        {
            float* dstRow = imageBuffer.GetData() + static_cast<size_t>(row) * rowElementCount;
            const float* srcRow = mat.ptr<float>(row);
            
            std::memcpy(dstRow, srcRow, rowElementCount * sizeof(float));
        }
    }
    
    return imageBuffer;
}