```md
# OpenCV Integration

## Why integrate OpenCV?

OpenCV is a mature computer vision library used in real-world image processing, robotics, AR, automation, and AI-related applications.

This project already has a custom `ImageBuffer` implementation to demonstrate modern C++ concepts such as:

- RAII
- move semantics
- safe memory ownership
- bounds checking
- image loading and saving
- basic image manipulation

OpenCV is added to complement this work, not replace it.

The goal is to use OpenCV for:

- validating image processing results
- comparing custom implementations with industry-standard behavior
- loading, saving, and transforming images quickly
- experimenting with filters, color conversion, resizing, and computer vision algorithms
- making the project more practical and relevant for real-world C++ development

The custom `ImageBuffer` remains useful because it shows how image memory can be managed manually and safely in modern C++.

OpenCV adds real-world tooling on top of that foundation.

---

# Installing OpenCV 4.x with vcpkg

## 1. Install vcpkg

Clone vcpkg:

```powershell
cd C:\Development\CPP
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

---

## 2. Install OpenCV

Install OpenCV for 64-bit Windows:

```powershell
.\vcpkg install opencv4:x64-windows
```

To verify the installation:

```powershell
.\vcpkg list | findstr opencv
```

You should see something similar to:

```text
opencv4:x64-windows
```

---

# CMake Setup

Create or update `CMakeLists.txt` in the project root:

```cmake
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
```

---

# Configure the Project

From the project root:

```powershell
cd C:\Development\CPP\CoreVisionUtils
```

Remove the previous CMake cache if needed:

```powershell
Remove-Item -Recurse -Force build
```

Configure the project:

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="C:/Development/CPP/vcpkg/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows `
  -DOpenCV_DIR="C:/Development/CPP/vcpkg/installed/x64-windows/share/opencv4"
```

Build:

```powershell
cmake --build build
```

---

# Rider Setup

## 1. Open the project folder

In Rider, open the folder:

```text
C:\Development\CPP\CoreVisionUtils
```

Do not open the `.sln` file.

---

## 2. Configure CMake options

Go to:

```text
File → Settings → Build, Execution, Deployment → CMake
```

Add these CMake options:

```text
-DCMAKE_TOOLCHAIN_FILE=C:/Development/CPP/vcpkg/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-windows
-DOpenCV_DIR=C:/Development/CPP/vcpkg/installed/x64-windows/share/opencv4
```

Then click:

```text
Reload CMake Project
```

---

## 3. Set the working directory

If the program loads files such as:

```cpp
cv::imread("Resources\\laika.png");
```

set the working directory in Rider:

```text
Run → Edit Configurations
```

Set:

```text
Working directory:
C:\Development\CPP\CoreVisionUtils
```

This allows the executable to find images stored in the `ImageProcessing` folder.

---

# Quick OpenCV Test

Use this code to verify OpenCV is working:

```cpp
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
```

Expected output:

```text
OpenCV version: 4.x.x
OpenCV test completed successfully.
```

A new file should be generated:

```text
opencv_output.png
```

---

# Notes

The build folder created by Rider, such as:

```text
cmake-build-debug
```

is normal.

That folder is only the build output directory.
The project source remains in:

```text
C:\Development\CPP\CoreVisionUtils
```

If image files are not found, check the Rider working directory.

```

```

## ⚠️ Important (Rider users)

If Rider cannot find OpenCV, it means the CMake options were not applied.

### Steps to fix:

1. Go to:
   File → Settings → Build, Execution, Deployment → CMake

2. Make sure to have CMAKE  options set as:
   -DCMAKE_TOOLCHAIN_FILE=C:/Development/CPP/vcpkg/scripts/buildsystems/vcpkg.cmake
   -DVCPKG_TARGET_TRIPLET=x64-windows
   -DOpenCV_DIR=C:/Development/CPP/vcpkg/installed/x64-windows/share/opencv4

3. Delete the CMake cache folder:
   cmake-build-debug/

4. Click:
   Reload CMake Project

5. Verify in the CMake output that OpenCV is found


