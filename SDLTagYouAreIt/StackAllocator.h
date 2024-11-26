#ifndef STACKALLOCATOR_H
#define STACKALLOCATOR_H

#include <cstddef>
#include <cassert>

class StackAllocator {
public:
    explicit StackAllocator(size_t size) {
        memory = new char[size];
        maxSize = size;
        offset = 0;
    }

    ~StackAllocator() {
        delete[] memory;
    }

    void* allocate(size_t size) {
        assert(offset + size <= maxSize && "StackAllocator: Out of memory!");
        void* ptr = memory + offset;
        offset += size;
        return ptr;
    }

    void clear() {
        offset = 0; // Reset the allocator for reuse
    }

private:
    char* memory;
    size_t maxSize;
    size_t offset;
};

#endif // STACKALLOCATOR_H

