﻿/*!
 * @file   belGraphics.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace bel
{

//-----------------------------------------------------------------------------
/*!
 * プラットフォームの内部処理を行います
 */
class Graphics : public Singleton<Graphics>
{
public:
    /*!
     * 初期化
     */
    bool initialize();

    /*!
     * コマンドの実行待ち
     */
    void waitToExecuteCommand();

    /*!
     * コマンドの実行
     */
    void executeCommand();

    /*!
     * 画面の反映
     */
    void present();

    /*!
     * 終了処理
     */
    void finalize();

private:
    uint32_t mNumBuffer          = 2;
    uint32_t mCurrentBufferIndex = 0;

    Microsoft::WRL::ComPtr<ID3D12Device6>      mpDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>    mpSwapChain;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;

    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12Resource>[]> mpColorBuffers;
    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>[]> mpRenderTargetDescriptorHeaps;

    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12Fence>[]>    mpFences;
    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>[]>    mpCommandAllocators;
    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>[]> mpCommandLists;

private:
    friend class Singleton<Graphics>;
    Graphics() {};
};
//-----------------------------------------------------------------------------

}