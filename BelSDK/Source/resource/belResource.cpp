/*!
 * @file   belResource.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResource.h"

namespace bel::res {
//-----------------------------------------------------------------------------
// move
//-----------------------------------------------------------------------------
Resource::Resource(Resource&& rhs) noexcept
    : mpBuffer(std::move(rhs.mpBuffer)), mpBufferPtr(rhs.mpBufferPtr), mSize(rhs.mSize)
{
    rhs.mpBufferPtr = nullptr;
    rhs.mSize       = 0;
}
//-----------------------------------------------------------------------------
Resource& Resource::operator=(Resource&& rhs) noexcept
{
    mpBuffer    = std::move(rhs.mpBuffer);
    mpBufferPtr = rhs.mpBufferPtr;
    mSize       = rhs.mSize;
    
    rhs.mpBufferPtr = nullptr;
    rhs.mSize       = 0;
    return *this;
}
//-----------------------------------------------------------------------------
} // bel::res::
