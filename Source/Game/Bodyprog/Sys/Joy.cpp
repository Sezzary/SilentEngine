#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Sys/Joy.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Input/Input.h"

namespace Silent::Game
{
    void Joy_ControllerDataUpdate() // 0x80034494
    {
        constexpr int CONTROLLER_COUNT             = 2;
        constexpr int PULSE_INITIAL_INTERVAL_TICKS = TICKS_PER_SECOND / 2;
        constexpr int PULSE_INTERVAL_TICKS         = PULSE_INITIAL_INTERVAL_TICKS / 10;

        s32               i;
        s32               prevBtnsHeld;
        s32               pulseTicks;
        s32               btnsPulsed;
        s_ControllerData* cont;

        // Update controller button flags.
        for (i = CONTROLLER_COUNT, cont = g_Controller0; i > 0; i--, cont++)
        {
            prevBtnsHeld = cont->buttonFlags.held;

            // Update held button flags.
            if (cont->analogController.status == 0xFF)
            {
                cont->buttonFlags.held = ControllerFlag_None;
            }
            else
            {
                cont->buttonFlags.held = ~cont->analogController.digitalButtons & 0xFFFF;
            }

            // TODO: Demagic hex values.
            //ControllerData_AnalogToDigital(cont, (*(u16*)&cont->analogController.status & 0x5300) == 0x5300);

            // Directional held flag sanitation? TODO: Find out what it's doing.
            cont->buttonFlags.held = cont->buttonFlags.held | (((cont->buttonFlags.held << 20) | (cont->buttonFlags.held << 8)) &
                                                    (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighRight | ControllerFlag_LStickHighDown | ControllerFlag_LStickHighLeft));

            // Clear up/down held flags if concurrent.
            if ((cont->buttonFlags.held & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)) == (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown))
            {
                cont->buttonFlags.held &= ~(ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown);
            }

            // Clear left/right held flags if concurrent.
            if ((cont->buttonFlags.held & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft)) == (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                cont->buttonFlags.held = cont->buttonFlags.held & ~(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft);
            }

            // Update clicked and released button flags.
            cont->buttonFlags.clicked  = ~prevBtnsHeld & cont->buttonFlags.held;
            cont->buttonFlags.released =  prevBtnsHeld & ~cont->buttonFlags.held;

            // Update pulse ticks.
            pulseTicks = cont->pulseTicks;
            if (cont->buttonFlags.held != prevBtnsHeld)
            {
                pulseTicks = 0;
            }
            else
            {
                pulseTicks += g_VBlanks;
            }

            // Update pulsed button flags.
            if (pulseTicks >= PULSE_INITIAL_INTERVAL_TICKS)
            {
                cont->buttonFlags.pulsed = cont->buttonFlags.held;
                pulseTicks           = PULSE_INITIAL_INTERVAL_TICKS - PULSE_INTERVAL_TICKS;
            }
            else
            {
                cont->buttonFlags.pulsed = cont->buttonFlags.clicked;
            }

            btnsPulsed              = cont->buttonFlags.pulsed;
            cont->pulseTicks        = pulseTicks;
            cont->buttonFlags.pulsedGui = btnsPulsed;

            // Clear left/right pulse flags if concurrent.
            if ((btnsPulsed & (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft)) == (ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft))
            {
                cont->buttonFlags.pulsedGui &= ~(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft);
            }

            // Clear up/down pulse flags if concurrent.
            if ((cont->buttonFlags.pulsedGui & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)) == (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown))
            {
                cont->buttonFlags.pulsedGui &= ~(ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown);
            }

            // Clear left/right pulse flags if up/down is concurrent.
            if ((cont->buttonFlags.pulsedGui & (ControllerFlag_LStickHighUp | ControllerFlag_LStickHighDown)))
            {
                cont->buttonFlags.pulsedGui &= ~(ControllerFlag_LStickHighRight | ControllerFlag_LStickHighLeft);
            }
        }
    }

    void ControllerData_AnalogToDigital(s_ControllerData* cont, bool arg1) // 0x80034670
    {
        s32 val;
        s32 axisIdx;
        s32 processedInputFlags;
        s32 normalizedAnalogData;
        s32 xorShiftedRawAnalog;
        s32 btnsHeld;
        s32 signedRawAnalog;
        s32 negativeDirBitIdx;
        s32 positiveDirBitIdx;

        btnsHeld = cont->buttonFlags.held;

        if (arg1)
        {
            signedRawAnalog     = *(u32*)&cont->analogController.rightX ^ 0x80808080;
            xorShiftedRawAnalog = signedRawAnalog;

            for (normalizedAnalogData = 0, axisIdx = 3;
                axisIdx >= 0;
                axisIdx--)
            {
                normalizedAnalogData <<= 8;
                val                    = xorShiftedRawAnalog >> 24;
                xorShiftedRawAnalog  <<= 8;

                if (val < -STICK_DEADZONE)
                {
                    normalizedAnalogData |= (val + STICK_DEADZONE) & 0xFF;
                    negativeDirBitIdx     = 23 - (axisIdx & (1 << 0));
                    btnsHeld             |= 1 << (negativeDirBitIdx - (axisIdx * 2));
                }
                else if (val >= STICK_DEADZONE)
                {
                    normalizedAnalogData |= (val - (STICK_DEADZONE - 1)) & 0xFF;
                    positiveDirBitIdx     = (axisIdx & 0x1) + 21;
                    btnsHeld             |= 1 << (positiveDirBitIdx - ((axisIdx >> 1) * 4));
                }
            }

            cont->buttonFlags.held = btnsHeld;
        }
        else
        {
            signedRawAnalog      = 0;
            normalizedAnalogData = 0;
        }

        processedInputFlags       = normalizedAnalogData;
        cont->rawSticks.rawData_0 = signedRawAnalog;

        // TODO: Demagic hex values. Analog stick or button flags?
        if (cont == g_Controller0)
        {
            if (!(processedInputFlags & 0xFF000000))
            {
                val = btnsHeld & 0x50;
                if (val == 0x40)
                {
                    normalizedAnalogData = processedInputFlags | 0x2D000000;
                }
                else if (val == 0x10)
                {
                    normalizedAnalogData = processedInputFlags | 0xD3000000;
                }
            }
            if (!(normalizedAnalogData & 0xFF0000))
            {
                val = btnsHeld & 0xA0;
                if (val == 0x20)
                {
                    normalizedAnalogData |= 0x2D0000;
                }
                else if (val == 0x80)
                {
                    normalizedAnalogData |= 0xD30000;
                }
            }
            if (!(processedInputFlags & 0xFF000000))
            {
                val = btnsHeld & 0x50;
                if (val == 0x40)
                {
                    processedInputFlags |= 0x20000000;
                }
                else if (val == 0x10)
                {
                    if (!(btnsHeld & g_GameWorkPtr->config.controllerConfig.run))
                    {
                        processedInputFlags |= 0xE0000000;
                    }
                    else
                    {
                        processedInputFlags |= 0xC0000000;
                    }
                }
            }
            if (!(processedInputFlags & 0xFF0000))
            {
                val = btnsHeld & 0xA0;
                if (val == 0x20)
                {
                    processedInputFlags |= 0x200000;
                }
                else if (val == 0x80)
                {
                    processedInputFlags |= 0xE00000;
                }
            }
        }

        cont->normalizedSticks.rawData_0 = normalizedAnalogData;
        cont->field_28 = processedInputFlags;
    }
}
