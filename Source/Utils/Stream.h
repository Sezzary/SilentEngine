#pragma once

namespace Silent::Utils
{
    // @todo Writer methods.

    /** @brief Binary file data stream. */
    class Stream
    {
    private:
        // =======
        // Fields
        // =======

        std::fstream _stream = {};
        int          _mode   = 0;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs a binary file data `Stream` which can be read from or written to.
         *
         * @param filename Full file path.
         * @param read Allow reading from the file.
         * @param write Allow writing to the file.
         */
        Stream(const std::filesystem::path& filename, bool read, bool write);

        /** @brief Gracefully destroys the `Stream` and closes the file. */
        ~Stream();

        // ==========
        // Inquirers
        // ==========

        /** @brief Checks if the file is open.
         *
         * @return `true` if the file is open, `false` otherwise.
         */
        bool IsOpen() const;

        /** @brief Checks if the end of the file has been reached.
         *
         * @return `true` if the end of the file has been reached, `false` otherwise.
         */
        bool IsEndOfFile() const;

        // ==========
        // Utilities
        // ==========

        /** @brief Closes the file. */
        void Close();

        /** @brief Reads buffer data from the data stream and increments the file pointer.
         *
         * @param[out] buffer Output buffer.
         * @param size Buffer size in bytes.
         */
        void Read(void* buffer, uint size);

        /** @brief Reads a byte from the data stream and increments the file pointer.
         *
         * @return `byte` data.
         */
        byte ReadByte();

        /** @brief Reads a boolean from the data stream and increments the file pointer.
         *
         * @return `bool` data.
         */
        bool ReadBool();

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

        /** @brief Reads a string from the data stream and increments the file pointer.
         *
         * @return `std::string` data.
         */
        std::string ReadString();

        /** @brief Reads an integer-based XYZ vector from the data stream and increments the file pointer.
         *
         * @return `Vector2i` data.
         */
        Vector2i ReadVector2i();

        /** @brief Reads a float-based XYZ vector from the data stream and increments the file pointer.
         *
         * @return `Vector2` data.
         */
        Vector2 ReadVector2();

        /** @brief Reads an integer-based XYZ vector from the data stream and increments the file pointer.
         *
         * @return `Vector3i` data.
         */
        Vector3i ReadVector3i();

        /** @brief Reads a float-based XYZ vector from the data stream and increments the file pointer.
         *
         * @return `Vector3` data.
         */
        Vector3 ReadVector3();

        /** @brief Reads an array from the data stream and increments the file pointer.
         *
         * @tparam T Array data type.
         * @param[out] dest Destination array.
         * @param size Destination size. If `NO_VALUE`, the size of `dest` will be used.
         */
        template <typename T>
        void ReadData(std::span<T> dest, uint size = NO_VALUE)
        {
            Read((byte*)dest.data(), ((size != NO_VALUE) ? size : dest.size()) * sizeof(T));
        }

        /** @brief Writes buffer data to the data stream and increments the file pointer.
         *
         * @param buffer Input buffer.
         * @param size Buffer size in bytes.
         */
        void Write(const void* buffer, uint size);
    };
}
