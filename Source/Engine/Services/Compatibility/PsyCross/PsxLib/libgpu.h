#pragma once

#include "Engine/Services/Compatibility/PsyCross/PsxLib/libgte.h"

typedef short VERTTYPE;

int (*GPU_printf)(const char* fmt, ...);

// Uses 32-bit pointers on `P_TAG` and other primitives.
// When turned off, also disables PGXP.
#ifndef USE_EXTENDED_PRIM_POINTERS
    #define USE_EXTENDED_PRIM_POINTERS 1
#endif

// PGXP-Z: Precise GTE transform pipeline with Z-depth and widescreen fixes. Recommended for game mode.
#ifndef USE_PGXP
    #define USE_PGXP USE_EXTENDED_PRIM_POINTERS
#endif

#define WAIT_TIME    0x800000
#define FNT_MAX_ID   8    // Max font stream IDs.
#define FNT_MAX_SPRT 1024 // Max font sprites in all streams.

#define limitRange(x, l, h) \
    ((x) = (((x) < (l)) ? (l) : (((x) > (h)) ? (h) : (x))))

#define setVector(v, _x, _y, _z) \
    (v)->vx = _x,                \
    (v)->vy = _y,                \
    (v)->vz = _z	

#define applyVector(v, _x, _y, _z, op) \
    (v)->vx op _x,                     \
    (v)->vy op _y,                     \
    (v)->vz op _z	

#define copyVector(v0, v1) \
    (v0)->vx = (v1)->vx,   \
    (v0)->vy = (v1)->vy,   \
    (v0)->vz = (v1)->vz 

#define addVector(v0, v1) \
    (v0)->vx += (v1)->vx, \
    (v0)->vy += (v1)->vy, \
    (v0)->vz += (v1)->vz	
    
#define dumpVector(str, v) \
    GPU_printf("%s=(%d,%d,%d)\n", str, (v)->vx, (v)->vy, (v)->vz)

#define dumpMatrix(x)                                                        \
    GPU_printf("\t%5d,%5d,%5d\n", (x)->m[0][0], (x)->m[0][1], (x)->m[0][2]), \
    GPU_printf("\t%5d,%5d,%5d\n", (x)->m[1][0], (x)->m[1][1], (x)->m[1][2]), \
    GPU_printf("\t%5d,%5d,%5d\n", (x)->m[2][0], (x)->m[2][1], (x)->m[2][2])

#define setRECT(r, _x, _y, _w, _h) \
    (r)->x = (_x),                 \
    (r)->y = (_y),                 \
    (r)->w = (_w),                 \
    (r)->h = (_h)

// ------------------------
// Set primitive attributes
// ------------------------

#define setTPage(p, tp, abr, x, y) \
    ((p)->tpage = getTPage(tp, abr, x, y))

#define setClut(p, x, y) \
    ((p)->clut = getClut(x, y))

// --------------------
// Set primitive colors
// --------------------

#define setRGB0(p, _r0, _g0, _b0) \
    (p)->r0 = _r0,                \
    (p)->g0 = _g0,                \
    (p)->b0 = _b0

#define setRGB1(p,_r1,_g1,_b1) \
    (p)->r1 = _r1,             \
    (p)->g1 = _g1,             \
    (p)->b1 = _b1

#define setRGB2(p,_r2,_g2,_b2) \
    (p)->r2 = _r2,             \
    (p)->g2 = _g2,             \
    (p)->b2 = _b2
    
#define setRGB3(p,_r3,_g3,_b3) \
    (p)->r3 = _r3,             \
    (p)->g3 = _g3,             \
    (p)->b3 = _b3

#define setRGBP0(p,_r0,_g0,_b0,_p0) \
    (p)->r0 = _r0,                  \
    (p)->g0 = _g0,                  \
    (p)->b0 = _b0,                  \
    (p)->p0 = _p0

#define setRGBP1(p,_r1,_g1,_b1,_p1) \
    (p)->r1 = _r1,                  \
    (p)->g1 = _g1,                  \
    (p)->b1 = _b1,                  \
    (p)->p1 = _p1

#define setRGBP2(p,_r2,_g2,_b2,_p2) \
    (p)->r2 = _r2,                  \
    (p)->g2 = _g2,                  \
    (p)->b2 = _b2,                  \
    (p)->p2 = _p2

#define setRGBP3(p,_r3,_g3,_b3,_p3) \
    (p)->r3 = _r3,                  \
    (p)->g3 = _g3,                  \
    (p)->b3 = _b3,                  \
    (p)->p3 = _p3

// ---------------------------
// Set primitive screen points
// ---------------------------

#define setXY0(p, _x0, _y0) \
    (p)->x0 = (_x0), (p)->y0 = (_y0)

