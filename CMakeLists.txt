cmake_minimum_required(VERSION 3.20)

project(CoreVisionUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV CONFIG REQUIRED)

add_executable(CoreVisionUtils
    ImageProcessing/ImageCodecSample.cpp
    ImageProcessing/ImageBuffer.cpp
    ImageProcessing/ImageCodec.cpp
)

target_include_directories(CoreVisionUtils
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ImageProcessing
)

target_link_libraries(CoreVisionUtils
    PRIVATE
        ${OpenCV_LIBS}
)