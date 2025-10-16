#include "Framework.h"
#include "Engine/Renderer/Common/Objects/Scene/Sprite2d.h"

#include "Engine/Application.h"
#include "Engine/Assets/Assets.h"
#include "Engine/Renderer/Common/Constants.h"
#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"
#include "Engine/Renderer/Common/Utils.h"

using namespace Silent::Assets;

namespace Silent::Renderer
{
    
    Sprite2d Sprite2d::CreateSprite2d(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                      uint depth, AlignMode alignMode, ScaleMode scaleMode, BlendMode blendMode)
    {
        return Sprite2d
        {
            .AssetIdx = assetIdx,
            .UvMin    = uvMin,
            .UvMax    = uvMax,
            .Position = pos,
            .Rotation = rot,
            .Scale    = scale,
            .Col      = color,
            .Depth    = depth,
            .AlignM   = alignMode,
            .ScaleM   = scaleMode,
            .BlendM   = blendMode
        };
    }

    Sprite2d Sprite2d::CreateSprite2d(int assetIdx, const Vector2i& uvMin, const Vector2i& uvMax,
                                      uint depth, AlignMode alignMode, ScaleMode scaleMode, BlendMode blendMode)
    {
        auto& assets = g_App.GetAssets();

        const auto asset = assets.GetAsset(assetIdx);
        if (asset->Type != AssetType::Tim)
        {
            throw std::runtime_error("Attempted to use non-image asset " + std::to_string(assetIdx) + "as 2D sprite texture.");
        }

        return {}; // @todo
    }
}
