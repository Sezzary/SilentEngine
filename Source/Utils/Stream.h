#pragma once

namespace Silent::Utils
{
    /** @brief Binary file data stream. */
    class Stream
    {
    private:
        // =======
        // Fields
        // =======

        std::ifstream _stream = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs a binary file data `Stream`.
         *
         * @param filename Full file path.
         */
        Stream(const std::filesystem::path& filename);

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the file is open.
         *
         * @return `true` if the file is open, `false` otherwise.
         */
        bool IsOpen() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Reads a byte from the data stream and increments the file pointer.
         *
         * @return `byte` data.
         */
        byte ReadByte();

        /** @brief Reads a 16-bit integer from the data stream and increments the file pointer.
         *
         * @return `int16` data.
         */
        int16 ReadInt16();

        /** @brief Reads a 32-bit integer from the data stream and increments the file pointer.
         *
         * @return `int32` data.
         */
        int32 ReadInt32();

        /** @brief Reads a 64-bit integer from the data stream and increments the file pointer.
         *
         * @return `int64` data.
         */
        int64 ReadInt64();

        /** @brief Reads a 16-bit unsigned integer from the data stream and increments the file pointer.
         *
         * @return `uint16` data.
         */
        uint16 ReadUint16();

        /** @brief Reads a 32-bit unsigned integer from the data stream and increments the file pointer.
         *
         * @return `uint32` data.
         */
        uint32 ReadUint32();

        /** @brief Reads a 64-bit unsigned integer from the data stream and increments the file pointer.
         *
         * @return `uint64` data.
         */
        uint64 ReadUint64();

        /** @brief Reads a 32-bit float from the data stream and increments the file pointer.
         *
         * @return `float` data.
         */
        float ReadFloat();

        /** @brief Reads an array from the data stream and increments the file pointer.
         *
         * @tparam T Data type.
         * @param dest Destination.
         * @param count Number of data elements.
         */
        template <typename T>
        void ReadArray(std::span<T> dest, uint count = NO_VALUE)
        {
            _stream.read((byte*)dest.data(), ((count != NO_VALUE) ? count : dest.size()) * sizeof(T));
        }
    };
}
