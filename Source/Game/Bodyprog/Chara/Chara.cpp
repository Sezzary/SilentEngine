#include "Framework.h"
#include "Game/Bodyprog/Chara/Chara.h"

#include "Assets/AssetStreamer.h"
#include "Game/Bodyprog/Chara/Anim.h"
#include "Game/Bodyprog/Chara/Collision.h"
#include "Game/Bodyprog/Screen/Data.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    bool Chara_HasFlag(const s_SubCharacter* chara, int flags)
    {
        return chara->flags & flags;
    }

    void Chara_PropsClear(s_SubCharacter* chara)
    {
        for (int i = 0; i < ARRAY_SIZE(chara->properties.dummy.properties_E8); i++)
        {
            chara->properties.dummy.properties_E8[i].val32 = 0;
        }
    }

    void Chara_DamageClear(s_SubCharacter* chara)
    {
        chara->damage.amount      = Q12(0.0f);
        chara->damage.position.vz = Q12(0.0f);
        chara->damage.position.vy = Q12(0.0f);
        chara->damage.position.vx = Q12(0.0f);
    }

    void Chara_AttackReceivedSet(s_SubCharacter* chara, int attack)
    {
        chara->attackReceived = attack;
    }

    int Chara_AttackReceivedGet(s_SubCharacter* chara)
    {
        return chara->attackReceived;
    }

    void Chara_AnimUpdate(s_SubCharacter* chara, const Asset& anmAsset, const s_AnimInfo* animInfos)
    {
        if (!(chara)->properties.npc.freeze)
        {
            auto& animInfo = animInfos[(chara)->model.anim.status];

            animInfo.playbackFunc(chara->model, anmAsset, animInfo);
        }
    }

    void Chara_AnimSet(s_SubCharacter* chara, int animStatus, int keyframeIdx)
    {
        chara->model.anim.status      = animStatus;
        chara->model.anim.time        = Q12(keyframeIdx);
        chara->model.anim.keyframeIdx = keyframeIdx;
    }

    void Chara_AnimStateReset(s_SubCharacter* chara)
    {
        if (chara->properties.npc.resetControlState)
        {
            chara->properties.npc.controlState      = 0;
            chara->model.stateStep                  = 0;
            chara->properties.npc.resetControlState = false;
        }
    }

    void Chara_CollisionSet(s_SubCharacter* chara, const s_CharaKeyframe* keyframe)
    {
        chara->collision.box.topExtent            = keyframe->box.topExtent;
        chara->collision.box.bottomExtent         = keyframe->box.bottomExtent;
        chara->collision.box.height               = keyframe->box.height;
        chara->collision.box.offsetY              = keyframe->box.offsetY;
        chara->collision.shapeOffsets.cylinder.vx = keyframe->shapeOffsets.cylinder.vx;
        chara->collision.shapeOffsets.cylinder.vz = keyframe->shapeOffsets.cylinder.vz;
        chara->collision.cylinder.outerRadius     = keyframe->box.field_8;
        chara->collision.shapeOffsets.box.vx      = keyframe->shapeOffsets.box.vx;
        chara->collision.shapeOffsets.box.vz      = keyframe->shapeOffsets.box.vz;
        chara->collision.cylinder.innerRadius     = keyframe->box.field_A;
    }

    void Chara_MoveSpeedDecrease(s_SubCharacter* chara, q19_12 speed, q19_12 limit)
    {
        chara->moveSpeed = MAX(chara->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, speed), limit);
    }

    void Chara_MoveSpeedUpdate(s_SubCharacter* chara, q19_12 speed, q19_12 limit)
    {
        q19_12 newMoveSpeed = Q12(0.0f);
        if (chara->moveSpeed > limit)
        {
            newMoveSpeed = MAX(chara->moveSpeed - Q12_MULT_PRECISE(g_DeltaTime, speed), limit);
        }
        else
        {
            newMoveSpeed = CLAMP_HIGH(Q12_MULT_PRECISE(g_DeltaTime, speed) + chara->moveSpeed, (limit));
        }

        chara->moveSpeed = newMoveSpeed;
    }
}
