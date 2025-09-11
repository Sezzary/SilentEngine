#pragma once

namespace Silent::Assets
{
    struct TmdQuad
    {

    };

    struct TmdTriangle
    {

    };

    struct TmdLine
    {

    };

    struct TmdSprite
    {

    };

    using TmdPrimitive = std::variant<TmdQuad,
                                      TmdTriangle,
                                      TmdLine,
                                      TmdSprite>;

    struct TmdMesh
    {
        std::vector<Vector3>      Vertices   = {};
        std::vector<Vector3>      Normals    = {};
        std::vector<TmdPrimitive> Primitives = {};
    };

    struct TmdAsset
    {
        struct Triangle
        {
            static constexpr uint TRI_VERTEX_COUNT = 3;

            std::array<uint, TRI_VERTEX_COUNT> Vertices = {};
            std::array<uint, TRI_VERTEX_COUNT> Normals  = {};
        };

        struct Mesh
        {
            std::vector<Vector3>  Vertices  = {};
            std::vector<Vector3>  Normals   = {};
            std::vector<Triangle> Triangles = {};
        };

        std::vector<Mesh> Meshes = {};
    };

    /** @brief Parses a TMD file to a usable asset.
     *
     * @param filename Absolute asset file path on the system.
     * @return Parsed TMD asset data as a `void` pointer.
     */
    std::shared_ptr<void> ParseTmd(const std::filesystem::path& filename);
}
