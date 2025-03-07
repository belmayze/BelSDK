/*!
 * @file   belGraphicsDevMeshHolder.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/dev/belGraphicsDevMeshHolder.h"

namespace bel::gfx::dev {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool MeshHolder::initialize()
{
    // Triangle
    {
        constexpr std::array<Vertex, 3> cVertices =
        {
            //     positions            , normals          , texcoords
            Vertex({  0.0f,  0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 0.5f, 0.f }),
            Vertex({ -0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 0.0f, 1.f }),
            Vertex({  0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 1.0f, 1.f })
        };
        constexpr std::array<uint16_t, 3> cIndexes = { 0, 1, 2 };
        Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cVertices.size();
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cTriangle)].initialize(init_arg);
    }
    // Quad Triangle
    {
        constexpr std::array<Vertex, 3> cVertices =
        {
            //     positions            , normals          , texcoords
            Vertex({ -1.f,  1.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f }),
            Vertex({ -1.f, -3.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 2.f }),
            Vertex({  2.f,  1.f, 0.f }, { 0.f, 0.f, 1.f }, { 2.f, 0.f })
        };
        constexpr std::array<uint16_t, 3> cIndexes = { 0, 1, 2 };
        Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cVertices.size();
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cQuadTriangle)].initialize(init_arg);
    }
    // Quad
    {
        constexpr std::array<Vertex, 4> cVertices =
        {
            //     positions            , normals          , texcoords
            Vertex({  0.5f,  0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 1.f, 0.f }),
            Vertex({ -0.5f,  0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f }),
            Vertex({ -0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f }),
            Vertex({  0.5f, -0.5f, 0.f }, { 0.f, 0.f, 1.f }, { 1.f, 1.f })
        };
        constexpr std::array<uint16_t, 6> cIndexes = { 0, 1, 2, 0, 2, 3 };
        Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cVertices.size();
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cQuad)].initialize(init_arg);
    }
    // Cube
    {
        constexpr std::array<Vertex, 4 * 6> cVertices =
        {
            //      positions             , normals            , texcoords
            Vertex({  0.5f,  0.5f,  0.5f }, {  0.f,  0.f,  1.f }, { 1.f, 0.f }), // +Z
            Vertex({ -0.5f,  0.5f,  0.5f }, {  0.f,  0.f,  1.f }, { 0.f, 0.f }),
            Vertex({ -0.5f, -0.5f,  0.5f }, {  0.f,  0.f,  1.f }, { 0.f, 1.f }),
            Vertex({  0.5f, -0.5f,  0.5f }, {  0.f,  0.f,  1.f }, { 1.f, 1.f }),
                                              
            Vertex({ -0.5f,  0.5f, -0.5f }, {  0.f,  0.f, -1.f }, { 1.f, 0.f }), // -Z
            Vertex({  0.5f,  0.5f, -0.5f }, {  0.f,  0.f, -1.f }, { 0.f, 0.f }),
            Vertex({  0.5f, -0.5f, -0.5f }, {  0.f,  0.f, -1.f }, { 0.f, 1.f }),
            Vertex({ -0.5f, -0.5f, -0.5f }, {  0.f,  0.f, -1.f }, { 1.f, 1.f }),
                                              
            Vertex({  0.5f,  0.5f, -0.5f }, {  1.f,  0.f,  0.f }, { 1.f, 0.f }), // +X
            Vertex({  0.5f,  0.5f,  0.5f }, {  1.f,  0.f,  0.f }, { 0.f, 0.f }),
            Vertex({  0.5f, -0.5f,  0.5f }, {  1.f,  0.f,  0.f }, { 0.f, 1.f }),
            Vertex({  0.5f, -0.5f, -0.5f }, {  1.f,  0.f,  0.f }, { 1.f, 1.f }),
                                              
            Vertex({ -0.5f,  0.5f,  0.5f }, { -1.f,  0.f,  0.f }, { 1.f, 0.f }), // -X
            Vertex({ -0.5f,  0.5f, -0.5f }, { -1.f,  0.f,  0.f }, { 0.f, 0.f }),
            Vertex({ -0.5f, -0.5f, -0.5f }, { -1.f,  0.f,  0.f }, { 0.f, 1.f }),
            Vertex({ -0.5f, -0.5f,  0.5f }, { -1.f,  0.f,  0.f }, { 1.f, 1.f }),
                                              
            Vertex({  0.5f,  0.5f, -0.5f }, {  0.f,  1.f,  0.f }, { 1.f, 0.f }), // +Y
            Vertex({ -0.5f,  0.5f, -0.5f }, {  0.f,  1.f,  0.f }, { 0.f, 0.f }),
            Vertex({ -0.5f,  0.5f,  0.5f }, {  0.f,  1.f,  0.f }, { 0.f, 1.f }),
            Vertex({  0.5f,  0.5f,  0.5f }, {  0.f,  1.f,  0.f }, { 1.f, 1.f }),
                                              
            Vertex({  0.5f,  0.5f,  0.5f }, {  0.f, -1.f,  0.f }, { 1.f, 0.f }), // -Y
            Vertex({ -0.5f,  0.5f,  0.5f }, {  0.f, -1.f,  0.f }, { 0.f, 0.f }),
            Vertex({ -0.5f,  0.5f, -0.5f }, {  0.f, -1.f,  0.f }, { 0.f, 1.f }),
            Vertex({  0.5f,  0.5f, -0.5f }, {  0.f, -1.f,  0.f }, { 1.f, 1.f })
        };
        constexpr std::array<uint16_t, 6 * 6> cIndexes =
        {
             0,  1,  2,  0,  2,  3, // +Z
             4,  5,  6,  4,  6,  7, // -Z
             8,  9, 10,  8, 10, 11, // +X
            12, 13, 14, 12, 14, 15, // -X
            16, 17, 18, 16, 18, 19, // +Y
            20, 21, 22, 20, 22, 23, // -Y
        };
        Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cVertices.size();
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cCube)].initialize(init_arg);
    }
    // Sphere
    {
        // 球体（縦11分割 x 円周30分割）
        static constexpr uint32_t cDivY = 11;
        static constexpr uint32_t cDivR = 30;

        std::unique_ptr<Vertex[]>   vertices = std::make_unique<Vertex[]>(cDivY * cDivR);
        std::unique_ptr<uint16_t[]> indices  = std::make_unique<uint16_t[]>((cDivY - 1) * cDivR * 6);

        // 球体
        {
            // 0         5             29
            // ----------- ... --------- 0
            // |/|/|/|/|/| ... |/|/|/|/|
            // ----------- ... --------- 1
            // |/|/|/|/|/| ... |/|/|/|/|
            // ....
            // ----------- ... --------- 9
            // |/|/|/|/|/| ... |/|/|/|/|
            // ----------- ... --------- 10

            // 頂点バッファー
            for (uint32_t i_y = 0; i_y < cDivY; ++i_y)
            {
                // -90 ~ 90 を 10 分割
                Degree y_deg = Degree((180.f / static_cast<float>(cDivY - 1)) * i_y - 90.f);
                float y = sin(Radian(y_deg));
                float s = cos(Radian(y_deg));

                for (uint32_t i_r = 0; i_r < cDivR; ++i_r)
                {
                    // 360 度を 30 分割
                    Degree x_deg = Degree((360.f / static_cast<float>(cDivR)) * i_r);
                    float x = cos(Radian(x_deg)) * s;
                    float z = sin(Radian(x_deg)) * s;

                    //
                    uint32_t i_vertex = i_y * cDivR + i_r;
                    vertices[i_vertex].position = Vector3(x * 0.5f, y * 0.5f, z * 0.5f);
                    vertices[i_vertex].normal.setNormalize(vertices[i_vertex].position);
                    vertices[i_vertex].texcoord = Vector2(
                        static_cast<float>(i_r) / static_cast<float>(cDivR - 1),
                        static_cast<float>(i_y) / static_cast<float>(cDivY - 1));
                }
            }

            // インデックスバッファー
            for (uint32_t i_y = 0; i_y < (cDivY - 1); ++i_y)
            {
                for (uint32_t i_r = 0; i_r < cDivR; ++i_r)
                {
                    uint16_t i0 =  i_y      * cDivR +   i_r;
                    uint16_t i1 = (i_y + 1) * cDivR +   i_r;
                    uint16_t i2 = (i_y + 1) * cDivR + ((i_r + 1) % cDivR); // 最後の面は最初とつなげる
                    uint16_t i3 =  i_y      * cDivR + ((i_r + 1) % cDivR);

                    uint32_t i_index = i_y * cDivR + i_r;
                    indices[i_index * 6 + 0] = i0;
                    indices[i_index * 6 + 1] = i1;
                    indices[i_index * 6 + 2] = i2;
                    indices[i_index * 6 + 3] = i0;
                    indices[i_index * 6 + 4] = i2;
                    indices[i_index * 6 + 5] = i3;
                }
            }
        }

        Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = vertices.get();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cDivY * cDivR;
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = indices.get();
        init_arg.index_buffer_size  = sizeof(uint16_t) * (cDivY - 1) * cDivR * 6;
        mMeshes[to_underlying(Type::cSphere)].initialize(init_arg);
    }

    return true;
}
//-----------------------------------------------------------------------------
}