#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Screen/BackgroundDraw.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Main/FsQueue.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Renderer/Renderer.h"
#include "Utils/Video.h"

using namespace Silent::Renderer;

namespace Silent::Game
{
    q0_8 g_Screen_BackgroundImgGamma = Q8(0.5f);

    void Screen_BackgroundImgDraw(const std::string& assetName, bool fit, int paletteIdx, const Color& bgColor)
    {
        auto& renderer = g_App.GetRenderer();

        // Submit fullscreen background shape.
        if (bgColor != Color::Clear)
        {
            auto shape = Shape2d::CreateQuad(Vector2::Zero,
                                             Vector2(SCREEN_SPACE_RES.x, 0.0f),
                                             Vector2(SCREEN_SPACE_RES.x, SCREEN_SPACE_RES.y),
                                             Vector2(0.0f, SCREEN_SPACE_RES.y),
                                             bgColor, bgColor, bgColor, bgColor,
                                             DEPTH_2D_MAX, ScaleMode::Fill, BlendMode::Opaque);
            renderer.SubmitShape2d(shape);
        }

        // Define scale mode.
        auto scaleMode = fit ? ScaleMode::Fit : ScaleMode::Fill;

        // Submit fullscreen background sprite.
        auto sprite = Sprite2d::CreateSprite2d(assetName, Vector2::Zero, Vector2::One,
                                               SCREEN_SPACE_RES * 0.5f, DEG_TO_RAD(0.0f), 1.0f, Color::White, paletteIdx,
                                               DEPTH_2D_MAX - 1, AlignMode::Center, scaleMode, BlendMode::Opaque);
        renderer.SubmitSprite2d(sprite);

        // Submit gamma overlay sprite.
        float gamma = Q8_TO_FLT(g_Screen_BackgroundImgGamma - Q8(0.5f));
        if (gamma != 0.0f)
        {
            auto gammaColor  = Color(1.0f, 1.0f, 1.0f, gamma);
            auto gammaSprite = Sprite2d::CreateSprite2d(assetName, Vector2::Zero, Vector2::One,
                                                        SCREEN_SPACE_RES * 0.5f, DEG_TO_RAD(0.0f), 1.0f, gammaColor, paletteIdx,
                                                        DEPTH_2D_MAX - 2, AlignMode::Center, scaleMode, BlendMode::Add);
            renderer.SubmitSprite2d(gammaSprite);
        }

        g_SysWork.bgmStatusFlags   |= BgmStatusFlag_Pause;
        g_Screen_BackgroundImgGamma = Q8(0.5f);
    }

    void Screen_BackgroundImgDraw(s_FsImageDesc* image)
    {
        Screen_BackgroundImgDraw(image->AssetName, true);

        g_SysWork.bgmStatusFlags   |= BgmStatusFlag_Pause;
        g_Screen_BackgroundImgGamma = Q8(0.5f);
    }

    void Screen_BackgroundImgDrawAlt(s_FsImageDesc* image)
    {
        Screen_BackgroundImgDraw(image);
    }

    void Screen_BackgroundImgTransition(const std::string& assetName0, const std::string& assetName1, q3_12 alpha,
                                        bool fit)
    {
        auto& renderer = g_App.GetRenderer();

        // Define scale mode.
        auto scaleMode = fit ? ScaleMode::Fit : ScaleMode::Fill;

        // Submit first blended fullscreen background sprite.
        auto color0  = Color::White;
        auto sprite0 = Sprite2d::CreateSprite2d(assetName0, Vector2::Zero, Vector2::One,
                                                SCREEN_SPACE_RES * 0.5f, DEG_TO_RAD(0.0f), 1.0f, Color::White, 0,
                                                DEPTH_2D_MAX, AlignMode::Center, scaleMode, BlendMode::Opaque);
        renderer.SubmitSprite2d(sprite0);

        // Submit second blended fullscreen backround sprite.
        auto color1  = Color(1.0f, 1.0f, 1.0f, Q12_TO_FLT(alpha));
        auto sprite1 = Sprite2d::CreateSprite2d(assetName0, Vector2::Zero, Vector2::One,
                                                SCREEN_SPACE_RES * 0.5f, DEG_TO_RAD(0.0f), 1.0f, color1, 0,
                                                DEPTH_2D_MAX - 1, AlignMode::Center, scaleMode, BlendMode::Alpha);
        renderer.SubmitSprite2d(sprite1);

        g_SysWork.bgmStatusFlags |= BgmStatusFlag_Pause;
    }

    bool Screen_BackgroundMotionBlur(s32 vBlanks) // 0x80031CCC
    {
        // @stub

        /*s32       interlacingEnabled;
        s32       i;
        s32       offsetY;
        s32       texOffsetY;
        s32       tPageOffsetY;
        s32       j;
        GsOT*     ot;
        SPRT*     sprt;
        DR_TPAGE* tPage;

        ot                 = (GsOT*)&g_OtTags1[g_ActiveBufferIdx + 1][0];
        sprt               = (SPRT*)GsOUT_PACKET_P;
        interlacingEnabled = GsDISPENV.isinter;

        for (i = 0; (i == 0 || (interlacingEnabled && i == 1)); i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (interlacingEnabled)
                {
                    texOffsetY   = (-(i == 0)) & 0x20;
                    offsetY      = (i == 0) ? -224 : 0;
                    tPageOffsetY = i << 8;
                }
                else
                {
                    offsetY      = -112;
                    texOffsetY   = (g_ActiveBufferIdx == 0) << 5;
                    tPageOffsetY = g_ActiveBufferIdx << 8;
                }

                addPrimFast(ot, sprt, 4);

                if ((VSync(SyncMode_Count) % vBlanks) == 0)
                {
                    setRGBC0(sprt, 127, 127, 127, PRIM_RECT | RECT_TEXTURE);
                }
                else
                {
                    setRGBC0(sprt, 128, 128, 128, PRIM_RECT | RECT_TEXTURE);
                }

                setWH(sprt, 256, 224);
                *((u32*)&sprt->u0) = texOffsetY << 8;

                setXY0Fast(sprt, (-g_GameWork.gsScreenWidth / 2) + (j << 8), offsetY);

                sprt++;
                tPage = (DR_TPAGE*)sprt;

                setDrawTPage(tPage, 0, 1, getTPage(2, 0, (j << 8), tPageOffsetY));
                AddPrim(ot, tPage);

                sprt = (PACKET*)sprt + sizeof(DR_TPAGE);
            }
        }

        GsOUT_PACKET_P = sprt;*/
        return false;
    }
}
