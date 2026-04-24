🖼️ Image I/O – RGB Load & Save
===============================

This module provides a **safe, minimal, and production-ready image pipeline** built around strict ownership and predictable data representation.

* * *

🎯 Goal
-------

Provide a clean pipeline to:

* Load an image from disk

* Convert it into a normalized float buffer

* Store it safely (`ImageBuffer`)

* Save it back to disk

* Ensure correctness for future image processing

* * *

🧱 Architecture
---------------

### `ImageBuffer`

Owns and manages pixel memory.

Responsibilities:

* RAII memory ownership

* Stores metadata (width, height, channel count)

* Provides safe and fast pixel access

* Prevents undefined behavior from uninitialized reads

* * *

### `ImageCodec`

Handles all I/O and format conversion.

Responsibilities:

* Decode images (via STB)

* Encode PNG output

* Convert between:
  
  * `uint8 [0..255]` (file format)
  
  * `float [0..1]` (internal format)

* * *

📦 Data Representation
----------------------

Images are stored as:

* `float` values in range `[0.0, 1.0]`

* Interleaved layout:
    [R, G, B, R, G, B, ...]

Indexing formula:
    index = (y * width + x) * channelCount + c;

* * *

⚠️ Critical Design Guarantees
--------------------------------------------

### 1. ✅ No uninitialized memory

Pixel buffers are **always initialized**.

```cpp
    data_ = new float[size_](); // value-initialized to 0.0f
```

This prevents:

* Undefined behavior

* Random artifacts

* NaN propagation

* * *

### 2. ✅ Safe STB memory ownership (RAII)

STB allocations are wrapped immediately:

```cpp
    struct StbiImageDeleter
    {
        void operator()(unsigned char* p) const noexcept
        {
            stbi_image_free(p);
        }
    };
    using StbiImagePtr = std::unique_ptr<unsigned char, StbiImageDeleter>;
```

Usage:

```cpp
    StbiImagePtr pixels(stbi_load(...));
```

This guarantees:

* No leaks

* Exception safety

* Clean ownership transfer

* * *

### 3. ❗Byte → float conversion

**Important:** STB returns `unsigned char*`, but `ImageBuffer` stores `float`.

```cpp
    const float* dst = buffer.GetData();
    for (size_t i = 0; i < buffer.GetSize(); ++i)
    {
        dst[i] = static_cast<float>(src[i]) / 255.0f;
    }
```

* * *

### 4. ⚠️ Channel consistency

* Load currently forces **3 channels (RGB)**

* Save currently expects **RGB (3 channels)**

Make sure pipeline is consistent:
    constexpr int RgbChannelCount = 3; // recommended for now

* * *

🔄 Load Pipeline
----------------

```cpp
    ImageBuffer ImageCodec::LoadFromFile(const std::string& path)
    {
        ...
    }
```

* * *

💾 Save Pipeline
----------------

```cpp
    bool ImageCodec::SaveRgbToPng(const ImageBuffer& buffer, const std::string& path)
    {
        ...
    }
```

* * *

🧠 Key Invariants
-----------------

The system now guarantees:

* `ImageBuffer` is always valid after construction

* Pixel memory is never read uninitialized

* STB memory is never leaked

* Float/byte conversions are explicit and correct


💡 Modern C++
--------------------------

### Lambdas

Lambdas are used to define **small, localized functions inline**, without polluting the class or global scope.

Advantages:

* Improve **readability** by keeping logic close to where it is used  
* Avoid creating unnecessary helper functions  
* Help express intent clearly in transformations and algorithms  

Example:

```cpp
	auto byteToFloat = [](const unsigned char p)
    {
        return static_cast<float>(p) / 255.0f;
    };
    
    std::transform(sourceData, sourceData + size, data, byteToFloat);
````

This could also be expressed as a lambda when appropriate.

---

### `std::optional`

`std::optional` is used to represent a value that **may or may not exist**, without relying on invalid states or sentinel values.

Advantages:

* Makes absence of value **explicit and type-safe**
* Avoids undefined behavior from invalid returns (e.g., null pointers or invalid spans)
* Forces the caller to **handle failure cases explicitly**
* Improves API clarity and correctness

Example usage:

```cpp
	std::optional<std::span<float>> GetSubSpan(size_t offset, size_t count)
    {
        if (offset > size_ || count > size_ - offset)
        {
            std::cout << "Subspan out of range" << '\n';
            
            return std::nullopt;
        }
        
        return std::span<float>(data_.get() + offset, count);
    }
```

Instead of returning an invalid span or silently failing, the function can return:

* `std::nullopt` → invalid request
* valid `std::span` → safe access

---

These modern C++ features improve:

* Safety
* Expressiveness
* Maintainability

while keeping performance predictable.

```

* * *

