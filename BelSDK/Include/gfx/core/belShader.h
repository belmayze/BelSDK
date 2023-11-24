/*!
 * @file   belShader.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <d3d12.h>
#include <variant>
#include <wrl/client.h>

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * シェーダー
 */
class Shader
{
    //-------------------------------------------------------------------------
public:
    //! モード
    enum Mode
    {
        cModeInvalid,
        cModeVertexPixel,
        cModeCompute,
        cModeAmplificationMesh
    };

    //-------------------------------------------------------------------------
public:
    //! シェーダーオブジェクト
    struct VertexPixel
    {
        void*  mVertexShaderPtr  = nullptr;
        size_t mVertexShaderSize = 0;
        void*  mPixelShaderPtr   = nullptr;
        size_t mPixelShaderSize  = 0;
    };
    struct Compute
    {
        void*  mComputeShaderPtr  = nullptr;
        size_t mComputeShaderSize = 0;
    };
    struct AmplificationMesh
    {
        void*  mAmplificationShaderPtr  = nullptr;
        size_t mAmplificationShaderSize = 0;
        void*  mMeshShaderPtr           = nullptr;
        size_t mMeshShaderSize          = 0;
    };

    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化 (Vertex + Pixel)
     * @param[in] vs_ptr  VS バイナリ
     * @param[in] vs_size VS バイナリサイズ
     * @param[in] ps_ptr  PS バイナリ
     * @param[in] ps_size PS バイナリサイズ
     */
    void initializeVertexPixel(void* vs_ptr, size_t vs_size, void* ps_ptr, size_t ps_size);

    /*!
     * 初期化（Compute）
     * @param[in] ptr  CS バイナリ
     * @param[in] size CS バイナリサイズ
     */
    void initializeCompute(void* ptr, size_t size);

    /*!
     * 初期化（Amplification + Mesh）
     * @param[in] as_ptr  AS バイナリ
     * @param[in] as_size AS バイナリサイズ
     * @param[in] ms_ptr  MS バイナリ
     * @param[in] ms_size MS バイナリサイズ
     */
    void initializeAmplificationMesh(void* as_ptr, size_t as_size, void* ms_ptr, size_t ms_size);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    // モード取得
    Mode getMode() const
    {
        if (std::holds_alternative<VertexPixel>(mShaderObject)) { return cModeVertexPixel; }
        if (std::holds_alternative<Compute>(mShaderObject)) { return cModeCompute; }
        if (std::holds_alternative<AmplificationMesh>(mShaderObject)) { return cModeAmplificationMesh; }
        return cModeInvalid;
    }

    //
    const VertexPixel& getVertexPixel() const
    {
        BEL_ASSERT(std::holds_alternative<VertexPixel>(mShaderObject));
        return std::get<VertexPixel>(mShaderObject);
    }

    //
    const Compute& getCompute() const
    {
        BEL_ASSERT(std::holds_alternative<Compute>(mShaderObject));
        return std::get<Compute>(mShaderObject);
    }

    //
    const AmplificationMesh& getAmplificationMesh() const
    {
        BEL_ASSERT(std::holds_alternative<AmplificationMesh>(mShaderObject));
        return std::get<AmplificationMesh>(mShaderObject);
    }

private:
    std::variant<VertexPixel, Compute, AmplificationMesh> mShaderObject;
};
//-----------------------------------------------------------------------------

}