#define setXY2(p, _x0, _y0, _x1, _y1) \
    (p)->x0 = (_x0), (p)->y0 = (_y0), \
    (p)->x1 = (_x1), (p)->y1 = (_y1)

#define setXY3(p, _x0, _y0, _x1, _y1, _x2, _y2) \
    (p)->x0 = (_x0), (p)->y0 = (_y0),           \
    (p)->x1 = (_x1), (p)->y1 = (_y1),           \
    (p)->x2 = (_x2), (p)->y2 = (_y2)

#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3) \
    (p)->x0 = (_x0), (p)->y0 = (_y0),                     \
    (p)->x1 = (_x1), (p)->y1 = (_y1),                     \
    (p)->x2 = (_x2), (p)->y2 = (_y2),                     \
    (p)->x3 = (_x3), (p)->y3 = (_y3)

#define setXYWH(p, _x0, _y0, _w, _h)                \
    (p)->x0 = (_x0),        (p)->y0 = (_y0),        \
    (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0),        \
    (p)->x2 = (_x0),        (p)->y2 = (_y0) + (_h), \
    (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)

// --------------------------
// Set primitive width/height
// --------------------------

#define setWH(p, _w, _h) \
    (p)->w = _w,         \
    (p)->h = _h

// ----------------------------
// Set primitive texture points
// ----------------------------

#define setUV0(p, _u0, _v0) \
    (p)->u0 = (_u0), (p)->v0 = (_v0)

#define setUV3(p, _u0, _v0, _u1, _v1, _u2, _v2) \
    (p)->u0 = (_u0), (p)->v0 = (_v0),           \
    (p)->u1 = (_u1), (p)->v1 = (_v1),           \
    (p)->u2 = (_u2), (p)->v2 = (_v2)

#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) \
    (p)->u0 = (_u0), (p)->v0 = (_v0),                     \
    (p)->u1 = (_u1), (p)->v1 = (_v1),                     \
    (p)->u2 = (_u2), (p)->v2 = (_v2),                     \
    (p)->u3 = (_u3), (p)->v3 = (_v3)

#define setUVWH(p, _u0, _v0, _w, _h)              \
    (p)->u0 = (_u0),      (p)->v0 = (_v0),        \
    (p)->u1 = (_u0)+(_w), (p)->v1 = (_v0),        \
    (p)->u2 = (_u0),      (p)->v2 = (_v0) + (_h), \
    (p)->u3 = (_u0)+(_w), (p)->v3 = (_v0) + (_h)

// ------------------------
// Dump primivie parameters
// ------------------------

#define dumpRECT16(r) \
    GPU_printf("(%d,%d)-(%d,%d)\n", (r)->x, (r)->y, (r)->w, (r)->h)

#define dumpWH(p) \
    GPU_printf("(%d,%d)\n", (p)->w, (p)->h) 

#define dumpXY0(p) \
    GPU_printf("(%d,%d)\n", (p)->x0, (p)->y0) 

#define dumpUV0(p) \
    GPU_printf("(%d,%d)\n", (p)->u0, (p)->v0) 

#define dumpXY2(p)                  \
    GPU_printf("(%d,%d)-(%d,%d)\n", \
               (p)->x0, (p)->y0, (p)->x1, (p)->y1)

#define dumpXY3(p)                          \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)\n", \
               (p)->x0, (p)->y0,            \
               (p)->x1, (p)->y1,            \
               (p)->x2, (p)->y2)

#define dumpUV3(p)                          \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)\n", \
               (p)->u0, (p)->v0,            \
               (p)->u1, (p)->v1,            \
               (p)->u2, (p)->v2)

#define dumpXY4(p)                                  \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)-(%d,%d)\n", \
               (p)->x0, (p)->y0,                    \
               (p)->x1, (p)->y1,                    \
               (p)->x2, (p)->y2,                    \
               (p)->x3, (p)->y3)

#define dumpUV4(p)                                  \
    GPU_printf("(%d,%d)-(%d,%d)-(%d,%d)-(%d,%d)\n", \
               (p)->u0, (p)->v0,                    \
               (p)->u1, (p)->v1,                    \
               (p)->u2, (p)->v2,                    \
               (p)->u3, (p)->v3)

#define dumpRGB0(p) \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r0, (p)->g0, (p)->b0) 	
        
#define dumpRGB1(p) \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r1, (p)->g1, (p)->b1)	

#define dumpRGB2(p) \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r2, (p)->g2, (p)->b2) 

#define dumpRGB3(p) \
    GPU_printf("(%3d,%3d,%3d)\n", (p)->r3, (p)->g3, (p)->b3) 	

