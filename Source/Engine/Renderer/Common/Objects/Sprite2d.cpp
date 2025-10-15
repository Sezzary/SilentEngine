#include "Framework.h"
#include "Engine/Renderer/Common/Objects/Sprite2d.h"

#include "Engine/Renderer/Common/Constants.h"
#include "Engine/Renderer/Common/Enums.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"
#include "Engine/Renderer/Common/Utils.h"

namespace Silent::Renderer
{
    Sprite2d Sprite2d::CreateSprite2d(int assetIdx, const Vector2& pos, float rot, const Vector2& scale, const Color& color,
                                      int depth, BlendMode blendMode, ScaleMode scaleMode)
    {
        // Transformed sprite. Primitive2d can also have this?

        auto rotMat = Matrix::CreateRotationZ(rot);
        auto points = std::array<Vector2, QUAD_VERTEX_COUNT>
        {
            Vector2::Transform(pos + SCREEN_SPACE_RES, rotMat),
            Vector2::Transform(pos + SCREEN_SPACE_RES, rotMat),
            Vector2::Transform(pos + SCREEN_SPACE_RES, rotMat),
            Vector2::Transform(pos + SCREEN_SPACE_RES, rotMat)
        };

        // etc etc

        return {};
        /*return Sprite2d
        {
            Primitive2d::CreateQuad(),
            .AssetIdx = assetIdx
        };*/
    }

    /*Sprite2d Sprite2d::CreateSprite2d(const Vector2i& pos)
    {
        return CreateSprite2d(pos );
    }*/
}
