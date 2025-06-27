#pragma once

#include "Engine/Services/Compatibility/PsyCross/PsxLib/libgpu.h"
#include "Engine/Services/Compatibility/PsyCross/PsxLib/libgte.h"

//#include "psx/types.h"

//#include "PsyX/PsyX_config.h"
//#include "PsyX/common/pgxp_defs.h"

extern OT_TAG prim_terminator; // `P_TAG` with zero length.

extern int     g_splitIndex;
extern DISPENV currentDispEnv;
extern DISPENV activeDispEnv;
extern DRAWENV activeDrawEnv;
extern int     g_GPUDisabledState;

void ClearSplits();
void DrawAllSplits();

void ParsePrimitivesLinkedList(uint* p, int singlePrimitive);
