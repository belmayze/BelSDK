/*!
 * @file   belResourceShaderArchive.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "resource/belResource.h"
#include "resource/belResourceLoader.h"

namespace bel::res {

class ShaderArchiveFactory;

//-----------------------------------------------------------------------------
class ShaderArchive : public Resource
{
    friend ShaderArchiveFactory;

    //-------------------------------------------------------------------------
public:
    //! シェーダータイプ
    enum class ShaderType
    {
        VertexPixel,          // vs + ps
        VertexGeometryPixel,  // vs + gs + ps
        Tessellation,         // hs + vs + ds + ps
        TessellationGeometry, // hs + vs + ds + gs + ps
        Compute,              // cs
        Mesh,                 // ms + ps
        MeshAmplification,    // as + ms + ps
        Library,              // lib
        Unknown
    };

    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    ShaderArchive() {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! シェーダー種別
    

    //-------------------------------------------------------------------------
private:
    //! ヘッダー構造体
    struct FileHeader
    {
        uint8_t magic[4];
        uint8_t version;
        uint8_t shader_type;
        uint8_t padding[2];

        uint32_t vertex_size;
        uint32_t vertex_offset;

        uint32_t geometry_size;
        uint32_t geometry_offset;

        uint32_t pixel_size;
        uint32_t pixel_offset;

        uint32_t compute_size;
        uint32_t compute_offset;

        uint32_t hull_size;
        uint32_t hull_offset;

        uint32_t domain_size;
        uint32_t domain_offset;

        uint32_t amplification_size;
        uint32_t amplification_offset;

        uint32_t mesh_size;
        uint32_t mesh_offset;

        uint32_t library_size;
        uint32_t library_offset;
    };

    //-------------------------------------------------------------------------
private:
    const FileHeader* mpFileHeader = nullptr;

    //-------------------------------------------------------------------------
private:
    //! リソース初期化
    ShaderArchive(Resource&& resource, const FileHeader& header)
        : Resource(std::move(resource))
        , mpFileHeader(&header) {}
};

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
class ShaderArchiveFactory
{
    //-------------------------------------------------------------------------
public:
    static ShaderArchive Create(Resource&& resource);
};

//-----------------------------------------------------------------------------
// loadSyncAs 特殊化
template <>
ShaderArchive Loader::loadSyncAs<ShaderArchive>(const std::string& filepath);

//-----------------------------------------------------------------------------
} // bel::res::
