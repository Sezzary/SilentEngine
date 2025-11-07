#include "Framework.h"
#include "Assets/Fonts.h"

namespace Silent::Assets
{
    constexpr auto POINT_SIZE_HIGH_RES = 48;
    constexpr auto POINT_SIZE_LOW_RES  = 16;
    constexpr char COMMON_PRECACHE[]   = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\
                                          ÁáÀàÂâĄąÄäÆæĆćÇçÉéÈèÊêËëĘęÍíÌìÎîÏïŁłŃńÑñŒœÓóÔôÖöØøßŚśÚúÙùÛûÜüŸÿŹźŻż\
                                          ¿¡\
                                          ";
    constexpr char JAPANESE_PRECACHE[] = "あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん\
                                          アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン\
                                          。！？、 ";

    const std::vector<FontMetadata> FONTS_METADATA
    {
        FontMetadata
        {
            .Name      = "FreeSerif.otf",
            .PointSize = POINT_SIZE_HIGH_RES,
            .Precache  = COMMON_PRECACHE
        },
        FontMetadata
        {
            .Name      = "ShipporiMincho.ttf",
            .PointSize = POINT_SIZE_HIGH_RES,
            .Precache  = JAPANESE_PRECACHE
        },
        FontMetadata
        {
            .Name      = "UnifontExMono.ttf",
            .PointSize = POINT_SIZE_LOW_RES,
            .Precache  = COMMON_PRECACHE
        }
    };
}
