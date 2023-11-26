/*!
 * @file   belPipelineState.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
 // C++
#define NOMINMAX
#include <d3d12.h>
#include <wrl/client.h>
// game
#include "gfx/core/belPipelineStateContext.h"

namespace bel::gfx { class Shader; }

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * パイプラインステート
 */
class PipelineState
{
    //-------------------------------------------------------------------------
    // 定数
    //-------------------------------------------------------------------------
public:
    static constexpr uint32_t cMaxRenderTarget    = 8;
    static constexpr uint32_t cMaxVertexAttribute = 16;

    //-------------------------------------------------------------------------
public:
    //! 入力情報コンテキスト
    struct InputContext
    {
        struct InputLayout
        {
            std::string mName                 = "";
            DXGI_FORMAT mFormat               = DXGI_FORMAT_UNKNOWN;
            uint32_t    mSemanticIndex        = 0;
            uint32_t    mSlotIndex            = 0;
            uint32_t    mOffset               = 0;
            uint32_t    mInstanceDataStepRate = 0;
        };
        std::array<InputLayout, cMaxVertexAttribute> mInputLayouts;
        uint32_t                                     mNumInputLayout;
        D3D12_PRIMITIVE_TOPOLOGY_TYPE                mPrimitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
    };
    //! 出力情報コンテキスト
    struct OutputContext
    {
        std::array<DXGI_FORMAT, cMaxRenderTarget> mColorFormats = { DXGI_FORMAT_UNKNOWN };
        DXGI_FORMAT                               mDepthFormat = DXGI_FORMAT_UNKNOWN;
        uint32_t                                  mNumRenderTarget = 1;
    };
    //! 初期化引数
    struct InitializeArg
    {
        //! シェーダー
        const Shader& mShader;
        //! 
        PipelineStateContext mContext;
        InputContext         mInputContext;
        OutputContext        mOutputContext;

        InitializeArg(const Shader& shader) : mShader(shader) {}
    };

    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipelineState;
};
//-----------------------------------------------------------------------------

}
