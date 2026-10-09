#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Screen/CutsceneBorder.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/View/Structs.h"
//#include "Game/Bodyprog/View/vc_main.h"
#include "Game/Bodyprog/View/VcUtil.h"
#include "Game/Main/FsQueue.h"

namespace Silent::Game
{
    static DR_MODE D_800A8E98[] =
    {
        { 0x3000000, 0xE1000240, 0x0 },
        { 0x3000000, 0xE1000240, 0x0 }
    };

    // TODO: Make a macro?
    static POLY_G4 D_800A8EB0[] =
    {
        {
            0x8000000,
            0x0, 0x0, 0x0, 0x3A,
            -160, -112,
            0x0, 0x0, 0x0, 0x0,
            160, -112,
            0x0, 0x0, 0x0, 0x0,
            -160, -96,
            0x0, 0x0, 0x0, 0x0,
            160, -96
        },
        {
            0x8000000,
            0x0, 0x0, 0x0, 0x3A,
            -160, -112,
            0x0, 0x0, 0x0, 0x0,
            160, -112,
            0x0, 0x0, 0x0, 0x0,
            -160, -96,
            0x0, 0x0, 0x0, 0x0,
            160, -96
        },
        {
            0x8000000,
            0x0, 0x0, 0x0, 0x3A,
            -160, 0x70,
            0x0, 0x0, 0x0, 0x0,
            160, 0x70,
            0x0, 0x0, 0x0, 0x0,
            -160, 0x60,
            0x0, 0x0, 0x0, 0x0,
            160, 96
        },
        {
            0x8000000,
            0x0, 0x0, 0x0, 0x3A,
            -160, 112,
            0x0, 0x0, 0x0, 0x0,
            160, 0x70,
            0x0, 0x0, 0x0, 0x0,
            -160, 96,
            0x0, 0x0, 0x0, 0x0,
            160, 96
        }
    };

    static q19_12 g_BlackBorderShade = Q12(0.0f);

    static void Screen_BlackBorderDraw(POLY_G4* poly, s32 color)
    {
        s32 i;
        s32 color0;
        s32 color1;

        color0 = color >> 4;
        color1 = color >> 5;

        if (color == Q12_CLAMPED(1.0f))
        {
            color1 = Q8_CLAMPED(1.0f);
        }

        for (i = 0; i < 2; i++)
        {
            poly[i * 2].r0 = color0;
            poly[i * 2].g0 = color0;
            poly[i * 2].b0 = color0;
            poly[i * 2].r1 = color0;
            poly[i * 2].g1 = color0;
            poly[i * 2].b1 = color0;
            poly[i * 2].r2 = color1;
            poly[i * 2].g2 = color1;
            poly[i * 2].b2 = color1;
            poly[i * 2].r3 = color1;
            poly[i * 2].g3 = color1;
            poly[i * 2].b3 = color1;
        }
    }

    void Screen_CutsceneCameraStateUpdate() // 0x80032904
    {
        //GsOT*    ot;
        POLY_G4* poly;
        //DR_MODE* drMode;

        //drMode = &D_800A8E98[g_ActiveBufferIdx];
        //poly   = &D_800A8EB0[g_ActiveBufferIdx];

        vcSetEvCamRate(g_BlackBorderShade);

        if (g_SysWork.bgmStatusFlags & BgmStatusFlag_Pause)
        {
            return;
        }

        switch (g_SysWork.cutsceneBorderState)
        {
            case 18:
                g_SysWork.cutsceneBorderState++;

            case 19:
                g_BlackBorderShade += Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f);
                if (g_BlackBorderShade >= Q12_CLAMPED(1.0f))
                {
                    g_BlackBorderShade = Q12_CLAMPED(1.0f);
                    g_SysWork.cutsceneBorderState++;
                }

                Screen_BlackBorderDraw(poly, g_BlackBorderShade);
                break;

            case 20:
            case 22:
                g_BlackBorderShade = Q12_CLAMPED(1.0f);
                g_SysWork.cutsceneBorderState++;

            case 21:
                Screen_BlackBorderDraw(poly, g_BlackBorderShade);
                break;

            case 23:
                g_BlackBorderShade -= Q12_MULT_FLOAT_PRECISE(g_DeltaTime, 1.0f);
                if (g_BlackBorderShade <= Q12(0.0f))
                {
                    g_BlackBorderShade            = Q12(0.0f);
                    g_SysWork.cutsceneBorderState = 0;
                    return;
                }

                Screen_BlackBorderDraw(poly, g_BlackBorderShade);
                break;

            case 0:
                g_BlackBorderShade            = Q12(0.0f);
                g_SysWork.cutsceneBorderState = 1;
                g_SysWork.sysState           &= ~SysFlag_CutsceneActive;
                return;

            case 1:
                return;
        }

        //ot = (GsOT*)&g_OtTags0[g_ActiveBufferIdx][4];
        //AddPrim(ot, poly);
        //AddPrim(ot, &poly[2]);
        //AddPrim(ot, drMode);

        if (!(g_SysWork.sysState & SysFlag_CutsceneActive))
        {
            vcChangeProjectionValue(g_GameWork.gsScreenHeight + Q12_MULT(377 - g_GameWork.gsScreenHeight, g_BlackBorderShade));
        }
    }
}
