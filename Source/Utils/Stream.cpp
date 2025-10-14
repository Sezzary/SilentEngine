#include "Framework.h"
#include "Utils/Stream.h"

namespace Silent::Utils
{
    Stream::Stream(const std::filesystem::path&  filename)
    {
        _stream = std::ifstream(filename, std::ios::binary);
    }

    bool Stream::IsOpen() const
    {
        return _stream.is_open();
    }
    
    byte Stream::ReadByte()
    {
        byte val = 0;
        _stream.read((byte*)&val, sizeof(byte));
        return val;
    }

    int16 Stream::ReadInt16()
    {
        int16 val = 0;
        _stream.read((byte*)&val, sizeof(int16));
        return val;
    }

    int32 Stream::ReadInt32()
    {
        int32 val = 0;
        _stream.read((byte*)&val, sizeof(int32));
        return val;
    }

    int64 Stream::ReadInt64()
    {
        int64 val = 0;
        _stream.read((byte*)&val, sizeof(int64));
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
        _stream.read((byte*)&val, sizeof(float));
        return val;
    }

    void Stream::ReadData(void* dest, uint dataSize, uint count)
    {
        _stream.read((byte*)dest, count * dataSize);
    }
}
