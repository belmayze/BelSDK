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
        constexpr std::array<float, (3 + 3 + 2) * 3> cVertices =
        {
            // positions     , normals      , texcoords
             0.0f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.5f, 0.f,
            -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.0f, 1.f,
             0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.0f, 1.f
        };
        constexpr std::array<uint16_t, 3> cIndexes = { 0, 1, 2 };
        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(float) * cVertices.size();
        init_arg.vertex_stride      = sizeof(float) * (3 + 3 + 2);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cTriangle)].initialize(init_arg);
    }
    // Quad
    {
        constexpr std::array<float, (3 + 3 + 2) * 4> cVertices =
        {
            // positions     , normals      , texcoords
             0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 1.f, 0.f,
            -0.5f,  0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
            -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 0.f, 1.f,
             0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f
        };
        constexpr std::array<uint16_t, 6> cIndexes = { 0, 1, 2, 0, 2, 3 };
        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.p_vertex_buffer    = cVertices.data();
        init_arg.vertex_buffer_size = sizeof(float) * cVertices.size();
        init_arg.vertex_stride      = sizeof(float) * (3 + 3 + 2);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cQuad)].initialize(init_arg);
    }
    // Cube
    {
        constexpr std::array<float, (3 + 3 + 2) * 4 * 6> cVertices =
        {
            // positions       , normals         , texcoords
             0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f, 1.f, 0.f, // +Z
            -0.5f,  0.5f,  0.5f,  0.f,  0.f,  1.f, 0.f, 0.f,
            -0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f, 0.f, 1.f,
             0.5f, -0.5f,  0.5f,  0.f,  0.f,  1.f, 1.f, 1.f,

            -0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f, 1.f, 0.f, // -Z
             0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f, 0.f, 0.f,
             0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f, 0.f, 1.f,
            -0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f, 1.f, 1.f,

             0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f, 1.f, 0.f, // +X
             0.5f,  0.5f,  0.5f,  1.f,  0.f,  0.f, 0.f, 0.f,
             0.5f, -0.5f,  0.5f,  1.f,  0.f,  0.f, 0.f, 1.f,
             0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f, 1.f, 1.f,

            -0.5f,  0.5f,  0.5f, -1.f,  0.f,  0.f, 1.f, 0.f, // -X
            -0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f, 0.f, 0.f,
            -0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f, 0.f, 1.f,
            -0.5f, -0.5f,  0.5f, -1.f,  0.f,  0.f, 1.f, 1.f,

             0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f, 1.f, 0.f, // +Y
            -0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f, 0.f, 0.f,
            -0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f, 0.f, 1.f,
             0.5f,  0.5f,  0.5f,  0.f,  1.f,  0.f, 1.f, 1.f,

             0.5f,  0.5f,  0.5f,  0.f, -1.f,  0.f, 1.f, 0.f, // -Y
            -0.5f,  0.5f,  0.5f,  0.f, -1.f,  0.f, 0.f, 0.f,
            -0.5f,  0.5f, -0.5f,  0.f, -1.f,  0.f, 0.f, 1.f,
             0.5f,  0.5f, -0.5f,  0.f, -1.f,  0.f, 1.f, 1.f,
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
        init_arg.vertex_buffer_size = sizeof(float) * cVertices.size();
        init_arg.vertex_stride      = sizeof(float) * (3 + 3 + 2);
        init_arg.p_index_buffer     = cIndexes.data();
        init_arg.index_buffer_size  = sizeof(uint16_t) * cIndexes.size();
        mMeshes[to_underlying(Type::cCube)].initialize(init_arg);
    }

    return true;
}
//-----------------------------------------------------------------------------
}