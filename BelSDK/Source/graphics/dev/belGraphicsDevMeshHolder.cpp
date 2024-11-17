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
        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cVertices.size();
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cTriangle)].initialize(init_arg);
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
        bel::gfx::Mesh::InitializeArg init_arg;
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
        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(Vertex) * cVertices.size();
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cCube)].initialize(init_arg);
    }
    // SphereIcosahedron
    {
        // 正20面体
        std::unique_ptr<Vertex[]>   vertices = std::make_unique<Vertex[]>(5 * 4);
        std::unique_ptr<uint16_t[]> indices  = std::make_unique<uint16_t[]>(20 * 3);

        // 座標
        for (uint32_t i_row = 0; i_row < 4; ++i_row)
        {
            float offset = Math::cPi / 5 * i_row;
            for (uint32_t i_col = 0; i_col < 5; ++i_col)
            {
                float y = std::cos(Radian((i_row / 3.f) * Math::cPi));
                float x = std::cos(Radian((i_col / 5.f) * Math::c2Pi + offset)) * (1.f - y * y);
                float z = std::sin(Radian((i_col / 5.f) * Math::c2Pi + offset)) * (1.f - y * y);
                Vector3 normal; normal.setNormalize(Vector3(x, y, z));
                vertices[i_row * 5 + i_col] = Vertex(
                    { x * 0.5f, y * 0.5f, z * 0.5f }, normal, { x * 0.5f + 0.5f, y * 0.5f + 0.5f }
                );
            }
        }

        // インデックス
        for (uint32_t i_row = 0; i_row < 4; ++i_row)
        {
            for (uint32_t i_col = 0; i_col < 5; ++i_col)
            {
                if (i_row < 2)
                {
                    if (i_col != 0)
                    {
                        indices[(i_row * 5 + i_col) * 3 + 0] =  i_row      * 5 + i_col;
                        indices[(i_row * 5 + i_col) * 3 + 1] = (i_row + 1) * 5 + i_col - 1;
                        indices[(i_row * 5 + i_col) * 3 + 2] = (i_row + 1) * 5 + i_col;
                    }
                    else
                    {
                        indices[(i_row * 5 + i_col) * 3 + 0] =  i_row      * 5 + i_col;
                        indices[(i_row * 5 + i_col) * 3 + 1] = (i_row + 1) * 5 + 4;
                        indices[(i_row * 5 + i_col) * 3 + 2] = (i_row + 1) * 5 + i_col;
                    }
                }
                else
                {
                    if (i_col != 4)
                    {
                        indices[(i_row * 5 + i_col) * 3 + 0] =  i_row      * 5 + i_col;
                        indices[(i_row * 5 + i_col) * 3 + 1] = (i_row - 1) * 5 + i_col + 1;
                        indices[(i_row * 5 + i_col) * 3 + 2] = (i_row - 1) * 5 + i_col;
                    }
                    else
                    {
                        indices[(i_row * 5 + i_col) * 3 + 0] =  i_row      * 5 + i_col;
                        indices[(i_row * 5 + i_col) * 3 + 1] = (i_row - 1) * 5;
                        indices[(i_row * 5 + i_col) * 3 + 2] = (i_row - 1) * 5 + i_col;
                    }
                }
            }
        }

        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = vertices.get();
        init_arg.vertex_buffer_size = sizeof(Vertex) * 5 * 4;
        init_arg.vertex_stride      = sizeof(Vertex);
        init_arg.p_index_buffer     = indices.get();
        init_arg.index_buffer_size  = sizeof(uint16_t) * 20 * 3;
        mMeshes[to_underlying(Type::cSphereIcosahedron)].initialize(init_arg);
    }

    return true;
}
//-----------------------------------------------------------------------------
}