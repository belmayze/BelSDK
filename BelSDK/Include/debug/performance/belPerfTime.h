/*!
 * @file   belPerfTime.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// std
#include <chrono>
// bel
#include "debug/text/belDebugTextRender.h"
#include "memory/belSingleton.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
class PerfTime : public Singleton<PerfTime>
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] render_texture_format デバッグ描画先のフォーマット
     */
    bool initialize(gfx::TextureFormat render_texture_format);

    /*!
     * 破棄
     */
    void finalize();

    //-------------------------------------------------------------------------
    // main
    //-------------------------------------------------------------------------
public:
    /*!
     * CPU メインスレッド処理の計測
     */
    void startMainCPU();
    void endMainCPU();

    /*!
     * GPU メインスレッド処理の計測
     * @param[in] command コマンド
     */
    void startMainGPU(gfx::CommandContext& command);
    void endMainGPU(gfx::CommandContext& command);

    /*!
     * GPU のタイムスタンプを集計する
     */
    void resolveGPUTimestamp();

    //-------------------------------------------------------------------------
    // process
    //-------------------------------------------------------------------------
public:
    /*!
     * バッファー切り替え
     */
    void swapBuffer();

    //-------------------------------------------------------------------------
    // draw
    //-------------------------------------------------------------------------
public:
    /*!
     * 描画
     * @param[in] command コマンド
     */
    void drawDebugText(gfx::CommandContext& command) const;

    //-------------------------------------------------------------------------
private:
    //! メインスレッド処理
    struct MainThreadContext
    {
        std::chrono::system_clock::time_point cpu_start;
        uint32_t cpu_microsec = 0;

        uint32_t gpu_microsec = 0;
    };

    //! バッファー
    struct BufferContext
    {
        MainThreadContext main_thread_context;
        std::chrono::system_clock::time_point base_time;
    };
    using BufferContexts = std::array<BufferContext, 2>;

    //-------------------------------------------------------------------------
private:
    bool           mInitialized = false;
    uint32_t       mBufferIndex = 0;
    BufferContexts mBufferContexts;
    TextRender     mTextRender;

    // クエリとそのリソース
    Microsoft::WRL::ComPtr<ID3D12QueryHeap> mpQuery;
    Microsoft::WRL::ComPtr<ID3D12Resource>  mpQueryResource;
};
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
