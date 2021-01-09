#pragma once

namespace DXP
{

// This is helper container to hold data used for
// drawing plots for ImGui. This is similar to CyclicBuffer, but
// operates on types rather than byte streams
template <typename T, int SIZE>
struct RingBuffer
{
    RingBuffer() {
        // If we're storing numbers numeric, zero them out
        if constexpr (std::is_arithmetic_v<T>) {
            Set(0);
        }
    };

    // Getter function signature
    using getter_t = T(*)(void*, int);

    // Helper function for ImGui usage
    getter_t GetterFunction() const {
        return +[](void* ptr, int i) {
            RingBuffer<T, SIZE>& buffer = *static_cast<RingBuffer<T, SIZE> *>(ptr);
            return buffer[i];
        };
    };

    void Set(T value) {
        values.fill(value);
    };

    void Push(T value) {
        values[current] = value;
        current++;
        current %= SIZE;
    };

    T Peek() const {
        if (current == 0)
            return values[SIZE - 1];

        return values[current - 1];
    };

    int32_t Size() const {
        return SIZE;
    };

    T& operator[](int i) {
        return values[(current + i) % SIZE];
    };

    const T* Data() const {
        return values.data();
    };

private:
    std::array<T, SIZE> values;
    int current = 0;
};

};
