#include "Framework.h"
#include "Engine/Renderer/Backends/OpenGl/Texture.h"

#include "Engine/Application.h"
#include "Engine/Renderer/Backends/OpenGl/ShaderProgram.h"
#include "Engine/Services/Assets/Assets.h"
#include "Engine/Services/Assets/Parsers/Tim.h" // TODO: Shouldn't need this.
#include "Engine/Services/Options.h"

using namespace Silent::Assets;
using namespace Silent::Services;

namespace Silent::Renderer
{
    void Texture::Initialize(const std::filesystem::path& filename, GLenum texUnit, GLenum format, GLenum pixelType)
    {
        const auto& options = g_App.GetOptions();

        // Store texture unit.
        _textureUnit = texUnit;

        // Generate texture object.
        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        // Configure repetition type.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Configure algorithm type used to make image smaller or bigger.
        switch (options->TextureFilter)
        {
            default:
            case TextureFilterType::Nearest:
            {
                SetNearestFilter();
                break;
            }

            case TextureFilterType::Bilinear:
            {
                SetBilinearFilter();
                break;
            }
        }

        // HACK!!!! Demonstrating TIM file load.
        // Read image from file.
        if (filename.extension().string() == ".TIM")
        {
            auto& assets = g_App.GetAssets();

            // Load asset.
            auto name = std::string();
            if (filename == "Assets/TIM/BG_ETC.TIM")
            {
                name = "TIM\\" + filename.filename().string();
                assets.LoadAsset(name).wait();
            }
            else
            {
                name = "1ST\\" + filename.filename().string();
                assets.LoadAsset(name).wait();
            }

            // Get asset data.
            const auto& asset = assets.GetAsset(name);
            auto data         = GetAssetData<TimAsset>(asset->Data);

            // Assign image to texture object.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data->Resolution.x, data->Resolution.y, 0, format, pixelType, data->Pixels.data());
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            stbi_set_flip_vertically_on_load(true);
            auto   res               = Vector2i::Zero;
            int    colorChannelCount = 0;
            uchar* data              = stbi_load(filename.string().c_str(), &res.x, &res.y, &colorChannelCount, 0);

            // Assign image to object.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, res.x, res.y, 0, format, pixelType, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Cleanup.
            stbi_image_free(data);
        }

        Unbind();
    }

    // For framebuffer.
    void Texture::Initialize(const Vector2i& res, GLenum format, GLenum pixelType)
    {
        // Generate texture object.
        glGenTextures(1, &_textureId);
        Bind();

        // Set filter type.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Configure repetition type.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Allocate storage.
        glTexImage2D(GL_TEXTURE_2D, 0, format, res.x, res.y, 0, format, pixelType, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureId, 0);
    }

    void Texture::Bind()
    {
        glActiveTexture(_textureUnit);
        glBindTexture(GL_TEXTURE_2D, _textureId);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Delete()
    {
        glDeleteTextures(1, &_textureId);
    }

    void Texture::SetUnit(ShaderProgram& shaderProg, const std::string& uniName, GLenum texUnitId)
    {
        // Get uniform location.
        uint texUniLoc = glGetUniformLocation(shaderProg.GetId(), uniName.c_str());

        // Set uniform value.
        shaderProg.Activate();
        glUniform1i(texUniLoc, texUnitId);
    }

    void Texture::Resize(const Vector2& res, GLenum format, GLenum pixelType)
    {
        Bind();
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, res.x, res.y, 0, format, pixelType, nullptr);
        Unbind();
    }

    void Texture::RefreshFilter()
    {
        const auto& options = g_App.GetOptions();

        Bind();
        switch (options->TextureFilter)
        {
            default:
            case TextureFilterType::Nearest:
            {
                SetNearestFilter();
                break;
            }

            case TextureFilterType::Bilinear:
            {
                SetBilinearFilter();
                break;
            }
        }
        Unbind();
    }

    void Texture::SetNearestFilter()
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
    }

    void Texture::SetBilinearFilter()
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        float anisotropyMax = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropyMax);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyMax);
    }
}