// ------------------
// Primitive handling
// ------------------

#if USE_EXTENDED_PRIM_POINTERS
    #define isendprim(p) \
        ((((P_TAG*)(p))->addr) == ((uintptr_t) & prim_terminator))

    #define nextPrim(p) \
        (void*)(((P_TAG*)(p))->addr)

    #define setaddr(p, _addr) \
        (((P_TAG*)(p))->addr = (uintptr_t)((uint*)_addr))

    #define getaddr(p) \
        (uintptr_t)(((P_TAG*)(p))->addr)
#else
    #define isendprim(p) \
        ((((P_TAG*)(p))->addr) == 0xFFFFFF)

    #define nextPrim(p) \
        (void*)((((P_TAG*)(p))->addr))

    #define setaddr(p, _addr)
        (((P_TAG*)(p))->addr = (uint)((uint*)_addr))

    #define getaddr(p) \
        (uint)(((P_TAG*)(p))->addr)
#endif

#define setlen(p, _len) \
    (((P_TAG*)(p))->len = (uchar)(_len))

#define setcode(p, _code) \
    (((P_TAG*)(p))->code = (uchar)(_code))

#define getlen(p) \
    (uchar)(((P_TAG*)(p))->len)

#define getcode(p) \
    (uchar)(((P_TAG*)(p))->code)

#if USE_PGXP && USE_EXTENDED_PRIM_POINTERS
    #define setpgxpindex(p, i) \
        (((P_TAG *)(p))->pgxp_index = (ushort)(i))

    #define addPrim(ot, p)       \
        setaddr(p, getaddr(ot)), \
        setaddr(ot, p),          \
        setpgxpindex(p, PGXP_GetIndex(1))
#else
    #define addPrim(ot, p)       \
        setaddr(p, getaddr(ot)), \
        setaddr(ot, p)
#endif

#define addPrims(ot, p0, p1)  \
    setaddr(p1, getaddr(ot)), \
    setaddr(ot, p0)

#define catPrim(p0, p1) \
    setaddr(p0, p1)

#if USE_EXTENDED_PRIM_POINTERS
    #define termPrim(p) \
        setaddr(p, &prim_terminator)
#else
    #define termPrim(p) \
        setaddr(p, 0xFFFFFFFF)
#endif

#define setSemiTrans(p, abe) \
    ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))

#define setShadeTex(p, tge) \
    ((tge) ? setcode(p, getcode(p) | 0x01) : setcode(p, getcode(p) & ~0x01))

#define getTPage(tp, abr, x, y) \
    ((((tp)  & 0x3)   << 7) |   \
     (((abr) & 0x3)   << 5) |   \
     (((y)   & 0x100) >> 4) |   \
     (((x)   & 0x3FF) >> 6) |   \
     (((y)   & 0x200) << 2))

#define getClut(x, y) \
    (((y) << 6) | (((x) >> 4) & 0x3F))

#define dumpTPage(tpage)                                \
    GPU_printf("tpage: (%d,%d,%d,%d)\n",                \
               ((tpage) >> 7)  & 0x3, ((tpage) >> 5) & 0x3, \
               ((tpage) << 6)  & 0x3C0,                    \
               (((tpage) << 4) & 0x100) + (((tpage) >> 2) & 0x200))

#define dumpClut(clut) \
    GPU_printf("clut: (%d,%d)\n", (clut & 0x3F) << 4, (clut >> 6))

#define _get_mode(dfe, dtd, tpage) \
    (0xE1000000           |        \
     ((dtd) ? 0x0200 : 0) |        \
     ((dfe) ? 0x0400 : 0) |        \
     ((tpage) & 0x9FF))

#define setDrawTPage(p, dfe, dtd, tpage) \
    setlen(p, 1),                        \
    ((p)->code[0] = _get_mode(dfe, dtd, tpage))

#define _get_tw(tw)	                                \
    (tw ? ((0xE2000000)                           | \
           ((((tw)->y        & 0xFF) >> 3) << 15) | \
           ((((tw)->x        & 0xFF) >> 3) << 10) | \
           (((~((tw)->h - 1) & 0xFF) >> 3) << 5)  | \
           (((~((tw)->w - 1) & 0xFF) >> 3))) :      \
          0)

#define setTexWindow(p, tw)       \
    setlen(p, 2),                 \
    ((p)->code[0] = _get_tw(tw)), \
    ((p)->code[1] = 0)

#define _get_len(rect) \
    ((((RECT16)->w * (rect)->h + 1) / 2) + 4)

