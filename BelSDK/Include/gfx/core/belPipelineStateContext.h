/*!
 * @file   belPipelineStateContext.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
 // C++
#include <array>
#define NOMINMAX
#include <d3d12.h>

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * パイプラインステート列挙
 */

//-----------------------------------------------------------------------------
/*!
 * パイプラインステート
 */
class PipelineStateContext
{
    //-------------------------------------------------------------------------
    // 定数
    //-------------------------------------------------------------------------
public:
    static constexpr uint32_t cMaxRenderTarget = 8;

    //-------------------------------------------------------------------------
    // enum
    //-------------------------------------------------------------------------
public:
    //! デプス比較式
    enum DepthFunc
    {
        cDepthFuncNever        = D3D12_COMPARISON_FUNC_NEVER,
        cDepthFuncLess         = D3D12_COMPARISON_FUNC_LESS,
        cDepthFuncEqual        = D3D12_COMPARISON_FUNC_EQUAL,
        cDepthFuncLessEqual    = D3D12_COMPARISON_FUNC_LESS_EQUAL,
        cDepthFuncGreater      = D3D12_COMPARISON_FUNC_GREATER,
        cDepthFuncNotEqual     = D3D12_COMPARISON_FUNC_NOT_EQUAL,
        cDepthFuncGreaterEqual = D3D12_COMPARISON_FUNC_GREATER_EQUAL,
        cDepthFuncAlways       = D3D12_COMPARISON_FUNC_ALWAYS
    };
    //! ブレンド値
    enum BlendFactor
    {
        cBlendFactorZero         = D3D12_BLEND_ZERO,
        cBlendFactorOne          = D3D12_BLEND_ONE,
        cBlendFactorSrcColor     = D3D12_BLEND_SRC_COLOR,
        cBlendFactorInvSrcColor  = D3D12_BLEND_INV_SRC_COLOR,
        cBlendFactorSrcAlpha     = D3D12_BLEND_SRC_ALPHA,
        cBlendFactorInvSrcAlpha  = D3D12_BLEND_INV_SRC_ALPHA,
        cBlendFactorDestAlpha    = D3D12_BLEND_DEST_ALPHA,
        cBlendFactorInvDestAlpha = D3D12_BLEND_INV_DEST_ALPHA,
        cBlendFactorDestColor    = D3D12_BLEND_DEST_COLOR,
        cBlendFactorInvDestColor = D3D12_BLEND_INV_DEST_COLOR
    };
    //! ブレンド式
    enum BlendOp
    {
        cBlendOpAdd         = D3D12_BLEND_OP_ADD,
        cBlendOpSubtract    = D3D12_BLEND_OP_SUBTRACT,
        cBlendOpRevSubtract = D3D12_BLEND_OP_REV_SUBTRACT,
        cBlendOpMin         = D3D12_BLEND_OP_MIN,
        cBlendOpMax         = D3D12_BLEND_OP_MAX
    };

    //-------------------------------------------------------------------------
public:
    //! ブレンド設定
    struct RenderTargetBlendDesc
    {
        bool        mEnable    = false;
        uint8_t     mWriteMask = 0xF;
        BlendFactor mSrcColor  = cBlendFactorSrcAlpha;
        BlendFactor mDstColor  = cBlendFactorInvSrcAlpha;
        BlendOp     mColorOp   = cBlendOpAdd;
        BlendFactor mSrcAlpha  = cBlendFactorOne;
        BlendFactor mDstAlpha  = cBlendFactorOne;
        BlendOp     mAlphaOp   = cBlendOpAdd;
    };
    struct BlendDesc
    {
        std::array<RenderTargetBlendDesc, cMaxRenderTarget> mRenderTargets;
    };

    //! デプス設定
    struct DepthStencilDesc
    {
        bool      mEnableDepth   = false;
        bool      mDepthWrite    = false;
        DepthFunc mDepthFunc     = cDepthFuncLess;
        bool      mEnableStencil = false;
    };

    //-------------------------------------------------------------------------
    // Mask
    //-------------------------------------------------------------------------
public:
    /*!
     * 出力カラーマスク
     * @param[in] r     trueの場合、Rチャンネルの出力を行います
     * @param[in] g     trueの場合、Gチャンネルの出力を行います
     * @param[in] b     trueの場合、Bチャンネルの出力を行います
     * @param[in] a     trueの場合、Aチャンネルの出力を行います
     * @param[in] index レンダリングターゲット番号
     */
    void setColorMask(bool r, bool g, bool b, bool a, uint32_t index);

    //-------------------------------------------------------------------------
    // Depth
    //-------------------------------------------------------------------------
public:
    /*!
     * デプステストおよびデプス書き込み
     * @param[in] enable trueの場合、デプステスト有効
     * @param[in] write  trueの場合、デプス書き込み有効
     */
    void setDepthTest(bool enable, bool write);

    /*!
     * デプステストの評価方法変更
     * @param[in] func 計算式
     */
    void setDepthFunc(DepthFunc func);

    //-------------------------------------------------------------------------
    // Blend
    //-------------------------------------------------------------------------
public:
    /*!
     * ブレンドの有効設定
     * @param[in] enable trueの場合、ブレンド有効
     * @param[in] index  レンダリングターゲット番号
     */
    void setBlendEnable(bool enable, uint32_t index);

    /*!
     * ブレンド計算式
     * @param[in] src_color
     * @param[in] dst_color
     * @param[in] color_op
     * @param[in] src_alpha
     * @param[in] dst_alpha
     * @param[in] alpha_op
     * @param[in] index  レンダリングターゲット番号
     */
    void setBlendFunc(
        BlendFactor src_color, BlendFactor dst_color, BlendOp color_op,
        BlendFactor src_alpha, BlendFactor dst_alpha, BlendOp alpha_op,
        uint32_t index
    );

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! BlendDesc
    const BlendDesc& getBlendDesc() const { return mBlendDesc; }
    //! DepthStencilDesc
    const DepthStencilDesc& getDepthStencilDesc() const { return mDepthStencilDesc; }

    //-------------------------------------------------------------------------
private:
    BlendDesc        mBlendDesc;
    DepthStencilDesc mDepthStencilDesc;
};
//-----------------------------------------------------------------------------

}
