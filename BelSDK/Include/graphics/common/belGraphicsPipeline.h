/*!
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
namespace bel::gfx { class DynamicDescriptorHandle; }
namespace bel::gfx { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// パイプライン
//-----------------------------------------------------------------------------
class Pipeline
{
    //-------------------------------------------------------------------------
public:
    //! デプス設定
    struct DepthConfig
    {
        bool depth_enable = false;
        bool depth_write  = false;
    };
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t                                     num_render_target     = 0;
        std::array<TextureFormat, cMaxRenderTargets> render_target_formats = {};
        TextureFormat                                depth_stencil_format  = TextureFormat::cUnknown;

        DepthConfig depth_config;

        uint32_t num_texture         = 0;
        uint32_t num_constant_buffer = 0;
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
     * @param[in] handle  デスクリプターハンドル
     * @param[in] index   レジスター番号
     * @param[in] texture テクスチャー
     */
    void activateTexture(DynamicDescriptorHandle& handle, uint32_t index, const Texture& texture) const;

    /*!
     * 定数バッファーを使用可能にする
     * @param[in] handle デスクリプターハンドル
     * @param[in] index  レジスター番号
     * @param[in] buffer 定数バッファー
     */
    void activateConstantBuffer(DynamicDescriptorHandle& handle, uint32_t index, const ConstantBuffer& buffer) const;

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
    // getter
    //-------------------------------------------------------------------------
public:
    //! デスクリプターの数を取得する
    uint32_t getNumDescriptor() const { return mNumDescriptor; }

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature>  mpRootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState>  mpPipeline;
    res::ShaderResource::ShaderType              mShaderType = res::ShaderResource::ShaderType::VertexPixel;

    uint32_t mNumDescriptor        = 0;
    uint32_t mConstantBufferOffset = 0;
};
//-----------------------------------------------------------------------------
}
