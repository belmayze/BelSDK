/*!
 * @file   belPerfTime.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "debug/performance/belPerfTime.h"
#include "graphics/common/belGraphicsCommandContext.h"
#include "graphics/belGraphicsEngine.h"
#include "thread/belThread.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
bool PerfTime::initialize(gfx::TextureFormat render_texture_format)
{
    BEL_DEBUG_LOG("[Bel][PerfTime] num of threads [%2d]\n", Thread::GetNumThreads());

    // 文字描画初期化
    mTextRender.initialize(1024, render_texture_format);

    // クエリ初期化
    {
        D3D12_QUERY_HEAP_DESC query_desc = {};
        query_desc.Type  = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
        query_desc.Count = 2 * 2; // (Begin + End) * 2 frames
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateQueryHeap(
            &query_desc, IID_PPV_ARGS(&mpQuery)
        )))
        {
            return false;
        }

        D3D12_RESOURCE_DESC resource_desc = {};
        resource_desc.SampleDesc.Count = 1;
        resource_desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
        resource_desc.Width            = sizeof(uint64_t) * query_desc.Count;
        resource_desc.Height           = 1;
        resource_desc.MipLevels        = 1;
        resource_desc.DepthOrArraySize = 1;
        resource_desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        D3D12_HEAP_PROPERTIES heap_props = {};
        heap_props.Type = D3D12_HEAP_TYPE_READBACK;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &heap_props, D3D12_HEAP_FLAG_NONE,
            &resource_desc, D3D12_RESOURCE_STATE_COPY_DEST,
            nullptr, IID_PPV_ARGS(&mpQueryResource))))
        {
            return false;
        }
    }

    // 初期化完了フラグ
    mInitialized = true;

    return true;
}
//-----------------------------------------------------------------------------
void PerfTime::finalize()
{
    mTextRender = TextRender();
}
//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
void PerfTime::startMainCPU()
{
    if (!mInitialized) { return; }

    // 計測開始
    mBufferContexts[mBufferIndex].main_thread_context.cpu_start = std::chrono::system_clock::now();
}
//-----------------------------------------------------------------------------
void PerfTime::endMainCPU()
{
    if (!mInitialized) { return; }

    // 計測終了
    MainThreadContext& context = mBufferContexts[mBufferIndex].main_thread_context;

    context.cpu_microsec =
        static_cast<uint32_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now() - context.cpu_start
            ).count()
        );
}
//-----------------------------------------------------------------------------
void PerfTime::startMainGPU(gfx::CommandContext& command)
{
    if (!mInitialized) { return; }

    // 開始タイムスタンプ記録
    uint32_t base_offset = mBufferIndex * 2;
    command.getCommandList().EndQuery(mpQuery.Get(), D3D12_QUERY_TYPE_TIMESTAMP, base_offset);
}
//-----------------------------------------------------------------------------
void PerfTime::endMainGPU(gfx::CommandContext& command)
{
    if (!mInitialized) { return; }

    // 終了タイムスタンプ記録
    uint32_t base_offset = mBufferIndex * 2;
    command.getCommandList().EndQuery(mpQuery.Get(), D3D12_QUERY_TYPE_TIMESTAMP, base_offset + 1);

    // データ解決
    command.getCommandList().ResolveQueryData(mpQuery.Get(), D3D12_QUERY_TYPE_TIMESTAMP, base_offset, 2, mpQueryResource.Get(), 0);
}
//-----------------------------------------------------------------------------
void PerfTime::resolveGPUTimestamp()
{
    if (!mInitialized) { return; }

    // GPU の周波数を取得
    uint64_t gpu_frequency = GraphicsEngine::GetInstance().getTimestampFrequency();

    // バックバッファーの処理は終わっているのでデータを読み込む
    uint32_t buffer_index = 1 - mBufferIndex;
    uint32_t base_offset = buffer_index * 2;
    BufferContext& context = mBufferContexts[buffer_index];

    //
    D3D12_RANGE range = {};
    range.Begin = sizeof(uint64_t) * base_offset;
    range.End   = sizeof(uint64_t) * (base_offset + 1);

    void* ptr;
    mpQueryResource->Map(0, &range, &ptr);
    uint64_t* data = reinterpret_cast<uint64_t*>(ptr);
    if (data)
    {
        context.main_thread_context.gpu_microsec = static_cast<uint32_t>((data[1] - data[0]) * 1000.f * 1000.f / gpu_frequency);
    }
    mpQueryResource->Unmap(0, nullptr);

    // 文字列生成
    {
        // @todo: format で初期化できる文字列クラスがほしい
        char str[1024];
        snprintf(str, 1024, "CPU:%6.2f%%\nGPU:%6.2f%%", context.main_thread_context.cpu_microsec / 166.6666f, context.main_thread_context.gpu_microsec / 166.6666f);

        mTextRender.calcText(str, Vector2(0.f, 720.f - 32.f), 16);
    }
}
//-----------------------------------------------------------------------------
// process
//-----------------------------------------------------------------------------
void PerfTime::swapBuffer()
{
    if (!mInitialized) { return; }

    mBufferIndex = 1 - mBufferIndex;

    // ここを起点の時間とする
    mBufferContexts[mBufferIndex].base_time = std::chrono::system_clock::now();
}
//-----------------------------------------------------------------------------
// draw
//-----------------------------------------------------------------------------
void PerfTime::drawDebugText(gfx::CommandContext& command) const
{
    if (!mInitialized) { return; }

    // 計測結果を出力
    mTextRender.draw(command);
}
//-----------------------------------------------------------------------------
}
