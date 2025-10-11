#pragma once

namespace Silent::Utils
{
    /** @brief Converts a string to upper case.
     *
     * @param str String to convert
     * @return String in upper case.
     */
    std::string ToUpper(std::string str);

    /** @brief Converts a string to lower case.
     *
     * @param str String to convert
     * @return String in lower case.
     */
    std::string ToLower(std::string str);

    /** @brief Creates a heap-allocated string copy.
     *
     * @param src String source.
     * @param size String size.
     * @return String copy.
     */
    char* CopyString(const char src[], uint size);

    /** @brief Converts a screen position in percent to normalized device coordinates.
     *
     * @param pos Screen position in percent with components in the range `[0.0f, 100.0f]`.
     * @return Normalized device coordinates with components in the range `[-1.0f, 1.0f]`.
     */
    Vector2 ConvertPercentToNdc(const Vector2& pos);

    /** @brief Converts normalized device coordinates to a screen position in percent.
     *
     * @param ndc Normalized device coordinates with components in the range `[-1.0f, 1.0f]`.
     * @return Screen position in percent with components in the range `[0.0f, 100.0f]`.
     */
    Vector2 ConvertNdcToPercent(const Vector2& ndc);

    /** @brief Gets the sign of a value.
     *
     * @tparam T Numeric type.
     * @param val Value.
     * @return `1` if the value is >=0, `-1` otherwise.
     */
    template <typename T>
    requires std::is_arithmetic_v<T>
    int GetSign(T val)
    {
        return (val >= 0) ? 1 : -1;
    }

    /** @brief Checks if a container contains a given element
     *
     * @tparam TContainer Container type.
     * @tparam TElement Element type.
     * @param cont Container to check.
     * @param element Element to check for.
     * @return `true` if the container contains the element, `false` otherwise.
     */
    template <typename TContainer, typename TElement>
    bool Contains(const TContainer& cont, const TElement& element)
    {
        auto it = std::find(cont.begin(), cont.end(), element);
        return it != cont.end();
    }

    /** @brief Erases elements from a container based on a predicate.
     *
     * @tparam TContainer Container type.
     * @tparam TPreciate Predicate type.
     * @param cont Container to erase elements from.
     * @param pred Predicate defining the basis for element erasure.
     */
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
