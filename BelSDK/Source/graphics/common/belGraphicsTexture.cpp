/*!
 * @file   belGraphicsTexture.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/internal/belGraphicsTextureDescriptorRegistry.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
void Texture::initializeFromGPUMemory(const InitializeArg& arg, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource)
{
    // GPU メモリーなので記録するだけ
    mWidth     = arg.mWidth;
    mHeight    = arg.mHeight;
    mNumMip    = arg.mNumMip;
    mFormat    = arg.mFormat;
    mDimension = arg.mDimension;
    mpResource = std::move(p_resource);
}
//-----------------------------------------------------------------------------
}