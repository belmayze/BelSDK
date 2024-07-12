/*!
 * @file   belGraphicsGlobalDescriptorHandle.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/internal/belGraphicsGlobalDescriptorHandle.h"
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
GlobalDescriptorHandle::~GlobalDescriptorHandle() { release(); }
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
GlobalDescriptorHandle::GlobalDescriptorHandle(GlobalDescriptorHandle&& handle) noexcept
{
    release();
    if (handle.hasValue())
    {
        mId = handle.getId();
        handle.mId.reset();
    }
}
//-----------------------------------------------------------------------------
GlobalDescriptorHandle& GlobalDescriptorHandle::operator=(GlobalDescriptorHandle&& rhs) noexcept
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
void GlobalDescriptorHandle::release()
{
    if (mId.has_value())
    {
        GlobalDescriptorRegistry::EraseAccessor::Erase(mId.value());
        mId.reset();
    }
}
//-----------------------------------------------------------------------------
}