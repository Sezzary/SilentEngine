#pragma once

//#include "PsyX/PsyX_config.h"

#define RENDERER_OGL
#define USE_GLAD
#define USE_OPENGL 1

#define GL_GLEXT_PROTOTYPES
#include "glad/glad.h"

#define TEXTURE_FORMAT GL_UNSIGNED_SHORT_1_5_5_5_REV

//#include "psx/types.h"
//#include "common/pgxp_defs.h"

#include "Engine/Services/Compatibility/PsyCross/PsxLib/libgpu.h"
#include "Engine/Services/Compatibility/PsyCross/PsxLib/libgte.h"

#include <stdio.h>
#include <stddef.h>

#ifndef NULL
    #define NULL 0
#endif

#define VRAM_FORMAT          GL_RG
#define VRAM_INTERNAL_FORMAT GL_RG32F

#define VRAM_WIDTH	1024
#define VRAM_HEIGHT	512

#define TPAGE_WIDTH  256
#define TPAGE_HEIGHT 256

#define MAX_VERTEX_BUFFER_SIZE (1 << (sizeof(ushort) * 8))

#pragma pack(push, 1)
struct GrVertex
{
#if USE_PGXP
    float x, y, page, clut;
    float z, scr_h, ofsX, ofsY;
#else
    short x, y, page, clut;
#endif

    uchar u, v, bright, dither;
    uchar r, g, b, a;

    char tcx, tcy, _p0, _p1;
};
#pragma pack(pop)

enum ShaderAttrib
{
    a_position,
    a_zw,
    a_texcoord,
    a_color,
    a_extra,
};

enum BlendMode
{
    BM_NONE,
    BM_AVERAGE,
    BM_ADD,
    BM_SUBTRACT,
    BM_ADD_QUATER_SOURCE
};

enum TexFormat
{
    TF_4_BIT,
    TF_8_BIT,
    TF_16_BIT,

    TF_32_BIT_RGBA // Custom texture.
};

typedef uint TextureID;
typedef uint ShaderID;

extern TextureID g_whiteTexture;
extern TextureID g_vramTexture;

void GR_SwapWindow();

// PSX VRAM operations
void GR_SaveVRAM(const char* outputFileName, int x, int y, int width, int height, int bReadFromFrameBuffer);
void GR_CopyVRAM(unsigned short* src, int x, int y, int w, int h, int dst_x, int dst_y);
void GR_ReadVRAM(unsigned short* dst, int x, int y, int dst_w, int dst_h);

void GR_StoreFrameBuffer(int x, int y, int w, int h);
void GR_UpdateVRAM();
void GR_ReadFramebufferDataToVRAM();

TextureID GR_CreateRGBATexture(int width, int height, uchar* data /*= nullptr*/);
ShaderID  GR_Shader_Compile(const char* source);

void GR_SetShader(const ShaderID shader);
void GR_Perspective3D(const float fov, const float width, const float height, const float zNear, const float zFar);
void GR_Ortho2D(float left, float right, float bottom, float top, float znear, float zfar);

void GR_SetBlendMode(BlendMode blendMode);
void GR_SetPolygonOffset(float ofs);
void GR_SetStencilMode(int drawPrim);
void GR_EnableDepth(int enable);
void GR_SetScissorState(int enable);
void GR_SetOffscreenState(const RECT16* offscreenRect, int enable);
void GR_SetupClipMode(const RECT16* clipRect, int enable);
void GR_SetViewPort(int x, int y, int width, int height);
void GR_SetTexture(TextureID texture, TexFormat texFormat);
void GR_SetOverrideTextureSize(int width, int height);
void GR_SetWireframe(int enable);

void GR_DestroyTexture(TextureID texture);
void GR_Clear(int x, int y, int w, int h, uchar r, uchar g, uchar b);
void GR_ClearVRAM(int x, int y, int w, int h, uchar r, uchar g, uchar b);
void GR_UpdateVertexBuffer(const GrVertex* vertices, int count);
void GR_DrawTriangles(int start_vertex, int triangles);

void GR_PushDebugLabel(const char* label);
void GR_PopDebugLabel();
