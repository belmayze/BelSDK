/*!
 * @file   belGraphicsTexture.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"
#include "graphics/internal/belGraphicsTextureDescriptorHandle.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// テクスチャー
//-----------------------------------------------------------------------------
class Texture
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t         mWidth     = 1;
        uint32_t         mHeight    = 1;
        uint32_t         mDepth     = 1;
        uint32_t         mNumMip    = 1;
        TextureFormat    mFormat    = TextureFormat::cR8G8B8A8_uNorm;
        TextureDimension mDimension = TextureDimension::c2D;
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * テクスチャー初期化
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    /*!
     * テクスチャーを GPU メモリーで初期化します
     * @param[in] arg        初期化引数
     * @param[in] p_resource GPUリソース
     * @param[in] state      リソースステート
     */
    bool initializeFromGPUMemory(const InitializeArg& arg, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource, ResourceState state);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! テクスチャーサイズを取得する
    uint32_t getWidth() const { return mWidth; }
    uint32_t getHeight() const { return mHeight; }
    uint32_t getDepth() const { return mDepth; }

    //! ミップマップ数を取得する
    uint32_t getNumMip() const { return mNumMip; }

    //! フォーマットを取得する
    TextureFormat getFormat() const { return mFormat; }

    //! ディメンジョンを取得する
    TextureDimension getDimension() const { return mDimension; }

    //! リソース取得
    ID3D12Resource& getResource() const { BEL_ASSERT(mpResource.Get() != nullptr); return *mpResource.Get(); }

    //! デスクリプターハンドルを取得
    const TextureDescriptorHandle& getDescriptorHandle() const { return mDescriptorHandle; }

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * リソースステートの変更を行います
     * @param[in] command    コマンド
     * @param[in] next_state 次のステート状態
     */
    void barrierTransition(CommandContext& command, ResourceState next_state) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mpResource;

    uint32_t              mWidth         = 1;
    uint32_t              mHeight        = 1;
    uint32_t              mDepth         = 1;
    uint32_t              mNumMip        = 1;
    TextureFormat         mFormat        = TextureFormat::cR8G8B8A8_uNorm;
    TextureDimension      mDimension     = TextureDimension::c2D;
    mutable ResourceState mResourceState = ResourceState::cGenericRead;

    TextureDescriptorHandle mDescriptorHandle;
};
//-----------------------------------------------------------------------------
}
