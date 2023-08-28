/*!
 * @file   belCommandList.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <d3d12.h>
#include <wrl/client.h>

namespace bel::math { class Color; }

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * コマンドリスト
 */
class CommandList
{
    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] type キューの種類
     */
    bool initialize(D3D12_COMMAND_LIST_TYPE type);

    /*!
     * コマンドの記録を開始します
     */
    void begin();

    /*!
     * コマンドの記録を終了します
     */
    void end();

    //-------------------------------------------------------------------------
    // clear
    //-------------------------------------------------------------------------
public:
    /*!
     * レンダーターゲットをクリアします
     * @param[in] descriptor_handle
     * @param[in] color
     */
    void clearColor(D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle, const math::Color& color);

    //-------------------------------------------------------------------------
    // resource barrier
    //-------------------------------------------------------------------------
public:
    /*!
     * リソース状態変化のバリアを入れます
     */
    void resourceBarrierTransition(ID3D12Resource* p_resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //
    ID3D12GraphicsCommandList& getCommandList() const { BEL_ASSERT(mpCommandList); return *mpCommandList.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    mpAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpCommandList;
};
//-----------------------------------------------------------------------------

}
