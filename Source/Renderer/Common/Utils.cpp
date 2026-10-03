#include "Framework.h"
#include "Renderer/Common/Utils.h"

#include "Application.h"
#include "Renderer/Common/Constants.h"
#include "Renderer/Common/Enums.h"
#include "Renderer/Renderer.h"

namespace Silent::Renderer
{
    Vector3 GetNormal(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2)
    {
        // Compute edges.
        auto edge0 = vert1 - vert0;
        auto edge1 = vert2 - vert0;

        // Compute edge normal.
        return Vector3::Normalize(Vector3::Cross(edge0, edge1));
    }

    Vector2 GetAlignmentPivot(AlignMode alignMode)
    {
        switch (alignMode)
        {
            default:
            case AlignMode::Center:
            {
                return Vector2(0.5f, 0.5f);
            }
            case AlignMode::CenterTop:
            {
                return Vector2(0.5f, 0.0f);
            }
            case AlignMode::CenterBottom:
            {
                return Vector2(0.5f, 1.0f);
            }
            case AlignMode::CenterLeft:
            {
                return Vector2(0.0f, 0.5f);
            }
            case AlignMode::CenterRight:
            {
                return Vector2(1.0f, 0.5f);
            }
            case AlignMode::TopLeft:
            {
                return Vector2(0.0f, 0.0f);
            }
            case AlignMode::TopRight:
            {
                return Vector2(0.0f, 1.0f);
            }
            case AlignMode::BottomLeft:
            {
                return Vector2(1.0f, 0.0f);
            }
            case AlignMode::BottomRight:
            {
                return Vector2(1.0f, 1.0f);
            }
        }

        return Vector2::Zero;
    }

    Vector2 GetGridAlignedScreenPercent(const Vector2& pos, float virtualHeight)
    {
        // Compute grid size.
        auto  res      = g_App.GetWindowResolution(); // @todo NOT thread safe!! Cannot use this function in renderer.
        float rawScale = res.y / virtualHeight;
        float scale    = std::max(1.0f, floorf(rawScale));
        auto  gridSize = Vector2(res.x / rawScale, virtualHeight);

        // Compute snapped pixel position.
        auto uv           = pos / 100.0f;
        auto snappedPixel = Vector2(roundf(uv.x * gridSize.x), 
                                    roundf(uv.y * gridSize.y));

        // Compute snapped screen position in percent.
        return (snappedPixel / gridSize) * SCREEN_SPACE_RES;
    }

    Vector2 GetScreenAspectCorrection(ScaleMode scaleMode)
    {
        const auto& renderer = g_App.GetRenderer();

        // @todo Only thread-safe when used in renderer, but useful to call elsewhere too.
        float aspect = renderer.GetViewportAspectRatio();

        auto aspectCorrection = Vector2::One;
        switch(scaleMode)
        {
            case ScaleMode::VerticalEdge:
            case ScaleMode::Fit:
            {
                aspectCorrection.x = 1.0f / aspect;
                break;
            }
            case ScaleMode::HorizontalEdge:
            case ScaleMode::Fill:
            {
                aspectCorrection.y = aspect;
                break;
            }
            default:
            {
                break;
            }
        }

        return aspectCorrection;
    }

    Vector2 ConvertRetroScreenPixelsToPercent(const Vector2i& pos)
    {
        return (pos.ToVector2() / RETRO_SCREEN_SPACE_RES) * SCREEN_SPACE_RES;
    }

    Vector2i ConvertScreenPercentToRetroPixels(const Vector2& pos)
    {
        return Vector2i((pos / SCREEN_SPACE_RES) * RETRO_SCREEN_SPACE_RES);
    }

    Vector2 ConvertScreenPercentToNdc(const Vector2& pos)
    {
        return Vector2(((pos.x * 2.0f) / SCREEN_SPACE_RES.x) - 1.0f,
                       1.0f - ((pos.y * 2.0f) / SCREEN_SPACE_RES.y));
    }

    Vector2 ConvertNdcToScreenPercent(const Vector2& ndc)
    {
        return Vector2(((ndc.x + 1.0f) * SCREEN_SPACE_RES.x) / 2.0f,
                       ((1.0f - ndc.y) * SCREEN_SPACE_RES.y) / 2.0f);
    }
}
