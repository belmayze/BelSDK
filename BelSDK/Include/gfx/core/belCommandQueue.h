/*!
 * @file   belCommandQueue.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#define NOMINMAX
#include <d3d12.h>
#include <wrl/client.h>

namespace bel::gfx { class CommandList; }

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * コマンドキュー
 */
class CommandQueue
{
    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    CommandQueue();
    //! デストラクター
    ~CommandQueue();

    /*!
     * 初期化
     * @param[in] type キューの種類
     */
    bool initialize(D3D12_COMMAND_LIST_TYPE type);

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * コマンド実行
     * @param[in] command 実行するコマンド
     */
    void executeCommand(const CommandList& command);

    //-------------------------------------------------------------------------
    // fence
    //-------------------------------------------------------------------------
public:
    /*!
     * 同期
     * @param[in] p_fence オブジェクト
     * @param[in] value   到達したら書き換える数値
     */
    void signal(ID3D12Fence* p_fence, UINT64 value);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //
    ID3D12CommandQueue& getCommandQueue() const { BEL_ASSERT(mpCommandQueue); return *mpCommandQueue.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Fence>        mpFence;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;
};
//-----------------------------------------------------------------------------

}