#define setDrawLoad(pt, rect)                                    \
    (_get_len(RECT16) <= 16) ?                                   \
        ((setlen(pt, _get_len(rect))),                           \
         ((pt)->code[0]               = 0xA0000000),             \
         ((pt)->code[1]               = *((uint*) & (rect)->x)), \
         ((pt)->code[2]               = *((uint*) & (rect)->w)), \
         ((pt)->p[_get_len(rect) - 4] = 0x01000000)) :           \
        (setlen(pt, 0))

#define setDrawStp(p, pbw)                            \
    setlen(p, 2),                                     \
    ((p)->code[0] = 0xE6000000 | ((pbw) ? 0x01 : 0)), \
    ((p)->code[1] = 0)

#define setDrawMode(p, dfe, dtd, tpage, tw)      \
    setlen(p, 3),                                \
    ((p)->code[0] = _get_mode(dfe, dtd, tpage)), \
    ((p)->code[1] = _get_tw((RECT16*)tw))

// -----------------------
// Primitive length macros
// -----------------------

#define setPolyF3(p) \
    setlen(p, 4),    \
    setcode(p, 0x20)

#define setPolyFT3(p) \
    setlen(p, 7),     \
    setcode(p, 0x24)

#define setPolyG3(p) \
    setlen(p, 6),    \
    setcode(p, 0x30)

#define setPolyGT3(p) \
    setlen(p, 9),     \
    setcode(p, 0x34)

#define setPolyF4(p) \
    setlen(p, 5),    \
    setcode(p, 0x28)

#define setPolyFT4(p) \
    setlen(p, 9),     \
    setcode(p, 0x2C)

#define setPolyG4(p) \
    setlen(p, 8),    \
    setcode(p, 0x38)

#define setPolyGT4(p) \
    setlen(p, 12),    \
    setcode(p, 0x3C)

#define setSprt8(p) \
    setlen(p, 3),   \
    setcode(p, 0x74)

#define setSprt16(p) \
    setlen(p, 3),    \
    setcode(p, 0x7C)

#define setSprt(p) \
    setlen(p, 4),  \
    setcode(p, 0x64)

#define setTile1(p)	\
    setlen(p, 2),   \
    setcode(p, 0x68)

#define setTile8(p)	\
    setlen(p, 2),   \
    setcode(p, 0x70)

#define setTile16(p) \
    setlen(p, 2),    \
    setcode(p, 0x78)

#define setTile(p) \
    setlen(p, 3),  \
    setcode(p, 0x60)

#define setLineF2(p) \
    setlen(p, 3),    \
    setcode(p, 0x40)

#define setLineG2(p) \
    setlen(p, 4),    \
    setcode(p, 0x50)

#define setLineF3(p)  \
    setlen(p, 5),     \
    setcode(p, 0x48), \
    (p)->pad = 0x55555555

#define setLineG3(p)       \
    setlen(p, 7),          \
    setcode(p, 0x58),      \
    (p)->pad = 0x55555555, \
    (p)->p2  = 0

#define setLineF4(p)  \
    setlen(p, 6),     \
    setcode(p, 0x4C), \
    (p)->pad = 0x55555555

#define setLineG4(p)       \
    setlen(p, 9),          \
    setcode(p, 0x5C),      \
    (p)->pad = 0x55555555, \
    (p)->p2  = 0,          \
    (p)->p3  = 0

#pragma pack(push, 1)

// `RECT16` rectangle.
struct RECT16
{
    short x, y; // Offset point on VRAM.
    short w, h; // Width and height.
};

// PsyCross custom struct to handle polygons.

#if USE_EXTENDED_PRIM_POINTERS
    #if defined(_M_X64) || defined(__amd64__)
        #define DECLARE_P_ADDR         \
            uintptr_t addr;            \
            uint      len        : 16; \
            uint      pgxp_index : 16;

        #define P_LEN 3 // 3 longs.
    #else
        #define DECLARE_P_ADDR         \
            uintptr_t addr;            \
            uint      len        : 16; \
            uint      pgxp_index : 16;

        #define P_LEN 2	// 2 longs.
    #endif

    #define DECLARE_P_ADDR_PTAG DECLARE_P_ADDR
#else // Don't use, just for reference.
    #define DECLARE_P_ADDR_PTAG \
        unsigned addr : 24;     \
        unsigned len  : 8;

    #define DECLARE_P_ADDR \
        uint tag;

    #define P_LEN 1 // 1 long.
#endif

// -----------------------------
// Polygon Primitive Definitions
// -----------------------------

struct OT_TAG
{
    DECLARE_P_ADDR_PTAG
};

struct P_TAG
{
    DECLARE_P_ADDR_PTAG
    uchar pad0, pad1, pad2, code;
};

// Flat triangle.
struct POLY_F3
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    VERTTYPE x1, y1;
    VERTTYPE x2, y2;
};
static_assert(((sizeof(POLY_F3) / 4) - P_LEN) == 4, "`POLY_F3` size must be 4 longs.");

