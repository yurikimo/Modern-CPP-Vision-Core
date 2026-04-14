# ImageBuffer – Modern C++ Memory & Move Semantics

This project demonstrates core modern C++ concepts:

- RAII (Resource Acquisition Is Initialization)
- Move semantics
- Copy prevention
- Stack vs Heap allocation
- lvalue vs rvalue behavior

---

## 📦 Class Overview

`ImageBuffer` manages a dynamically allocated array of `float`.

```cpp
class ImageBuffer
{
private:
    size_t Size = 0;
    float* Data = nullptr;
};
```

---

## 🧠 Memory Management (RAII)

- Memory is allocated in the constructor
- Memory is released in the destructor

```cpp
ImageBuffer(size_t InSize);
~ImageBuffer();
```

This ensures:

> No memory leaks and deterministic cleanup

---

## 🚫 Copy Disabled

```cpp
ImageBuffer(const ImageBuffer&) = delete;
ImageBuffer& operator=(const ImageBuffer&) = delete;
```

### Why?

Copying would duplicate the pointer, causing:

- Double deletion
- Undefined behavior
- Unnecessary memory allocation

---

## 🚀 Move Semantics

### Move Constructor

```cpp
ImageBuffer(ImageBuffer&& other) noexcept;
```

### Move Assignment

```cpp
ImageBuffer& operator=(ImageBuffer&& other) noexcept;
```

### Behavior

- Transfers ownership of memory
- Leaves source object in a valid empty state:
  - `Data = nullptr`
  - `Size = 0`

---

## ⚡ Why `noexcept`?

- Guarantees no exceptions
- Enables optimizations in STL containers
- Required for efficient moves in `std::vector`

---

## 🔄 lvalue vs rvalue

### lvalue

- Named object
- Has persistent memory

```cpp
imageBuffer buffer(1024);
```

### rvalue

- Temporary or explicitly marked as movable

```cpp
std::move(buffer);
```

---

## ❗ Why `std::move` is required

Without it:

```cpp
ProcessBuffer(buffer);
```

- Tries to copy → ❌ fails (copy is deleted)

With it:

```cpp
ProcessBuffer(std::move(buffer));
```

- Uses move constructor → ✅ works

---

## 🔍 Function Parameter Behavior

### ❌ By value (copy required)

```cpp
void ProcessBuffer(image_buffer buffer);
```

- Requires copy or move

---

### ✅ By reference (no copy)

```cpp
void ProcessBuffer(image_buffer& buffer);
```

- Works without copy

---

### ✅ By pointer (no copy)

```cpp
void ProcessBuffer(image_buffer* buffer);
```

- Pass address explicitly

---

## 🧪 Example Execution

```cpp
imageBuffer buffer(1024);
ProcessBuffer(std::move(buffer));
```

### Output

```
Buffer created
Buffer created for moving
Processing Buffer
Buffer deleted
Buffer is cleaned
Buffer deleted
```

### Explanation

- First delete → moved object inside function
- Second delete → original object (now empty)

---

## 🧠 Key Takeaways

- RAII guarantees safe memory handling
- Copy must be disabled for raw pointer ownership
- Move semantics enable high-performance transfers
- `std::move` does not move — it enables moving
- lvalue vs rvalue determines which constructor is used

---

## 🎯 Goal of this Project

This project is to showcase:

- Master modern C++ (C++17+)
- Low-level memory management
