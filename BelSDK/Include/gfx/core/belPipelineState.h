/*!
 * @file   belPipelineState.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
 // C++
#include <d3d12.h>
#include <wrl/client.h>

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
public:
    //! 初期化引数
    struct InitializeArg
    {
        const Shader& mShader;

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
    void initialize(const InitializeArg& arg);

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipelineState;
};
//-----------------------------------------------------------------------------

}
