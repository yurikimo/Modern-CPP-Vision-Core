# CoreVisionUtils – ImageBuffer, ImageCodec, and OpenCV Interop

This project demonstrates a small, modern C++ image-processing pipeline built around a custom `ImageBuffer`, safe file I/O, and zero-copy OpenCV interoperability.

The goal is to keep the core image code simple, explicit, showing real computer-vision workflows.

---

## Why OpenCV is integrated

OpenCV is used as an industry-standard computer-vision library for:

- image transformations
- grayscale conversion
- blurring and filtering
- validating custom image-processing code
- experimenting with real CV workflows

OpenCV is added **beside** the custom image pipeline, not as a replacement for it.

The custom `ImageBuffer` remains useful because it demonstrates:

- RAII ownership
- move-only resource management
- safe pixel access
- normalized float image storage
- explicit memory layout
- controlled interop with external libraries

---

## Architecture

```text
CoreVisionUtils/
    CMakeLists.txt
    README.md

    ImageProcessing/
        ImageBuffer.h
        ImageBuffer.cpp
        ImageCodec.h
        ImageCodec.cpp
        ImageCodecSample.cpp
        SafeImageSize.h
        stb_image.h
        stb_image_write.h

    Interop/
        OpenCVInterop.h
        OpenCVInterop.cpp
```

### `ImageBuffer`

Owns image memory.

Responsibilities:

- store width, height, and channel count
- own pixel data using RAII
- prevent accidental copying of large buffers
- allow move semantics
- provide full-buffer and subspan access
- provide safe and fast pixel access

The internal data format is:

```text
float values in range [0.0, 1.0]
interleaved layout
RGB:  [R, G, B, R, G, B, ...]
Gray: [Y, Y, Y, ...]
```

Indexing formula:

```cpp
index = (y * width + x) * channelCount + c;
```

---

### `ImageCodec`

Handles file I/O only.

Responsibilities:

- load RGB images from disk using STB
- convert `uint8 [0..255]` into `float [0..1]`
- save `ImageBuffer` data back to PNG
- support 1-channel grayscale and 3-channel RGB buffers

Important design decision:

> `ImageCodec` does **not** know about OpenCV.

This keeps file I/O independent from computer-vision library interop.

---

### `OpenCVInterop`

Bridges `ImageBuffer` and `cv::Mat`.

Responsibilities:

- create a zero-copy `cv::Mat` view over `ImageBuffer` memory
- convert OpenCV output back into an `ImageBuffer`
- keep OpenCV-specific logic outside `ImageCodec`

This separation keeps the architecture clean:

```text
ImageCodec      -> ImageBuffer <-> files
OpenCVInterop   -> ImageBuffer <-> cv::Mat
```

---

## Key APIs

### Load image

```cpp
std::optional<ImageBuffer> imageBuffer =
    ImageCodec::LoadRgbFromFile("Resources\\laika.png");
```

The return type is `std::optional<ImageBuffer>` because loading can fail.

This avoids invalid sentinel values and forces the caller to handle failure:

```cpp
if (!imageBuffer.has_value())
{
    std::cout << "Failed to load image" << std::endl;
    return 1;
}
```

---

### Save image

```cpp
ImageCodec::SaveToPng(imageBuffer.value(), "Output\\output.png");
```

`SaveToPng` supports:

- 1-channel grayscale
- 3-channel RGB
- float input in range `[0.0, 1.0]`

Values are clamped and converted to bytes before saving.

---

### Save from raw span

```cpp
ImageCodec::SaveToPng(
    "Output\\output.png",
    imageBuffer.GetSpan(),
    imageBuffer.GetWidth(),
    imageBuffer.GetHeight(),
    imageBuffer.GetChannelCount()
);
```

This overload keeps saving flexible without exposing OpenCV to `ImageCodec`.

---

## OpenCV Interop

### Zero-copy `ImageBuffer` to `cv::Mat`

```cpp
cv::Mat image = OpenCVInterop::ToCvMat(imageBuffer.value());
```

This does **not** copy image data.

It creates a `cv::Mat` view over the existing `ImageBuffer` memory.

Important:

- The returned cv::Mat is a non-owning view.
- The ImageBuffer must outlive the cv::Mat.
- Modifying the cv::Mat modifies the underlying ImageBuffer.

This is useful for performance because OpenCV can process the existing memory directly.

---

### OpenCV output back to `ImageBuffer`

```cpp
ImageBuffer output = OpenCVInterop::FromCvMat(gray);
```

`FromCvMat` copies OpenCV output into a new `ImageBuffer`.

Current supported OpenCV formats:

- `CV_32FC1`
- `CV_32FC3`

The function handles both:

- continuous matrices with one `memcpy`
- non-contiguous matrices with row-by-row copies

For non-contiguous matrices, copying must be done per row:

