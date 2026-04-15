#pragma once

#include <limits>
#include <stdexcept>

inline size_t SafeImageSize(const int width, const int height, const int channelCount)
{
    // Validate inputs first
    if (width <= 0 || height <= 0 || channelCount <= 0)
    {
        throw std::invalid_argument("Invalid image dimensions");
    }

    const size_t w = static_cast<size_t>(width);
    const size_t h = static_cast<size_t>(height);
    const size_t c = static_cast<size_t>(channelCount);

    // Check width * height
    if (w > std::numeric_limits<size_t>::max() / h)
    {
        throw std::overflow_error("Image size overflow (width * height)");
    }

    const size_t pixelCount = w * h;

    // Check (width * height) * channelCount
    if (pixelCount > std::numeric_limits<size_t>::max() / c)
    {
        throw std::overflow_error("Image size overflow (pixelCount * channelCount)");
    }

    return pixelCount * c;
}