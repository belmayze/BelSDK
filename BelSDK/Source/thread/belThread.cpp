/*!
 * @file   belThread.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "thread/belThread.h"

namespace bel {
//-----------------------------------------------------------------------------
// static
//-----------------------------------------------------------------------------
uint32_t Thread::GetNumThreads()
{
    return std::thread::hardware_concurrency();
}
//-----------------------------------------------------------------------------
uint32_t Thread::GetThreadIndex()
{
    // @TODO:
    return 0;
}
//-----------------------------------------------------------------------------
// ctor / dtor
//-----------------------------------------------------------------------------
Thread::Thread(const std::string& name, Callback callback)
    :mName(name), mCallback(callback) {}
//-----------------------------------------------------------------------------
Thread::~Thread() { if (mpThread) { join(); } }
//-----------------------------------------------------------------------------
// thread
//-----------------------------------------------------------------------------
void Thread::start()
{
    mpThread = std::make_unique<std::thread>(&Thread::onInvoke_, this);
    setAffinityMaskImpl_();

    // スレッド名を設定する
#   if BEL_PLATFORM_IS_WIN()
    {
        // スレッド名の変換
        size_t strlen = std::strlen(mName.c_str()) + 1; // len + null
        std::unique_ptr<TCHAR[]> str = std::make_unique<TCHAR[]>(strlen);
        size_t ret = 0;
#   ifdef UNICODE
        mbstowcs_s(&ret, str.get(), strlen, mName.c_str(), strlen);
#   else
        strcpy_s(str.get(), strlen, mName.c_str());
#   endif // UNICODE
        // 設定
        SetThreadDescription(mpThread->native_handle(), str.get());
    }
#   else
#       error
#   endif
}
//-----------------------------------------------------------------------------
void Thread::setAffinityMask(uint64_t mask)
{
    mAffinityMask = mask;
    if (mpThread)
    {
        setAffinityMaskImpl_();
    }
}
//-----------------------------------------------------------------------------
void Thread::join()
{
    BEL_ASSERT(mpThread.get() != nullptr);
    mpThread->join();
    mpThread.reset();
}
//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
void Thread::setAffinityMaskImpl_()
{
    BEL_ASSERT(mpThread.get() != nullptr);
    if (mAffinityMask == std::numeric_limits<uint64_t>::max()) { return; }

#   if BEL_PLATFORM_IS_WIN()
    SetThreadAffinityMask(mpThread->native_handle(), mAffinityMask);
#   else
#       error
#   endif // BEL_PLATFORM_IS_WIN()
}
//-----------------------------------------------------------------------------
void Thread::onInvoke_()
{
    int exit_code = mCallback(*this);
    if (exit_code != 0)
    {
#       if BEL_PLATFORM_IS_WIN()
        _endthreadex(exit_code);
#       else
#           error
#       endif // BEL_PLATFORM_IS_WIN()
    }
}
//-----------------------------------------------------------------------------
}
