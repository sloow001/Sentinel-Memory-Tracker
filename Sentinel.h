#pragma once
#include <cstddef>
#include <string>

namespace Sentinel {
    void* allocate(std::size_t size, const char* file, int line);
    void deallocate(void* ptr);
    void reportLeaks();
}

// Macros
#define SENTINEL_NEW(size) Sentinel::allocate(size, __FILE__, __LINE__)
#define SENTINEL_DELETE(ptr) Sentinel::deallocate(ptr)