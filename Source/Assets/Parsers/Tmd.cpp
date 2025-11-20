#include "Framework.h"
#include "Assets/Parsers/Tmd.h"

namespace Silent::Assets
{
    struct MeshMetadata
    {
        uint32 VertexOffset    = 0;
        uint32 VertexCount     = 0;
        uint32 NormalOffset    = 0;
        uint32 NormalCount     = 0;
        uint32 PrimitiveOffset = 0;
        uint32 PrimitiveCount  = 0;
        uint32 Scale           = 0;
    };

    std::shared_ptr<void> ParseTmd(const std::filesystem::path& filename)
    {
        constexpr int FIXP_FLAG = 1 << 0;

        // Read file.
        auto file = std::ifstream(filename, std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error(Fmt("Failed to open TMD `{}`.", filename.string()));
        }

        // Read version (unused).
        uint32 ver = 0;
        file.read((byte*)&ver, 4);

        // Read flags.
        uint32 flags = 0;
        file.read((byte*)&flags, 4);

        // Read mesh count.
        uint16 meshCount = 0;
        file.read((byte*)&meshCount, 2);

        // Read mesh metadatas.
        auto metadatas = std::vector<MeshMetadata>(meshCount);
        for (auto& metadata : metadatas)
        {
            // Read vertex data.
            file.read((byte*)&metadata.VertexOffset, 4);
            file.read((byte*)&metadata.VertexCount, 4);

            // Read normal data.
            file.read((byte*)&metadata.NormalOffset, 4);
            file.read((byte*)&metadata.NormalCount, 4);

            // Read primitive data.
            file.read((byte*)&metadata.PrimitiveOffset, 4);
            file.read((byte*)&metadata.PrimitiveCount, 4);

            // Read scale.
            file.read((byte*)&metadata.Scale, 4);

            if (!(flags & FIXP_FLAG))
            {
                // Something.
            }
        }

        // Create asset.
        auto asset = TmdAsset
        {
            .Meshes = std::vector<TmdAsset::Mesh>(meshCount)
        };

        // Read meshes.
        for (int i = 0; i < meshCount; i++)
        {
            auto&       mesh     = asset.Meshes[i];
            const auto& metadata = metadatas[i];

            // Read vertices.
            mesh.Vertices.reserve(metadata.VertexCount);
            for (int j = 0; j < metadata.VertexCount; j++)
            {
                // Read components.
                int16 x   = 0;
                int16 y   = 0;
                int16 z   = 0;
                int16 pad = 0;
                file.read((byte*)&x, 2);
                file.read((byte*)&y, 2);
                file.read((byte*)&z, 2);
                file.read((byte*)&pad, 2);

                // Collect vertex.
                mesh.Vertices.push_back(Vector3(x, y, z));
            }

            // Read normals.
            mesh.Normals.reserve(metadata.NormalCount);
            for (int j = 0; j < metadata.NormalCount; j++)
            {
                // Read components.
                int16 x   = 0;
                int16 y   = 0;
                int16 z   = 0;
                int16 pad = 0;
                file.read((byte*)&x, 2);
                file.read((byte*)&y, 2);
                file.read((byte*)&z, 2);
                file.read((byte*)&pad, 2);

                // Collect normal.
                auto normal = Vector3::Normalize((Vector3(x, y, z) / 4096.0f));
                mesh.Normals.push_back(normal);
            }

            // Read primitives.
            mesh.Triangles.reserve(metadata.PrimitiveCount);
            for (int j = 0; j < metadata.PrimitiveCount; j++)
            {
                // Read attributes.
                int8 olen  = 0;
                int8 ilen  = 0;
                int8 flags = 0;
                int8 mode  = 0;
                file.read((byte*)&olen, 1);
                file.read((byte*)&ilen, 1);
                file.read((byte*)&flags, 1);
                file.read((byte*)&mode, 1);

                //????


                // Read vertex indices.
                /*uint16 vertIdx0 = 0;
                uint16 vertIdx1 = 0;
                uint16 vertIdx2 = 0;
                file.read((byte*)&vertIdx0, 2);
                file.read((byte*)&vertIdx1, 2);
                file.read((byte*)&vertIdx2, 2);

                // Read normal indices.
                uint16 normalIdx0 = 0;
                uint16 normalIdx1 = 0;
                uint16 normalIdx2 = 0;
                file.read((byte*)&normalIdx0, 2);
                file.read((byte*)&normalIdx1, 2);
                file.read((byte*)&normalIdx2, 2);

                // Collect triangle;
                mesh.Triangles.push_back(TmdAsset::Triangle
                {
                    .Vertices = { vertIdx0, vertIdx1, vertIdx2 },
                    .Normals  = { normalIdx0, normalIdx1, normalIdx2 }
                });*/
            }
        }

        return std::make_shared<TmdAsset>(std::move(asset));
    }
}
