#pragma once

namespace Silent::Utils
{
    std::string ToUpper(std::string str);
    std::string ToLower(std::string str);
    char*       CopyString(const char src[], uint size);

    template <typename T>
    requires std::is_arithmetic_v<T>
    int GetSign(T val)
    {
        return (val >= 0) ? 1 : -1;
    }

    template <typename TContainer, typename TElement>
    bool Contains(const TContainer& cont, const TElement& element)
    {
        auto it = std::find(cont.begin(), cont.end(), element);
        return it != cont.end();
    }

    template <typename TContainer, typename TPredicate>
    void EraseIf(TContainer& cont, TPredicate pred)
    {
        cont.erase(std::remove_if(cont.begin(), cont.end(), pred), cont.end());
    }

    template<typename T>
    std::span<const T> ToSpan(const T* rawArray, uint size)
    {
        return std::span<const T>(rawArray, size);
    }

    template<typename T>
    std::span<T> ToSpan(T* rawArray, uint size)
    {
        return std::span<T>(rawArray, size);
    }

    template <typename T, uint size>
    std::span<T> ToSpan(T(&cArray)[size])
    {
        return std::span<T>(cArray, size);
    }

    template<typename T>
    std::span<const typename T::value_type> ToSpan(const T& cont)
    {
        return std::span<const typename T::value_type>(std::begin(cont), std::end(cont));
    }

    template<typename T>
    std::span<typename T::value_type> ToSpan(T& cont)
    {
        return std::span<typename T::value_type>(std::begin(cont), std::end(cont));
    }
}
