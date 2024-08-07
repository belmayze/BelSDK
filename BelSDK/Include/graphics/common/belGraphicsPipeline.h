﻿/*!
 * @file   belGraphicsPipeline.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"
#include "resource/belResourceShaderResource.h"

namespace bel::gfx { class ConstantBuffer; }
namespace bel::gfx { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// パイプライン
//-----------------------------------------------------------------------------
class Pipeline
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t                                     mNumRenderTarget = 0;
        std::array<TextureFormat, cMaxRenderTargets> mRenderTargetFormats = {};
        TextureFormat                                mDepthStencilFormat = TextureFormat::cUnknown;

        uint32_t mNumTexture        = 0;
        uint32_t mNumConstantBuffer = 0;
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] arg    初期化引数
     * @param[in] shader シェーダーリソース
     */
    bool initialize(const InitializeArg& arg, const res::ShaderResource& shader);

    //-------------------------------------------------------------------------
    // shader
    //-------------------------------------------------------------------------
public:
    /*!
     * テクスチャーを使用可能にする
     * @param[in] index   レジスター番号
     * @param[in] texture テクスチャー
     */
    void activateTexture(uint32_t index, const Texture& texture) const;

    /*!
     * 定数バッファーを使用可能にする
     * @param[in] index  レジスター番号
     * @param[in] buffer 定数バッファー
     */
    void activateConstantBuffer(uint32_t index, const ConstantBuffer& buffer) const;

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * パイプラインを設定します
     * @param[in] command コマンド
     */
    void setPipeline(CommandContext& command) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature>  mpRootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>  mpPipeline;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    res::ShaderResource::ShaderType              mShaderType = res::ShaderResource::ShaderType::VertexPixel;

    uint32_t mConstantBufferOffset = 0;
};
//-----------------------------------------------------------------------------
}
