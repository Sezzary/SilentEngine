#pragma once

#include "Game/Bodyprog/Chara/Model.h"
#include "Game/Bodyprog/Formats/Lm.h"
#include "Game/Bodyprog/Formats/Model.h"
#include "Game/Bodyprog/Gfx/WorldObject.h"
#include "Game/Bodyprog/Map/Map.h"
#include "Game/Bodyprog/View/Structs.h"

namespace Silent::Game
{
    /** @brief World GFX workspace.
     * TODO: Could be `s_WorldWork`? Contains more than GFX data.
     */
    struct s_WorldGfxWork
    {
        s_MapInfo*          mapInfo;
        u8                  useStoredPoint; /** `bool` */
        s8                  __pad_5[3];
        VECTOR3             ipdSamplePoint; /** Used by IPD logic to sample which chunks to load or unload. */
        u8*                 charaLmBuffer;
        s_CharaModel*       registeredCharaModels[Chara_Count];
        s_CharaModel        charaModels[CHARA_GROUP_COUNT];
        s_CharaModel        harryModel;
        s_HeldItem          heldItem; /** Item held by the player. */
        s_CollisionTrigger* collisionTriggers;
        VC_CAMERA_INTINFO   vcCameraInternalInfo; /** Debug camera info. */
        s_LmHeader          itemLmHdr;
        u8                  itemLmData[4096 - sizeof(s_LmHeader)]; // 4kb allocated for 2.75kb game files.
        s32                 itemQueueLmIdx;
        s32                 objectCount;                     /** `objects` size. */
        s_WorldObject       objects[WORLD_OBJECT_COUNT_MAX]; /** World objects to draw. */
    };

    /** @brief World fog info. */
    struct s_Fog
    {
        s32     nearDistance;
        q23_8   farDistance; // "DrawDistanmce" in SHME, "has no effect when fog is disabled".
        s32     depthShift;  // "FogThing1" from SHME. Affects the distance where fog begins.
        s32     intensity;   // "FogThing2" from SHME. Affects the distance where fog begins.
        CVECTOR color;
    };

    // Related to `s_PointLight`.
    typedef struct
    {
        VECTOR3 field_0[2][1];
    } s_WorldEnvWork_84;

    /** @brief Dynamic point light. */
    struct s_PointLight
    {
        s32               field_0; // Light intensity in Q4?
        q3_12             lensFlareIntensity;
        q19_12            intensity;
        SVECTOR           direction; /** Q3.12 */
        VECTOR3           position;  /** Q19.12 */
        SVECTOR           rotation;  /** Q3.12 */
        SVECTOR           field_28;  // Q8 light position for matrix?
        SVECTOR           field_30;  // Light offset?
        s_WorldEnvWork_84 field_38[3];
    };

    /** @brief World environment workspace.
     *
     * Holds fog distances and ramps, lighting and color parameters, water zone references, and other per-map environmental
     * data used for world drawing.
     */
    struct s_WorldEnvWork
    {
        u8           field_0;                // `bool`?
        u8           isFogEnabled;           /** `bool` */
        u8           enableTintLightOverlap; /** `bool`. Allows overlapping the world tint and volumetric light color.
                                              * @note Specifically modifies the volumetric light color that the point light generates,
                                              * the color it applies to the enviroment doesn't change. */
        u8           field_3; // Enviroment lighting.
        s_WaterZone* waterZones;
        s32          screenBrightness;
        s_Fog        fog;
        s32          field_20; // Map lighting.
        CVECTOR      field_24; // Character color lighting.
        CVECTOR      worldTintColor;
        MATRIX       colorMat;
        s_PointLight light;
        u8           fogRamp[128]; // Fog-related values based on `fog.nearDistance`/`fog.farDistance`.
        u16          field_14C;
    };
}
