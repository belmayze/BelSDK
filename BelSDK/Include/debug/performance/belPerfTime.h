/*!
 * @file   belPerfTime.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "memory/belSingleton.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
class PerfTime : public Singleton<PerfTime>
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     */
    void initialize();

    //-------------------------------------------------------------------------
    // process
    //-------------------------------------------------------------------------
public:
    /*!
     * リセットします
     */
    void reset();

    /*!
     * 処理の開始を記録します
     * @param[in] name 処理名
     */
    void start(const std::string& name);

    /*!
     * 処理の終了を記録します
     */
    void end();

    //-------------------------------------------------------------------------
private:
    //! スレッドコンテキスト
    struct ThreadContext
    {
        // 入れ子にできるようにする
        std::chrono::system_clock::time_point start;
        uint32_t microsec = 0;
    };

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<ThreadContext[]> mThreadContexts;
};
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
