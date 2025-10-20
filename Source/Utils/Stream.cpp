#include "Framework.h"
#include "Utils/Stream.h"

namespace Silent::Utils
{
    Stream::Stream(const std::filesystem::path& filename, bool read, bool write)
    {
        if (read)
        {
            _mode |= std::fstream::in;
        }
        if (write)
        {
            _mode |= std::fstream::out | std::fstream::trunc;
        }

        _stream.open(filename, _mode);
    }

    Stream::~Stream()
    {
        _stream.close();
    }

    bool Stream::IsOpen() const
    {
        return _stream.is_open();
    }

    bool Stream::IsEndOfFile() const
    {
        return _stream.eof();
    }

    void Stream::Close()
    {
        _stream.flush();
        _stream.close();
    }

    void Stream::Read(void* buffer, uint size)
    {
        if (!_stream.good() && !(_mode & std::fstream::in))
        {
            Log("Failed to read from binary file data stream.", LogLevel::Warning);
            return;
        }

        _stream.read((byte*)buffer, size);
    }

    byte Stream::ReadByte()
    {
        byte val = 0;
        Read((byte*)&val, sizeof(byte));
        return val;
    }

    bool Stream::ReadBool()
    {
        bool val = false;
        Read((byte*)&val, sizeof(bool));
        return val;
    }

    int16 Stream::ReadInt16()
    {
        int16 val = 0;
        Read((byte*)&val, sizeof(int16));
        return val;
    }

    int32 Stream::ReadInt32()
    {
        int32 val = 0;
        Read((byte*)&val, sizeof(int32));
        return val;
    }

    int64 Stream::ReadInt64()
    {
        int64 val = 0;
        Read((byte*)&val, sizeof(int64));
        return val;
    }

    uint16 Stream::ReadUint16()
    {
        return (uint16)ReadInt16();
    }

    uint32 Stream::ReadUint32()
    {
        return (uint32)ReadInt32();
    }

    uint64 Stream::ReadUint64()
    {
        return (uint64)ReadInt64();
    }

    float Stream::ReadFloat()
    {
        float val = 0.0f;
        Read((byte*)&val, sizeof(float));
        return val;
    }

    std::string Stream::ReadString()
    {
        int size = ReadInt32();

        auto str = std::string();
        str.resize(size);

        Read(str.data(), size);
        return str;
    }
    
    Vector2i Stream::ReadVector2i()
    {
        int x = ReadInt32();
        int y = ReadInt32();
        return Vector2i(x, y);
    }

    Vector2 Stream::ReadVector2()
    {
        float x = ReadFloat();
        float y = ReadFloat();
        return Vector2(x, y);
    }

    Vector3i Stream::ReadVector3i()
    {
        int x = ReadInt32();
        int y = ReadInt32();
        int z = ReadInt32();
        return Vector3i(x, y, z);
    }

    Vector3 Stream::ReadVector3()
    {
        float x = ReadFloat();
        float y = ReadFloat();
        float z = ReadFloat();
        return Vector3(x, y, z);
    }

    void Stream::Write(const void* buffer, uint size)
    {
        if (!_stream.good() && !(_mode & std::fstream::out))
        {
            Log("Failed to write to binary file data stream.", LogLevel::Warning);
            return;
        }

        _stream.write((const byte*)buffer, size);
    }
}
