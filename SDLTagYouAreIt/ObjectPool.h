#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
#include <cassert>

template <typename T>
class ObjectPool {
public:
    explicit ObjectPool(size_t size) {
        objects.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            objects.push_back(new T());
            available.push_back(objects.back());
        }
    }

    ~ObjectPool() {
        for (auto obj : objects) {
            delete obj;
        }
    }

    T* acquire() {
        if (available.empty()) {
            return nullptr; // Pool exhausted
        }
        T* obj = available.back();
        available.pop_back();
        return obj;
    }

    void release(T* obj) {
        assert(obj != nullptr);
        available.push_back(obj);
    }

    void reset() {
        available = objects;
    }

private:
    std::vector<T*> objects;
    std::vector<T*> available;
};

#endif // OBJECTPOOL_H
