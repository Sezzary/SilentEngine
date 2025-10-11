#include "Framework.h"
#include "Engine/Assets/Parsers/Tim.h"

namespace Silent::Assets
{
    /** @brief Bits per pixel types. */
    enum class BitsPerPixel
    {
        Bpp4,
        Bpp8,
        Bpp16
    };

    enum class TimFlags
    {
        Bpp4    = 0,
        Bpp8    = 1 << 0,
        Bpp16   = 1 << 1,
        HasClut = 1 << 3
    };

    std::shared_ptr<void> ParseTim(const std::filesystem::path& filename)
    {
        constexpr int HEADER_MAGIC  = 0x10;
        constexpr int BPP_MASK      = 0x7;

        // Read file.
        auto file = std::ifstream(filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Couldn't open TIM `" + filename.string() + "`.");
        }

        // Confirm TIM format magic.
        uint32 magic = 0;
        file.read((char*)&magic, 4);
        if (magic != HEADER_MAGIC)
        {
            throw std::runtime_error("Invalid TIM `" + filename.string() + "`.");
        }

        // Read CLUT and BPP flags.
        uint32 flags = 0;
        file.read((char*)&flags, 4);

        // Read CLUT.
        auto clut = std::vector<uint16>{};
        if (flags & (int)TimFlags::HasClut)
        {
            // Read size.
            uint32 clutSize = 0;
            file.read((char*)&clutSize, 4);

            // Read frame ebuffer coordinates (unused).
            uint16 clutX = 0;
            uint16 clutY = 0;
            file.read((char*)&clutX, 2);
            file.read((char*)&clutY, 2);

            // Read dimensions.
            uint16 clutW = 0;
            uint16 clutH = 0;
            file.read((char*)&clutW, 2);
            file.read((char*)&clutH, 2);

            // Read color values.
            uint clutCount = clutW * clutH;
            clut.resize(clutCount);
            file.read((char*)clut.data(), clutCount * 2);
        }

        // Read image data header (unused).
        uint32 imageSize = 0;
        file.read((char*)&imageSize, 4);

        // Read frame buffer coordinates (unused).
        uint16 imageX = 0;
        uint16 imageY = 0;
        file.read((char*)&imageX, 2);
        file.read((char*)&imageY, 2);

        // Read image dimensions.
        uint16 imageW = 0;
        uint16 imageH = 0;
        file.read((char*)&imageW, 2);
        file.read((char*)&imageH, 2);

        // Define BPP.
        auto bpp = BitsPerPixel::Bpp4;
        switch ((TimFlags)(flags & BPP_MASK))
        {
            default:
            case TimFlags::Bpp4:
            {
                bpp = BitsPerPixel::Bpp4;
                break;
            }
            case TimFlags::Bpp8:
            {
                bpp = BitsPerPixel::Bpp8;
                break;
            }
            case TimFlags::Bpp16:
            {
                bpp = BitsPerPixel::Bpp16;
                break;
            }
        }

        // Definte color depth.
        uint colorDepth = 1 << (flags & BPP_MASK);

        // Define image width coefficient based on BPP.
        int  widthCoeff = 1;
        switch (bpp)
        {
            default:
            case BitsPerPixel::Bpp4:
            {
                widthCoeff = 4;
                break;
            }
            case BitsPerPixel::Bpp8:
            {
                widthCoeff = 2;
                break;
            }
            case BitsPerPixel::Bpp16:
            {
                widthCoeff = 1;
                break;
            }
        }

        // Define image resolution.
        auto res = Vector2i(imageW * widthCoeff, imageH);

        // Create asset.
        auto asset = TimAsset
        {
            .ColorDepth = colorDepth,
            .Resolution = res,
            .Pixels     = std::vector<uint8>((res.x * res.y) * 4)
        };

        auto setPixelColor = [&](int x, int y, uint16 color)
        {
            // Collect extracted RGBA components.
            uchar* out = &asset.Pixels[((y * res.x) + x) * 4];
            out[0]     = (color & 0x1F)         << 3; // B.
            out[1]     = ((color >> 5) & 0x1F)  << 3; // G.
            out[2]     = ((color >> 10) & 0x1F) << 3; // R.
            out[3]     = (color & 0x8000) ? 0 : 255;  // A.

            // Key out black as transparent.
            if (out[0] == 0 && out[1] == 0 && out[2] == 0)
            {
                out[3] = 0;
            }
        };

        // Read pixels.
        for (int y = 0; y < res.y; y++)
        {
            for (int x = 0; x < res.x;)
            {
                switch (bpp)
                {
                    default:
                    case BitsPerPixel::Bpp4:
                    {
                        // Read color data.
                        uint8 byte = 0;
                        file.read((char*)&byte, 1);

                        // Decode values from byte.
                        uint val0 = byte & 0xF;
                        uint val1 = byte >> 4;

                        for (int i = 0; i < 2 && x < res.x; i++, x++)
                        {
                            // Set pixel.
                            if (clut.empty())
                            {
                                // Grayscale color [0, 15].
                                uint16 color = ((i == 0) ? val0 : val1) * (0xFFFF / 0xF);
                                setPixelColor(x, y, color);
                            }
                            else
                            {
                                // CLUT color.
                                uint16 color = clut[(i == 0) ? val0 : val1];
                                setPixelColor(x, y, color);
                            }
                        }
                        break;
                    }
                    case BitsPerPixel::Bpp8:
                    {
                        // Read color data.
                        uint idx = 0;
                        file.read((char*)&idx, 1);
                        
                        // Set pixel.
                        if (clut.empty())
                        {
                            // Grayscale color [0, 255].
                            uint16 color = idx * (0xFFFF / 0xFF);
                            setPixelColor(x, y, 0xFFFF);
                        }
                        else
                        {
                            // CLUT color.
                            uint16 color = clut[idx];
                            setPixelColor(x, y, color);
                        }

                        x++;
                        break;
                    }
                    case BitsPerPixel::Bpp16:
                    {
                        // Read color.
                        uint16 color = 0;
                        file.read((char*)&color, 2);

                        // Set pixel.
                        setPixelColor(x, y, color);

                        x++;
                        break;
                    }
                }
            }
        }

        return std::make_shared<TimAsset>(std::move(asset));
    }
}
