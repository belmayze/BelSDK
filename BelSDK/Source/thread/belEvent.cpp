/*!
 * @file   belEvent.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "thread/belEvent.h"

namespace bel {
//-----------------------------------------------------------------------------
// ctor / dtor
//-----------------------------------------------------------------------------
Event::Event()
{
#   if BEL_PLATFORM_IS_WIN()
    mpHandle = CreateEvent(0, TRUE, FALSE, 0);
#   else
#       error
#   endif // BEL_PLATFORM_IS_WIN()
}
//-----------------------------------------------------------------------------
Event::~Event()
{
#   if BEL_PLATFORM_IS_WIN()
    CloseHandle(mpHandle);
#   else
#       error
#   endif // BEL_PLATFORM_IS_WIN()
}
//-----------------------------------------------------------------------------
// event
//-----------------------------------------------------------------------------
bool Event::wait()
{
#   if BEL_PLATFORM_IS_WIN()
    return WaitForSingleObject(mpHandle, INFINITE) == WAIT_OBJECT_0;
#   else
#       error
#   endif // BEL_PLATFORM_IS_WIN()
}
//-----------------------------------------------------------------------------
bool Event::signal()
{
#   if BEL_PLATFORM_IS_WIN()
    return SetEvent(mpHandle);
#   else
#       error
#   endif // BEL_PLATFORM_IS_WIN()
}
//-----------------------------------------------------------------------------
bool Event::reset()
{
#   if BEL_PLATFORM_IS_WIN()
    return ResetEvent(mpHandle);
#   else
#       error
#   endif // BEL_PLATFORM_IS_WIN()
}
//-----------------------------------------------------------------------------
}