// Flat quad.
struct POLY_F4
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    VERTTYPE x1, y1;
    VERTTYPE x2, y2;
    VERTTYPE x3, y3;
};
static_assert(((sizeof(POLY_F4) / 4) - P_LEN) == 5, "`POLY_F4` size must be 5 longs.");

// Flat textured triangle.
struct POLY_FT3
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; ushort clut;
    VERTTYPE x1, y1;
    uchar    u1, v1; ushort tpage;
    VERTTYPE x2, y2;
    uchar    u2, v2; ushort pad1;
};
static_assert(((sizeof(POLY_FT3) / 4) - P_LEN) == 7, "`POLY_FT3` size must be 7 longs.");

// Flat textured quad.
struct POLY_FT4
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; short clut;
    VERTTYPE x1, y1;
    uchar    u1, v1; ushort tpage;
    VERTTYPE x2, y2;
    uchar    u2, v2; ushort pad1;
    VERTTYPE x3, y3;
    uchar    u3, v3; ushort pad2;
};
static_assert(((sizeof(POLY_FT4) / 4) - P_LEN) == 9, "`POLY_FT4` size must be 9 longs.");

// Gouraud triangle.
struct POLY_G3
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    r1, g1, b1, pad1;
    VERTTYPE x1, y1;
    uchar    r2, g2, b2, pad2;
    VERTTYPE x2, y2;
};
static_assert(((sizeof(POLY_G3) / 4) - P_LEN) == 6, "`POLY_G3` size must be 6 longs.");

// Gouraud quad.
struct POLY_G4
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    r1, g1, b1, pad1;
    VERTTYPE x1, y1;
    uchar    r2, g2, b2, pad2;
    VERTTYPE x2, y2;
    uchar    r3, g3, b3, pad3;
    VERTTYPE x3, y3;
};
static_assert(((sizeof(POLY_G4) / 4) - P_LEN) == 8, "`POLY_G4` size must be 8 longs.");

// Gouraud textured triangle.
struct POLY_GT3
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; ushort clut;
    uchar    r1, g1, b1, p1;
    VERTTYPE x1, y1;
    uchar    u1, v1; ushort tpage;
    uchar    r2, g2, b2, p2;
    VERTTYPE x2, y2;
    uchar    u2, v2; ushort pad2;
};
static_assert(((sizeof(POLY_GT3) / 4) - P_LEN ) == 9, "`POLY_GT3` size must be 9 longs.");

// Gouraud textured quad.
struct POLY_GT4
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; ushort clut;
    uchar    r1, g1, b1, p1;
    VERTTYPE x1, y1;
    uchar    u1, v1; ushort tpage;
    uchar    r2, g2, b2, p2;
    VERTTYPE x2, y2;
    uchar    u2, v2; ushort pad2;
    uchar    r3, g3, b3, p3; // 10
    VERTTYPE x3, y3;         // 11
    uchar    u3, v3; ushort pad3;
};
static_assert(((sizeof(POLY_GT4) / 4) - P_LEN) == 12, "`POLY_GT4` size must be 12 longs.");

// ---------------
// Line primitives
// ---------------

// Unconnected flat line.
struct LINE_F2
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    VERTTYPE x1, y1;
};
static_assert(((sizeof(LINE_F2) / 4) - P_LEN) == 3, "`LINE_F2` size must be 3 longs.");

// Unconnected gouraud line.
struct LINE_G2
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    r1, g1, b1, p1;
    VERTTYPE x1, y1;
};
static_assert(((sizeof(LINE_G2) / 4) - P_LEN) == 4, "`LINE_G2` size must be 4 longs.");

// 2 connected flat lines.
struct LINE_F3
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    VERTTYPE x1, y1;
    VERTTYPE x2, y2;
    uint     pad;
};
static_assert(((sizeof(LINE_F3) / 4) - P_LEN) == 5, "`LINE_F3` size must be 5 longs.");

// 2 connected gouraud lines.
struct LINE_G3
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    r1, g1, b1, p1;
    VERTTYPE x1, y1;
    uchar    r2, g2, b2, p2;
    VERTTYPE x2, y2;
    uint     pad;
};
static_assert(((sizeof(LINE_G3) / 4) - P_LEN) == 7, "`LINE_G3` size must be 7 longs.");

// 3 connected flat line quads.
struct LINE_F4
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    VERTTYPE x1, y1;
    VERTTYPE x2, y2;
    VERTTYPE x3, y3;
    uint     pad;
};
static_assert(((sizeof(LINE_F4) / 4) - P_LEN) == 6, "`LINE_F4` size must be 6 longs.");

