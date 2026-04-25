#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    cv::Mat image = cv::imread("Resources\\laika.png");

    if (image.empty())
    {
        std::cout << "Failed to load image" << std::endl;
        return 1;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::imwrite("Output\\opencv_output.png", gray);

    std::cout << "OpenCV test completed successfully." << std::endl;

    return 0;
}