#include "Framework.h"
#include "Utils/Stream.h"

#include "Services/Filesystem.h"
#include "Utils/Bitfield.h"
#include "Utils/Utils.h"

using namespace Silent::Services;

namespace Silent::Utils
{
    Stream::Stream(const stdfs::path& filename, bool read, bool write)
    {
        if (read)
        {
            _flags |= (int)std::fstream::in;
        }

        if (write)
        {
            _flags |= (int)std::fstream::out | (int)std::fstream::trunc;
            stdfs::create_directories(filename.parent_path());
        }

        _stream.open(filename, (std::ios_base::openmode)_flags);
    }

    Stream::~Stream()
    {
        Close();
    }

    int Stream::GetSize()
    {
        if (!IsOpen())
        {
            return 0;
        }

        // Store current position.
        auto curPos = _stream.tellg();

        // Get size from file end and return to current position.
        _stream.seekg(0, std::fstream::end);
        int size = (int)_stream.tellg();
        _stream.seekg(curPos);
        return size;
    }

    int Stream::GetPosition()
    {
        if (!IsOpen())
        {
            return NO_VALUE;
        }

        return (int)_stream.tellg();
    }

    void Stream::SetPosition(int pos)
    {
        if (!IsOpen())
        {
            return;
        }

        _stream.seekg(pos, std::fstream::beg);
        if (_stream.fail())
        {
            Debug::Log(Fmt("Failed to set position of binary file data stream to {}.", pos), Debug::LogLevel::Error);
        }
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

    void Stream::Skip(int size)
    {
        _stream.ignore(size);
    }

    void Stream::Read(void* buffer, int size)
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

    int8 Stream::ReadInt8()
    {
        int8 val = 0;
        Read((int8*)&val, sizeof(int8));
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

    uint8 Stream::ReadUint8()
    {
        return (uint8)ReadInt8();
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

    std::string Stream::ReadCString(int size)
    {
        constexpr int BUFFER_SIZE = 32;

        auto str = std::string();
        str.reserve(BUFFER_SIZE);

        int startPos = GetPosition();

        // Reach `char`s.
        int limit = (size != NO_VALUE) ? size : (GetSize() - startPos);
        for (int i = 0; i < limit; i++)
        {
            char c = ReadByte();
            if (c == '\0') 
            {
                break;
            }

            str += c;
        }

        // If size is specified, set stream position to end of region.
        if (size != NO_VALUE)
        {
            SetPosition(startPos + size);
        }

        return str;
    }

    Bitfield Stream::ReadBitfield()
    {
        int  size   = ReadInt32();
        auto chunks = std::vector<Bitfield::ChunkType>((size + (Bitfield::CHUNK_SIZE - 1)) / Bitfield::CHUNK_SIZE);
        ReadArray(ToSpan(chunks));
        return Bitfield(size, chunks);
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

    bool Stream::Write(const void* buffer, int size)
    {
        if (!_stream.good() && !(_flags & std::fstream::out))
        {
            Debug::Log("Failed to write to binary file data stream.", Debug::LogLevel::Warning);
            return false;
        }

        _stream.write((const byte*)buffer, size);
        return !_stream.fail();
    }

    bool Stream::WriteBool(bool val)
    {
        return Write((byte*)&val, sizeof(bool));
    }

    bool Stream::WriteByte(byte val)
    {
        return Write((byte*)&val, sizeof(byte));
    }

    bool Stream::WriteInt16(int16 val)
    {
        return Write((byte*)&val, sizeof(int16));
    }

    bool Stream::WriteInt32(int32 val)
    {
        return Write((byte*)&val, sizeof(int32));
    }

    bool Stream::WriteInt64(int64 val)
    {
        return Write((byte*)&val, sizeof(int64));
    }

    bool Stream::WriteUint16(uint16 val)
    {
        return Write((byte*)&val, sizeof(uint16));
    }

    bool Stream::WriteUint32(uint32 val)
    {
        return Write((byte*)&val, sizeof(uint32));
    }

    bool Stream::WriteUint64(uint64 val)
    {
        return Write((byte*)&val, sizeof(uint64));
    }

    bool Stream::WriteFloat(float val)
    {
        return Write((byte*)&val, sizeof(float));
    }

    bool Stream::WriteString(const std::string& val)
    {
        return Write((byte*)val.data(), val.size());
    }

    bool Stream::WriteBitfield(const Bitfield& val)
    {
        if (!WriteInt32(val.GetSize()))
        {
            return false;
        }

        if (!WriteArray(ToSpan(val.GetChunks())))
        {
            return false;
        }

        return true;
    }

    bool Stream::WriteVector2i(const Vector2i& val)
    {
        return Write((byte*)&val, sizeof(Vector2i));
    }

    bool Stream::WriteVector2(const Vector2& val)
    {
        return Write((byte*)&val, sizeof(Vector2));
    }

    bool Stream::WriteVector3i(const Vector3i& val)
    {
        return Write((byte*)&val, sizeof(Vector3i));
    }

    bool Stream::WriteVector3(const Vector3& val)
    {
        return Write((byte*)&val, sizeof(Vector3));
    }

    bool Stream::WriteJson(const json& val)
    {
        if (!TestWrite())
        {
            return false;
        }

        _stream << val.dump(JSON_INDENT_SIZE);
        _stream.seekg(0, std::fstream::end);
        return !_stream.fail();
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