// 3 connected gouraud lines.
struct LINE_G4
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    r1, g1, b1, p1;
    VERTTYPE x1, y1;
    uchar    r2, g2, b2, p2;
    VERTTYPE x2, y2;
    uchar    r3, g3, b3, p3;
    VERTTYPE x3, y3;
    uint     pad;
};
static_assert(((sizeof(LINE_G4) / 4) - P_LEN) == 9, "`LINE_G4` size must be 9 longs.");

// -----------------
// Sprite primitives
// -----------------

// Free-size sprite.
struct SPRT
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; ushort clut;
    VERTTYPE w, h;
};
static_assert(((sizeof(SPRT) / 4) - P_LEN) == 4, "`SPRT size` must be 4 longs.");

// 16x16 sprite.
struct SPRT_16
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; ushort clut;
};
static_assert(((sizeof(SPRT_16) / 4) - P_LEN) == 3, "`SPRT_16` size must be 3 longs.");

// 8x8 sprite.
struct SPRT_8
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    uchar    u0, v0; ushort clut;
};
static_assert(((sizeof(SPRT_8) / 4) - P_LEN) == 3, "`SPRT_8` size must be 3 longs.");
        
// ---------------
// Tile primitives
// ---------------

// Free-size tile.
struct TILE
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
    VERTTYPE w, h;
};
static_assert(((sizeof(TILE) / 4) - P_LEN) == 3, "`TILE` size must be 3 longs.");

// 16x16 tile.
struct TILE_16
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
};
static_assert(((sizeof(TILE_16) / 4) - P_LEN) == 2, "`TILE_16` size must be 2 longs.");

// 8x8 tile.
struct TILE_8
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
};
static_assert(((sizeof(TILE_8) / 4) - P_LEN) == 2, "`TILE_8` size must be 2 longs.");

// 1x1 tile.
struct TILE_1
{
    DECLARE_P_ADDR
    uchar    r0, g0, b0, code;
    VERTTYPE x0, y0;
};
static_assert(((sizeof(TILE_1) / 4) - P_LEN) == 2, "`TILE_1` size must be 2 longs.");

// ------------------
// Special primitives
// ------------------

// Draw mode.
struct DR_MODE
{
    DECLARE_P_ADDR
    uint code[2];
};
static_assert(((sizeof(DR_MODE) / 4) - P_LEN) == 2, "`DR_MODE` size must be 2 longs.");

// Texture window.
struct DR_TWIN
{
    DECLARE_P_ADDR
    uint code[2];
};
static_assert(((sizeof(DR_TWIN) / 4) - P_LEN) == 2, "`DR_TWIN` size must be 2 longs.");

// Draw area.
struct DR_AREA
{
    DECLARE_P_ADDR
    uint code[2];
};
static_assert(((sizeof(DR_AREA) / 4) - P_LEN) == 2, "`DR_AREA` size must be 2 longs.");

// Draw offset.
struct DR_OFFSET
{
    DECLARE_P_ADDR
    uint code[2];
};
static_assert(((sizeof(DR_OFFSET) / 4) - P_LEN) == 2, "`DR_OFFSET` size must be 2 longs.");

// Move image.
struct DR_MOVE
{
    DECLARE_P_ADDR
    uint code[5];
};
static_assert(((sizeof(DR_MOVE) / 4) - P_LEN) == 5, "`DR_MOVE` size must be 5 longs.");

// Load image.
struct DR_LOAD
{
    DECLARE_P_ADDR
    uint code[3];
    uint p[13];
};
static_assert(((sizeof(DR_LOAD) / 4) - P_LEN) == 16, "`DR_LOAD` size must be 16 longs.");

// Draw TPage.
typedef	struct DR_TPAGE
{
    DECLARE_P_ADDR
    uint code[1];
};
static_assert(((sizeof(DR_TPAGE) / 4) - P_LEN) == 1, "`DR_TPAGE` size must be 1 long.");

// Draw STP.
struct DR_STP
{
    DECLARE_P_ADDR
    uint code[2];
};
static_assert(((sizeof(DR_STP) / 4) - P_LEN) == 2, "`DR_STP` size must be 2 longs.");

// -----------------
// PsyCross commands
// -----------------

struct DR_PSYX_TEX
{
    DECLARE_P_ADDR
    uint code[2];
};
static_assert(((sizeof(DR_PSYX_TEX) / 4) - P_LEN) == 2, "`DR_PSYX_TEX` size must be 2 longs.");

struct DR_PSYX_DBGMARKER
{
    DECLARE_P_ADDR
    uint        code;
    const char* text;
};

// -----------
// Environment
// -----------

