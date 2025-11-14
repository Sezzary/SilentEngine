#pragma once

namespace Silent::Assets
{
    constexpr char ASSETS_FONTS_DIR_NAME[] = "Fonts";
    constexpr char GLYPH_PRECACHE[]        = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\
                                              ÁáÀàÂâĄąÄäÆæĆćÇçÉéÈèÊêËëĘęÍíÌìÎîÏïŁłŃńÑñŒœÓóÔôÖöØøßŚśÚúÙùÛûÜüŸÿŹźŻż\
                                              “”¿¡★\
                                              あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん\
                                              アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン\
                                              。！？、";

    struct FontMetadata
    {
        std::string              Name      = {};
        std::vector<std::string> Filenames = {};
        int                      PointSize = 0;
    };

    extern const std::vector<FontMetadata> FONTS_METADATA;
}
