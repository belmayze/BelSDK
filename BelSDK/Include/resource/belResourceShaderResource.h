/*!
 * @file   belResourceShaderResource.h
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

class ShaderResourceFactory;

//-----------------------------------------------------------------------------
class ShaderResource : public Resource
{
    friend ShaderResourceFactory;

    //-------------------------------------------------------------------------
public:
    //! シェーダータイプ
    //! @todo: この列挙は他でも使うのでクラスの外に出したい
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
    ShaderResource() {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! シェーダー種別
    ShaderType getShaderType() const { return static_cast<ShaderType>(getHeader_().shader_type); }

    //! 頂点シェーダー取得
    std::pair<size_t, const void*> getVertexBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::VertexPixel ||
            getShaderType() == ShaderType::VertexGeometryPixel ||
            getShaderType() == ShaderType::Tessellation ||
            getShaderType() == ShaderType::TessellationGeometry
        );
        BEL_ASSERT(getHeader_().vertex_size != 0 && getHeader_().vertex_offset != 0);
        return { static_cast<size_t>(getHeader_().vertex_size), getShaderBinary_(getHeader_().vertex_offset) };
    }

    //! ジオメトリシェーダー取得
    std::pair<size_t, const void*> getGeometryBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::VertexGeometryPixel ||
            getShaderType() == ShaderType::TessellationGeometry
        );
        BEL_ASSERT(getHeader_().geometry_size != 0 && getHeader_().geometry_offset != 0);
        return { static_cast<size_t>(getHeader_().geometry_size), getShaderBinary_(getHeader_().geometry_offset) };
    }

    //! ピクセルシェーダー取得
    std::pair<size_t, const void*> getPixelBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::VertexPixel ||
            getShaderType() == ShaderType::VertexGeometryPixel ||
            getShaderType() == ShaderType::Tessellation ||
            getShaderType() == ShaderType::TessellationGeometry ||
            getShaderType() == ShaderType::Mesh ||
            getShaderType() == ShaderType::MeshAmplification
        );
        BEL_ASSERT(getHeader_().pixel_size != 0 && getHeader_().pixel_offset != 0);
        return { static_cast<size_t>(getHeader_().pixel_size), getShaderBinary_(getHeader_().pixel_offset) };
    }

    //! ハルシェーダー取得
    std::pair<size_t, const void*> getHullBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::Tessellation ||
            getShaderType() == ShaderType::TessellationGeometry
        );
        BEL_ASSERT(getHeader_().hull_size != 0 && getHeader_().hull_offset != 0);
        return { static_cast<size_t>(getHeader_().hull_size), getShaderBinary_(getHeader_().hull_offset) };
    }

    //! ドメインシェーダー取得
    std::pair<size_t, const void*> getDomainBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::Tessellation ||
            getShaderType() == ShaderType::TessellationGeometry
        );
        BEL_ASSERT(getHeader_().domain_size != 0 && getHeader_().domain_offset != 0);
        return { static_cast<size_t>(getHeader_().domain_size), getShaderBinary_(getHeader_().domain_offset) };
    }

    //! コンピュートシェーダー取得
    std::pair<size_t, const void*> getComputeBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::Compute
        );
        BEL_ASSERT(getHeader_().compute_size != 0 && getHeader_().compute_offset != 0);
        return { static_cast<size_t>(getHeader_().compute_size), getShaderBinary_(getHeader_().compute_offset) };
    }

    //! 増幅シェーダー取得
    std::pair<size_t, const void*> getAmplificationBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::MeshAmplification
        );
        BEL_ASSERT(getHeader_().amplification_size != 0 && getHeader_().amplification_offset != 0);
        return { static_cast<size_t>(getHeader_().amplification_size), getShaderBinary_(getHeader_().amplification_offset) };
    }

    //! メッシュシェーダー取得
    std::pair<size_t, const void*> getMeshBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::Mesh ||
            getShaderType() == ShaderType::MeshAmplification
        );
        BEL_ASSERT(getHeader_().mesh_size != 0 && getHeader_().mesh_offset != 0);
        return { static_cast<size_t>(getHeader_().mesh_size), getShaderBinary_(getHeader_().mesh_offset) };
    }

    //! ライブラリシェーダー取得
    std::pair<size_t, const void*> getLibraryBinary() const
    {
        BEL_ASSERT(
            getShaderType() == ShaderType::Library
        );
        BEL_ASSERT(getHeader_().library_size != 0 && getHeader_().library_offset != 0);
        return { static_cast<size_t>(getHeader_().library_size), getShaderBinary_(getHeader_().library_offset) };
    }

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
    //! ヘッダー取得関数
    const FileHeader& getHeader_() const { BEL_ASSERT(mpFileHeader != nullptr); return *mpFileHeader; }

    //! シェーダーの先頭ポインター取得
    const void* getShaderBinary_(uint32_t offset) const
    {
        const void* ptr = getBuffer();
        return reinterpret_cast<const uint8_t*>(ptr) + offset;
    }

    //-------------------------------------------------------------------------
private:
    //! リソース初期化
    ShaderResource(Resource&& resource, const FileHeader& header)
        : Resource(std::move(resource))
        , mpFileHeader(&header) {}
};

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
class ShaderResourceFactory
{
    //-------------------------------------------------------------------------
public:
    static ShaderResource Create(Resource&& resource);
};

//-----------------------------------------------------------------------------
// loadSyncAs 特殊化
template <>
ShaderResource Loader::loadSyncAs<ShaderResource>(const std::string& filepath);

//-----------------------------------------------------------------------------
} // bel::res::
