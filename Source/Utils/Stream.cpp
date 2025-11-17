#include "Framework.h"
#include "Utils/Stream.h"

#include "Services/Filesystem.h"

using namespace Silent::Services;

namespace Silent::Utils
{
    Stream::Stream(const std::filesystem::path& filename, bool read, bool write)
    {
        if (read)
        {
            _flags |= std::fstream::in;
        }
        if (write)
        {
            _flags |= std::fstream::out | std::fstream::trunc;
            std::filesystem::create_directories(filename.parent_path());
        }

        _stream.open(filename, _flags);
    }

    Stream::~Stream()
    {
        Close();
    }

    uint Stream::GetSize()
    {
        if (!IsOpen())
        {
            return 0;
        }

        // Store current position.
        auto curPos = _stream.tellg();

        // Get size from file end and return to current position.
        _stream.seekg(0, std::fstream::end);
        uint size = (uint)_stream.tellg();
        _stream.seekg(curPos);
        return size;
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
        if (!TestRead())
        {
            return;
        }

        _stream.read((byte*)buffer, size);
    }

    bool Stream::ReadBool()
    {
        bool val = false;
        Read((byte*)&val, sizeof(bool));
        return val;
    }

    byte Stream::ReadByte()
    {
        byte val = 0;
        Read((byte*)&val, sizeof(byte));
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

    json Stream::ReadJson()
    {
        if (!TestRead())
        {
            return json();
        }

        try
        {
            auto val = json::parse(_stream);
            _stream.seekg(0, std::fstream::end);
            return val;
        }
        catch (const json::parse_error& ex)
        {
            Debug::Log("Failed to read JSON from binary file data stream.", Debug::LogLevel::Warning);
            return json();
        }
    }

    void Stream::Write(const void* buffer, uint size)
    {
        if (!_stream.good() && !(_flags & std::fstream::out))
        {
            Debug::Log("Failed to write to binary file data stream.", Debug::LogLevel::Warning);
            return;
        }

        _stream.write((const byte*)buffer, size);
    }

    void Stream::WriteBool(bool val)
    {
        Write((byte*)&val, sizeof(bool));
    }

    void Stream::WriteByte(byte val)
    {
        Write((byte*)&val, sizeof(byte));
    }

    void Stream::WriteInt16(int16 val)
    {
        Write((byte*)&val, sizeof(int16));
    }

    void Stream::WriteInt32(int32 val)
    {
        Write((byte*)&val, sizeof(int32));
    }

    void Stream::WriteInt64(int64 val)
    {
        Write((byte*)&val, sizeof(int64));
    }

    void Stream::WriteUint16(uint16 val)
    {
        Write((byte*)&val, sizeof(uint16));
    }

    void Stream::WriteUint32(uint32 val)
    {
        Write((byte*)&val, sizeof(uint32));
    }

    void Stream::WriteUint64(uint64 val)
    {
        Write((byte*)&val, sizeof(uint64));
    }

    void Stream::WriteFloat(float val)
    {
        Write((byte*)&val, sizeof(float));
    }

    void Stream::WriteString(const std::string& val)
    {
        Write((byte*)val.data(), val.size());
    }

    void Stream::WriteVector2i(const Vector2i& val)
    {
        Write((byte*)&val, sizeof(Vector2i));
    }
    
    void Stream::WriteVector2(const Vector2& val)
    {
        Write((byte*)&val, sizeof(Vector2));
    }
    
    void Stream::WriteVector3i(const Vector3i& val)
    {
        Write((byte*)&val, sizeof(Vector3i));
    }
    
    void Stream::WriteVector3(const Vector3& val)
    {
        Write((byte*)&val, sizeof(Vector3));
    }

    void Stream::WriteJson(const json& val)
    {
        if (!TestWrite())
        {
            return;
        }

        _stream << val.dump(JSON_INDENT_SIZE);
        _stream.seekg(0, std::fstream::end);
    }

    bool Stream::TestRead() const
    {
        if (!_stream.good() && !(_flags & std::fstream::in))
        {
            Debug::Log("Failed to read from binary file data stream.", Debug::LogLevel::Warning);
            return false;
        }

        return true;
    }

    bool Stream::TestWrite() const
    {
        if (!_stream.good() && !(_flags & std::fstream::out))
        {
            Debug::Log("Failed to write binary file data stream.", Debug::LogLevel::Warning);
            return false;
        }

        return true;
    }
}
