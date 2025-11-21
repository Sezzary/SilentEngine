#pragma once

namespace Silent::Utils{ struct FontMetadata; }

using namespace Silent::Utils;

namespace Silent::Assets
{
    constexpr char ASSETS_FONTS_DIR_NAME[] = "Fonts";
    constexpr char GLYPH_PRECACHE[]        = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
                                             "ÁÀÂĄÄÆĆÇÉÈÊËĘÍÌÎÏŁŃÑÓÔÖØŒŚÚÙÛÜŸŹŻ"
                                             "áàâąäæćçéèêëęíìîïłńñóôöøœßśúùûüÿźż"
                                             "“”¿¡★"
                                             "あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをん"
                                             "アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲン"
                                             "０１２３４５６７８９"
                                             "ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ"
                                             "ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ"
                                             "。！？、";

    extern const std::vector<FontMetadata> FONTS_METADATA;
}