// Packed drawing environment.
struct DR_ENV
{
    DECLARE_P_ADDR
    uint	code[15];
};
static_assert(((sizeof(DR_ENV) / 4) - P_LEN) == 15, "`DR_ENV` size must be 15 longs.");

struct DRAWENV
{
    RECT16 clip;       // Clip area.
    short  ofs[2];     // Drawing offset.
    RECT16 tw;         // Texture window.
    ushort tpage;      // Texture page.
    uchar  dtd;        // Dither flag. Off: 0, on: 1.
    uchar  dfe;        // Flag to draw on display area. Off: 0, on: 1.
    uchar  drt;
    uchar  isbg;       // Enable to auto-clear.
    uchar  r0, g0, b0; // Initital background color.
    DR_ENV dr_env;     // Reserved.
};

struct DISPENV
{
    RECT16 disp;       // Display area.
    RECT16 screen;     // Display start point.
    uchar  isinter;    // Interlace. Off: 0, on: 1.
    uchar  isrgb24;    // RGB24 bit mode.
    uchar  pad0, pad1; // Reserved.
};

// Multi-purpose TMD primitive.
struct TMD_PRIM
{
    uint   id;
    uchar  r0, g0, b0, p0; // Vertex color 0.
    uchar  r1, g1, b1, p1; // Vertex color 1.
    uchar  r2, g2, b2, p2; // Vertex color 2.
    uchar  r3, g3, b3, p3; // Vertex color 3.
    ushort tpage, clut;    // Texture page ID, clut ID.
    uchar  u0, v0, u1, v1; // Texture corner point.
    uchar  u2, v2, u3, v3;

    // Independent vertex model.

    SVECTOR	x0, x1, x2, x3; // 3D corner point.
    SVECTOR	n0, n1, n2, n3; // 3D corner normal vector.
    
    // Common vertex model.

    SVECTOR* v_ofs; // Vertex data base offset.
    SVECTOR* n_ofs; // Normal data base offset.

    ushort vert0, vert1; // Vertex index.
    ushort vert2, vert3;		
    ushort norm0, norm1; // Normal index.
    ushort norm2, norm3;
};

/// Multi-purpose TIM image.
struct TIM_IMAGE
{
    uint    mode;    // Pixel mode.
    RECT16* cRECT16; // CLUT `RECT16` on frame buffer.
    uint*   caddr;   // CLUT address on main memory.
    RECT16* pRECT16; // Texture image `RECT16` on frame buffer.
    uint*   paddr;   // Texture image address on main memory.
};

#pragma pack(pop)

#if USE_EXTENDED_PRIM_POINTERS
    OT_TAG prim_terminator;
#endif

#ifdef LoadImage
    #undef LoadImage
#endif
    
// ----------
// Prototypes
// ----------

#ifndef _FNTPRINT_
    #define _FNTPRINT_

    int FntPrint(char* fmt, ...);
#endif

#ifndef _KANJIFNTPRINT_
    #define _KANJIFNTPRINT_

    int KanjiFntPrint(char* text, ...);
#endif

DISPENV* GetDispEnv(DISPENV* env);
DISPENV* PutDispEnv(DISPENV* env);
DISPENV* SetDefDispEnv(DISPENV* env, int x, int y, int w, int h);
DRAWENV* GetDrawEnv(DRAWENV* env);
DRAWENV* PutDrawEnv(DRAWENV* env);
DRAWENV* SetDefDrawEnv(DRAWENV* env, int x, int y, int w, int h);

TIM_IMAGE* ReadTIM(TIM_IMAGE* timimg);
TMD_PRIM*  ReadTMD(TMD_PRIM* tmdprim);

int    CheckPrim(char* s, ulong* p);
int    ClearImage(RECT16* RECT16, uchar r, uchar g, uchar b);
int    ClearImage2(RECT16* RECT16, uchar r, uchar g, uchar b);
int    DrawSync(int mode);
int    FntOpen(int x, int y, int w, int h, int isbg, int n);
int    GetGraphDebug();
int    GetTimSize(uchar* sjis);
int    IsEndPrim(void* p);
int    KanjiFntOpen(int x, int y, int w, int h, int dx, int dy, int cx, int cy, int isbg, int n);
void   KanjiFntClose();
int    Krom2Tim(uchar* sjis, ulong* taddr, int dx, int dy, int cdx, int cdy, uint fg, uint bg);
int    LoadImage(RECT16* rect, ulong* p);
int    MargePrim(void* p0, void* p1);
int    MoveImage(RECT16* rect, int x, int y);
int    OpenTIM(ulong* addr);
int    OpenTMD(ulong* tmd, int obj_no);
int    ResetGraph(int mode);
int    SetGraphDebug(int level);
int    StoreImage(RECT16* rect, ulong* p);
ulong* ClearOTag(ulong* ot, int n);
ulong* ClearOTagR(ulong* ot, int n);
ulong* FntFlush();
ulong* KanjiFntFlush(int id);

