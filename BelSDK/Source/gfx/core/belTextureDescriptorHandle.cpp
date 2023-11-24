/*!
 * @file   belTextureDescriptorHandle.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belTextureDescriptorHandle.h"
#include "gfx/core/belTextureDescriptorRegistry.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
TextureDescriptorHandle::~TextureDescriptorHandle()
{
    release();
}
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
