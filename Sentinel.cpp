#include <iostream>
#include "Sentinel.h"
#include <unordered_map>
#include <mutex>

namespace {
    struct AllocationInfo{
        std::size_t size;
        std::string file;
        int line;
    };

    // Tracks active allocations
    std::unordered_map<void*, AllocationInfo> allocations;

    // Ensures thread-safe access to the allocation tracker
    std::mutex allocMutex;
}

namespace Sentinel {
    // Allocates memory and registers the allocation for tracking
    void* allocate(std::size_t size, const char* file, int line){
        void* ptr = std::malloc(size);
        if (!ptr) throw std::bad_alloc{};

        // Protect shared allocation map
        std::lock_guard<std::mutex> lock(allocMutex);
        allocations[ptr] = { size, file, line };

        return ptr;
    }

    void deallocate(void* ptr){
        if (!ptr) return;

        std::lock_guard<std::mutex> lock(allocMutex);

        auto it = allocations.find(ptr);
        if (it == allocations.end()){
            std::cerr << "[Sentinel] WARNING: Freeing unknown pointer: " << ptr << "\n";
            return;
        }

        allocations.erase(it);
        std::free(ptr);
    }

    void reportLeaks(){
        std::lock_guard<std::mutex> lock(allocMutex);

        if (allocations.empty()){
            std::cout << "[Sentinel] No memory leaks detected.\n";
            return;
        }

        std::cout << "\n[Sentinel] MEMORY LEAKS DETECTED:\n";
        for (const auto& [ptr, info] : allocations){
            std::cout << " - Leak at " << ptr
                      << " | Size: " << info.size
                      << " bytes | Location: "
                      << info.file << ":" << info.line << "\n";
        }
    }
}