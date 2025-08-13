/*!
 * @file   belResourceMaterial.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel

namespace bel::res {

//-----------------------------------------------------------------------------
//! リソース単体で使われることはなく、res::Model に内包されて使用する
class Material
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    Material() {}
    //! 先頭アドレスで初期化
    Material(uint8_t* ptr) : mPtr(ptr) {}

    //-------------------------------------------------------------------------
    // 情報取得
    //-------------------------------------------------------------------------
public:
    //! 使用するメッシュ番号
    uint32_t getMeshIndex() const
    {
        return getFileHeader_().mesh_index;
    }
    //! 使用するメッシュ開始インデックス
    size_t getMeshStartIndex() const
    {
        return getFileHeader_().mesh_start_index;
    }
    //! 使用するメッシュインデックス数
    size_t getMeshIndexCount() const
    {
        return getFileHeader_().mesh_index_count;
    }

    //! テクスチャー数を取得
    uint32_t getNumTexture() const
    {
        return getFileHeader_().num_texture;
    }
    //! テクスチャー名を取得
    const char* getTextureName(uint32_t index) const
    {
        const TextureInfo& texture_info = getTextureInfo_(index);
        // データが改変され、終端文字が消えても大丈夫か？
        return getStringPtr_(texture_info.name_offset, texture_info.name_strlen);
    }

    //! サンプラー数を取得
    uint32_t getNumSampler() const
    {
        return getFileHeader_().num_sampler;
    }
    //! サンプラー
    void getSampler(D3D12_SAMPLER_DESC& out, uint32_t index) const
    {
        const SamplerInfo& sampler_info = getSamplerInfo_(index);

        out.Filter        = static_cast<D3D12_FILTER>(sampler_info.filter);
        out.AddressU      = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(sampler_info.address_u);
        out.AddressV      = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(sampler_info.address_v);
        out.AddressW      = static_cast<D3D12_TEXTURE_ADDRESS_MODE>(sampler_info.address_w);
        out.MipLODBias    = sampler_info.mip_lod_bias;
        out.MaxAnisotropy = sampler_info.anisotropy;
        std::memcpy(out.BorderColor, sampler_info.border_color, sizeof(float) * 4);
        out.MinLOD = sampler_info.min_mip_lod;
        out.MaxLOD = sampler_info.max_mip_lod;
    }

    //-------------------------------------------------------------------------
    // ファイルフォーマット
    // 
    // +------------------+--------------+
    // | Header           | 12 bytes     |
    // +------------------+--------------+
    // | Textures         |  8 bytes * N |
    // +------------------+--------------+
    // | Samplers         | 36 bytes * N |
    // +------------------+--------------+
    // | Strings          | variable     |
    // +------------------+--------------+
    // 
    //-------------------------------------------------------------------------
private:
    //! ヘッダー構造体
    struct FileHeader
    {
        uint8_t  mesh_index         = 0;
        uint8_t  num_sampler        = 0;
        uint8_t  num_texture        = 0;
        uint8_t  shader_name_strlen = 0;
        uint32_t mesh_start_index   = 0;
        uint32_t mesh_index_count   = 0;
        uint32_t shader_name_offset = 0;
    };
    static_assert(sizeof(FileHeader) == 16);

    // テクスチャー情報
    struct TextureInfo
    {
        uint8_t  name_strlen = 0;
        uint32_t name_offset = 0;
    };
    static_assert(sizeof(TextureInfo) == 8);

    // テクスチャー情報
    struct SamplerInfo
    {
        uint8_t filter            = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        uint8_t address_u         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        uint8_t address_v         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        uint8_t address_w         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        uint8_t shader_visibility = D3D12_SHADER_VISIBILITY_ALL;
        uint8_t anisotropy        = 0;
        float   max_mip_lod       = std::numeric_limits<float>::max();
        float   min_mip_lod       = 0.f;
        float   mip_lod_bias      = 0.f;
        float   border_color[4] = { 0.f };
    };
    static_assert(sizeof(SamplerInfo) == 36);

    //-------------------------------------------------------------------------
private:
    uint8_t* mPtr = nullptr;

    //-------------------------------------------------------------------------
    // internal function
    //-------------------------------------------------------------------------
private:
    //! ファイルヘッダーを取得
    const FileHeader& getFileHeader_() const
    {
        BEL_ASSERT(mPtr != nullptr);
        return *reinterpret_cast<const FileHeader*>(mPtr);
    }
    //! テクスチャー情報を取得
    const TextureInfo& getTextureInfo_(uint32_t index) const
    {
        BEL_ASSERT(mPtr != nullptr);
        BEL_ASSERT(index < getFileHeader_().num_texture);
        return reinterpret_cast<const TextureInfo*>(mPtr + sizeof(FileHeader))[index];
    }
    //! サンプラー情報を取得
    const SamplerInfo& getSamplerInfo_(uint32_t index) const
    {
        BEL_ASSERT(mPtr != nullptr);
        const FileHeader& file_header = getFileHeader_();
        BEL_ASSERT(index < file_header.num_sampler);
        size_t offset = sizeof(FileHeader) + sizeof(TextureInfo) * file_header.num_texture;
        return reinterpret_cast<const SamplerInfo*>(mPtr + offset)[index];
    }
    //! 文字列取得
    const char* getStringPtr_(size_t offset, size_t strlen) const
    {
        BEL_ASSERT(mPtr != nullptr);
        return reinterpret_cast<const char*>(mPtr + offset);
    }
};

//-----------------------------------------------------------------------------
} // bel::res::
