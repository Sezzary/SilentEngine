#pragma once

namespace Silent::Utils
{
    /** @brief Double-buffered data. */
    template <typename T>
    struct DoubleBuffer
    {
        T Back  = {};
        T Front = {};

        void Swap();
        void Flush();
    };

    template <typename T>
    void DoubleBuffer<T>::Swap()
    {
        std::swap(Back, Front);
    }

    template <typename T>
    void DoubleBuffer<T>::Flush()
    {
        Back = Front;
    }
}
