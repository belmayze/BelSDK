/*!
 * @file   belGraphicsTextureDescriptorHandle.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/internal/belGraphicsTextureDescriptorHandle.h"
#include "graphics/internal/belGraphicsTextureDescriptorRegistry.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TextureDescriptorHandle::~TextureDescriptorHandle() { release(); }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TextureDescriptorHandle::TextureDescriptorHandle(TextureDescriptorHandle&& handle) noexcept
{
    release();
    if (handle.hasValue())
    {
        mId = handle.getId();
        handle.mId.reset();
    }
}
//-----------------------------------------------------------------------------
TextureDescriptorHandle& TextureDescriptorHandle::operator=(TextureDescriptorHandle&& rhs) noexcept
{
    release();
    if (rhs.hasValue())
    {
        mId = rhs.getId();
        rhs.mId.reset();
    }
    return *this;
}
//-----------------------------------------------------------------------------
void TextureDescriptorHandle::release()
{
    if (mId.has_value())
    {
        TextureDescriptorRegistry::EraseAccessor::Erase(mId.value());
        mId.reset();
    }
}
//-----------------------------------------------------------------------------
}