uint DrawSyncCallback(void (*func)());

ushort GetClut(int x, int y);
ushort GetTPage(int tp, int abr, int x, int y);
ushort LoadClut(ulong* clut, int x, int y);
ushort LoadClut2(ulong* clut, int x, int y);
ushort LoadTPage(ulong* pix, int tp, int abr, int x, int y, int w, int h);

void* NextPrim(void* p);
void  AddPrim(void* ot, void* p);
void  AddPrims(void* ot, void* p0, void* p1);
void  CatPrim(void* p0, void* p1);

void DrawOTag(ulong* p);
void DrawOTagIO(ulong* p);
void DrawOTagEnv(ulong* p, DRAWENV* env);
void DrawPrim(void* p);
void DumpClut(ushort clut);
void DumpDispEnv(DISPENV* env);
void DumpDrawEnv(DRAWENV* env);
void DumpOTag(ulong* p);
void DumpTPage(ushort tpage);

void FntLoad(int x, int y);

void SetDispMask(int mask);
void SetDrawArea(DR_AREA* p, RECT16* r);
void SetDrawEnv(DR_ENV* dr_env, DRAWENV* env);
void SetDrawLoad(DR_LOAD* p, RECT16* RECT16);
void SetDrawMode(DR_MODE* p, int dfe, int dtd, int tpage, RECT16* tw);
void SetDrawTPage(DR_TPAGE* p, int dfe, int dtd, int tpage);
void SetDrawMove(DR_MOVE* p, RECT16* RECT16, int x, int y);
void SetDrawOffset(DR_OFFSET* p, ushort* ofs);
void SetDrawStp(DR_STP* p, int pbw);
void SetDumpFnt(int id);

void SetLineF2(LINE_F2* p);
void SetLineF3(LINE_F3* p);
void SetLineF4(LINE_F4* p);
void SetLineG2(LINE_G2* p);
void SetLineG3(LINE_G3* p);
void SetLineG4(LINE_G4* p);
void SetPolyF3(POLY_F3* p);
void SetPolyF4(POLY_F4* p);
void SetPolyFT3(POLY_FT3* p);
void SetPolyFT4(POLY_FT4* p);
void SetPolyG3(POLY_G3* p);
void SetPolyG4(POLY_G4* p);
void SetPolyGT3(POLY_GT3* p);
void SetPolyGT4(POLY_GT4* p);

void SetSemiTrans(void* p, int abe);
void SetShadeTex(void* p, int tge);
void SetSprt(SPRT* p);
void SetSprt16(SPRT_16* p);
void SetSprt8(SPRT_8* p);
void SetTexWindow(DR_TWIN* p, RECT16* tw);
void SetTile(TILE* p);
void SetTile1(TILE_1* p);
void SetTile16(TILE_16* p);
void SetTile8(TILE_8* p);

void   TermPrim(void* p);
ulong* BreakDraw();
void   ContinueDraw(uint* insaddr, uint* contaddr);
int    IsIdleGPU(int max_count);
int    GetODE();

int LoadImage2(RECT16* RECT16, ulong* p);
int StoreImage2(RECT16* RECT16, ulong* p);
int MoveImage2(RECT16* RECT16, int x, int y);
int DrawOTag2(uint*p);

void GetDrawMode(DR_MODE* p);
void GetTexWindow(DR_TWIN* p);
void GetDrawArea(DR_AREA* p);
void GetDrawOffset(DR_OFFSET* p);
void GetDrawEnv2(DR_ENV* p);

// -----------------
// PsyCross commands
// -----------------

void SetPsyXTexture(DR_PSYX_TEX* p, uint grTextureId, int width, int height);
void SetPsyXDebugMarker(DR_PSYX_DBGMARKER* p, const char* str);

#ifdef _DEBUG
    #define PSYX_DBG_MARKER_TEXT(primptr, ot, text) \
    {                                               \
        DR_PSYX_DBGMARKER* marker;                  \
        marker = (DR_PSYX_DBGMARKER*)(primptr);     \
        SetPsyXDebugMarker(marker, text);           \
        (primptr) += sizeof(DR_PSYX_DBGMARKER);     \
        addPrim((ot), marker);                      \
    }
#else
    #define PSYX_DBG_MARKER_TEXT(primptr, ot, text)
#endif

#define PSYX_DBG_MARKER_RESET(primptr, ot) PSYX_DBG_MARKER_TEXT(primptr, ot, nullptr)
