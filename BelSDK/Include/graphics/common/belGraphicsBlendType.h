/*!
 * @file   belGraphicsBlendType.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx {
//-----------------------------------------------------------------------------
// ブレンドタイプ
//-----------------------------------------------------------------------------
enum class BlendType : std::uint8_t
{
    cZero,
    cOne,
    cSrcColor,
    cInvSrcColor,
    cSrcAlpha,
    cInvSrcAlpha,
    cDestAlpha,
    cInvDestAlpha,
    cDestColor,
    cInvDestColor,
    cSrcAlphaSat,
    cBlendFactor,
    cInvBlendFactor,
    cSrc1Color,
    cInvSrc1Color,
    cSrc1Alpha,
    cInvSrc1Alpha
};

// cast
constexpr D3D12_BLEND to_native(BlendType type)
{
    switch (type)
    {
    case BlendType::cZero:           return D3D12_BLEND_ZERO;
    case BlendType::cOne:            return D3D12_BLEND_ONE;
    case BlendType::cSrcColor:       return D3D12_BLEND_SRC_COLOR;
    case BlendType::cInvSrcColor:    return D3D12_BLEND_INV_SRC_COLOR;
    case BlendType::cSrcAlpha:       return D3D12_BLEND_SRC_ALPHA;
    case BlendType::cInvSrcAlpha:    return D3D12_BLEND_INV_SRC_ALPHA;
    case BlendType::cDestAlpha:      return D3D12_BLEND_DEST_ALPHA;
    case BlendType::cInvDestAlpha:   return D3D12_BLEND_INV_DEST_ALPHA;
    case BlendType::cDestColor:      return D3D12_BLEND_DEST_COLOR;
    case BlendType::cInvDestColor:   return D3D12_BLEND_INV_DEST_COLOR;
    case BlendType::cSrcAlphaSat:    return D3D12_BLEND_SRC_ALPHA_SAT;
    case BlendType::cBlendFactor:    return D3D12_BLEND_BLEND_FACTOR;
    case BlendType::cInvBlendFactor: return D3D12_BLEND_INV_BLEND_FACTOR;
    case BlendType::cSrc1Color:      return D3D12_BLEND_SRC1_COLOR;
    case BlendType::cInvSrc1Color:   return D3D12_BLEND_INV_SRC1_COLOR;
    case BlendType::cSrc1Alpha:      return D3D12_BLEND_SRC1_ALPHA;
    case BlendType::cInvSrc1Alpha:   return D3D12_BLEND_INV_SRC1_ALPHA;
    }

    BEL_ASSERT(0);
    return D3D12_BLEND_ZERO;
}

//-----------------------------------------------------------------------------
// ブレンド計算式
//-----------------------------------------------------------------------------
enum class BlendOpType : std::uint8_t
{
    cAdd,
    cSubtract,
    cRevSubtract,
    cMin,
    cMax
};

// cast
constexpr D3D12_BLEND_OP to_native(BlendOpType type)
{
    switch (type)
    {
    case BlendOpType::cAdd:         return D3D12_BLEND_OP_ADD;
    case BlendOpType::cSubtract:    return D3D12_BLEND_OP_SUBTRACT;
    case BlendOpType::cRevSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
    case BlendOpType::cMin:         return D3D12_BLEND_OP_MIN;
    case BlendOpType::cMax:         return D3D12_BLEND_OP_MAX;
    }

    BEL_ASSERT(0);
    return D3D12_BLEND_OP_ADD;
}

//-----------------------------------------------------------------------------
}
