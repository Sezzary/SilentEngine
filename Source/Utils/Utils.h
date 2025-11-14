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

    /** @brief Finds a read-only value associated with a given pair in a hash map.
     *
     * @tparam TKey Key type.
     * @tparam TValue Value type.
     * @param cont Container to search.
     * @param key Key to find.
     * @return Read-only alue at the given key.
     */
    template <typename TKey, typename TValue>
    const TValue* Find(const std::unordered_map<TKey, TValue>& cont, const TKey& key)
    {
        auto it = cont.find(key);
        return (it != cont.end()) ? &it->second : nullptr;
    }

    /** @brief Finds a writable value associated with a given pair in a hash map.
     *
     * @tparam TKey Key type.
     * @tparam TValue Value type.
     * @param cont Container to search.
     * @param key Key to find.
     * @return Writable value at the given key.
     */
    template <typename TKey, typename TValue>
    TValue* Find(std::unordered_map<TKey, TValue>& cont, const TKey& key)
    {
        auto it = cont.find(key);
        return (it != cont.end()) ? &it->second : nullptr;
    }

    /** @brief Checks if a container has a matching element.
     *
     * @tparam TContainer Container type.
     * @tparam TElement Element type.
     * @param cont Container to check.
     * @param element Element to check for.
     * @return `true` if the container has the element, `false` otherwise.
     */
    template <typename TContainer, typename TElement>
    bool Contains(const TContainer& cont, const TElement& element)
    {
        return std::find(cont.begin(), cont.end(), element) != cont.end();
    }

    /** @brief Fills all elements in a container with a specified value.
     *
     * @param cont Container to fill.
     * @param val Fill value.
     */
    template <typename TContainer, typename TValue>
    void Fill(TContainer& cont, const TValue& val)
    {
        std::fill(cont.begin(), cont.end(), val);
    }

    /** @brief Erases elements from a container based on a predicate.
     *
     * @tparam TContainer Container type.
     * @tparam TPreciate Predicate type.
     * @param cont Container to erase elements from.
     * @param pred Predicate defining the basis for element erasure.
     */
    template <typename TContainer, typename TPredicate>
    void Erase(TContainer& cont, TPredicate pred)
    {
        cont.erase(std::remove_if(cont.begin(), cont.end(), pred), cont.end());
    }

    /** @brief Sorts elements in a container.
     *
     * @tparam T Container type.
     * @param cont Container to sort.
     */
    template <typename T>
    void Sort(T& cont)
    {
        std::sort(cont.begin(), cont.end());
    }

    /** @brief Sorts elements in a container based on a predicate.
     *
     * @tparam TContainer Container type.
     * @tparam TPreciate Predicate type.
     * @param cont Container to sort.
     * @param pred Predicate defining the basis for sorting.
     */
    template <typename TContainer, typename TPredicate>
    void Sort(TContainer& cont, const TPredicate& pred)
    {
        std::sort(cont.begin(), cont.end(), pred);
    }

    /** @brief Reverses elements in a container.
     *
     * @tparam T Container type.
     * @param cont Container to reverse.
     */
    template <typename T>
    void Reverse(T& cont)
    {
        std::reverse(cont.begin(), cont.end());
    }

    /** @brief Converts a raw array to a read-only span.
     *
     * @tparam T Element type.
     * @param rawArray Raw array.
     * @param size Array size.
     * @return Read-only `std::span` representing the raw array.
     */
    template<typename T>
    std::span<const T> ToSpan(const T* rawArray, uint size)
    {
        return std::span<const T>(rawArray, size);
    }

    /** @brief Converts a raw array to a writable span.
     *
     * @tparam T Element type.
     * @param rawArray Raw array.
     * @param size Array size.
     * @return Writable `std::span` representing the raw array.
     */
    template<typename T>
    std::span<T> ToSpan(T* rawArray, uint size)
    {
        return std::span<T>(rawArray, size);
    }

    /** @brief Converts a C array to a span.
     *
     * @tparam T Element type.
     * @param cArray C array.
     * @param size Array size.
     * @return `std::span` representing the C array.
     */
    template <typename T, uint size>
    std::span<T> ToSpan(T(&cArray)[size])
    {
        return std::span<T>(cArray, size);
    }

    /** @brief Converts a container to a read-only span.
     *
     * @tparam T Container type.
     * @param cont Container.
     * @return Read-only `std::span` representing the container.
     */
    template<typename T>
    std::span<const typename T::value_type> ToSpan(const T& cont)
    {
        return std::span<const typename T::value_type>(std::begin(cont), std::end(cont));
    }

    /** @brief Converts a container to a writable span.
     *
     * @tparam T Container type.
     * @param cont Container.
     * @return Writable `std::span` representing the container.
     */
    template<typename T>
    std::span<typename T::value_type> ToSpan(T& cont)
    {
        return std::span<typename T::value_type>(std::begin(cont), std::end(cont));
    }
}