```cpp
for (int row = 0; row < mat.rows; ++row)
{
    const float* srcRow = mat.ptr<float>(row);
    float* dstRow = imageBuffer.GetData() + row * rowElementCount;

    std::memcpy(dstRow, srcRow, rowElementCount * sizeof(float));
}
```

This avoids duplicating the first row or writing repeatedly to the start of the destination buffer.

---

## Gaussian blur explanation

```cpp
cv::GaussianBlur(gray, gray, cv::Size(7, 7), 0.5);
```

Meaning:

- first `gray`: source image
- second `gray`: destination image
- `cv::Size(7, 7)`: blur kernel size
- `0.5`: blur strength / Gaussian sigma

The kernel size must be odd.

Larger kernel or sigma values produce stronger blur.

---

## Build setup with vcpkg and OpenCV

### Install vcpkg

```powershell
cd C:\Development\CPP
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

### Install OpenCV

```powershell
.\vcpkg install opencv4:x64-windows
```

Verify:

```powershell
.\vcpkg list | findstr opencv
```

---

## CMake setup

Example `CMakeLists.txt`:

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
    Interop/OpenCVInterop.cpp
)

target_include_directories(CoreVisionUtils
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ImageProcessing
        ${CMAKE_CURRENT_SOURCE_DIR}/Interop
)

target_link_libraries(CoreVisionUtils
    PRIVATE
        ${OpenCV_LIBS}
)
```

Configure from the project root:

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

## Rider setup

Open the folder:

```text
C:\Development\CPP\CoreVisionUtils
```

Do not open the Visual Studio solution.

Go to:

```text
File → Settings → Build, Execution, Deployment → CMake
```

Add CMake options:

```text
-DCMAKE_TOOLCHAIN_FILE=C:/Development/CPP/vcpkg/scripts/buildsystems/vcpkg.cmake
-DVCPKG_TARGET_TRIPLET=x64-windows
-DOpenCV_DIR=C:/Development/CPP/vcpkg/installed/x64-windows/share/opencv4
```

Then reload:

```text
File → Reload CMake Project
```

Set the run configuration working directory to:

```text
C:\Development\CPP\CoreVisionUtils
```

This allows relative paths like these to work:

```text
Resources\laika.png
Output\gray_laika.png
```

---

## Debugging in Rider

Debug/Release is controlled by the CMake profile, not by the run configuration.

Go to:

```text
File → Settings → Build, Execution, Deployment → CMake
```

Make sure the active profile is a Debug profile.

Then use the bug icon to start debugging.

If breakpoints are not hit, check:

- CMake profile is Debug
- debugger is configured under Toolchains
- executable points to the CMake target
- working directory is correct
- project was rebuilt after changes

---

## Common mistakes

### Using the wrong OpenCV Mat type

If the buffer has 3 channels, use `CV_32FC3`.

If the buffer has 1 channel, use `CV_32FC1`.

Using `CV_32FC1` for RGB data causes OpenCV channel errors during color conversion.

---

### Forgetting that `memcpy` uses bytes

Wrong:

```cpp
std::memcpy(dst, src, width * height * channelCount);
```

Correct:

```cpp
std::memcpy(dst, src, width * height * channelCount * sizeof(float));
```

---

### Assuming every `cv::Mat` is continuous

Some OpenCV matrices are views with row stride.

Always check:

```cpp
if (mat.isContinuous())
```

If not continuous, copy row by row.

---

### Confusing RGB and BGR

- STB-loaded `ImageBuffer`: RGB
- OpenCV `imread`: usually BGR

Use the correct OpenCV color conversion code based on the source.

---

## Current limitations

- `ImageCodec::LoadRgbFromFile` always loads RGB
- `ImageCodec::SaveToPng` supports only 1-channel and 3-channel buffers
- `OpenCVInterop::FromCvMat` currently supports only `CV_32F` mats
- `ToCvMat` returns a non-owning view, so lifetime must be managed carefully

---

## Validation checklist

- Load `Resources\laika.png`
- Convert `ImageBuffer` to `cv::Mat`
- Convert RGB to grayscale
- Apply Gaussian blur
- Convert `cv::Mat` back to `ImageBuffer`
- Save using `ImageCodec::SaveToPng`
- Confirm `Output\gray_laika.png` is created

Expected console output:

```text
OpenCV version: 4.x.x
Saved gray to png
```

---

## Summary

This project now demonstrates:

- custom image memory ownership
- safe loading and saving
- `std::optional` for fallible operations
- `std::span` for non-owning data access
- lambdas with STL algorithms
- OpenCV interop without coupling `ImageCodec` to OpenCV
- zero-copy `ImageBuffer` to `cv::Mat`
- safe copy-back from OpenCV output to `ImageBuffer`

This is a strong foundation for future computer-vision, real-time image processing, and AR/VR-related C++ work.
