/*!
 * @file   belPerfTime.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "debug/performance/belPerfTime.h"
#include "thread/belThread.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
void PerfTime::initialize()
{
    // 理論コア数分確保する
    mThreadContexts = std::make_unique<ThreadContext[]>(Thread::GetNumThreads());
    BEL_PRINT("[Bel][PerfTime] num of threads [%2d]\n", Thread::GetNumThreads());
}
//-----------------------------------------------------------------------------
// process
//-----------------------------------------------------------------------------
void PerfTime::reset()
{
    // @TODO
}
//-----------------------------------------------------------------------------
void PerfTime::start(const std::string& name)
{
    // 実行中のスレッド情報を取得
    uint32_t thread_index = Thread::GetThreadIndex();
    ThreadContext& ctx = mThreadContexts[thread_index];
    // 計測開始
    ctx.mStart = std::chrono::system_clock::now();
}
//-----------------------------------------------------------------------------
void PerfTime::end()
{
    // 実行中のスレッド情報を取得
    uint32_t thread_index = Thread::GetThreadIndex();
    ThreadContext& ctx = mThreadContexts[thread_index];
    // 計測終了
    ctx.mMicrosec = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - ctx.mStart).count());

    // @TODO: いったん仮で文字出力する
    BEL_PRINT("%u us. (%.2f fps)\n", ctx.mMicrosec, (1000.f * 1000.f) / static_cast<float>(ctx.mMicrosec));
}
//-----------------------------------------------------------------------------
}
