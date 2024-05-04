/*!
 * @file   belGraphicsEngineD3D.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/belGraphicsEngine.h"
#include "memory/belSingleton.h"

namespace bel::gfx { class CommandList; }
namespace bel::gfx { class CommandQueue; }

namespace bel {
//-----------------------------------------------------------------------------
// アプリケーションのウィンドウ処理を行います
//-----------------------------------------------------------------------------
class GraphicsEngineD3D : public IGraphicsEngine,
                          public Singleton<GraphicsEngineD3D>
{
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    GraphicsEngineD3D();

    //-------------------------------------------------------------------------
public:
    /*!
     * グラフィックスを初期化します
     */
    virtual bool initialize() override;

    /*!
     * メインキューのコマンドを実行します
     */
    virtual void executeCommand() override;

    /*!
     * メインキューのコマンド実行完了を待機する
     */
    virtual void waitCommandQueue() override;

    /*!
     * 画面を反映する
     */
    virtual void present() override;

    /*!
     * グラフィックスを終了処理します
     */
    virtual void finalize() override;

    //-------------------------------------------------------------------------
public:
    //! デバイス取得
    ID3D12Device6& getDevice() { BEL_ASSERT(mpDevice.Get()); return *mpDevice.Get(); }

    //-------------------------------------------------------------------------
    // パラメーター
    // @note 最終的には外部から指定できるようにする
    //-------------------------------------------------------------------------
private:
    //! バッファー数
    static constexpr uint32_t cNumBuffer = 3;
    //! テクスチャーハンドルの最大数
    static constexpr uint32_t cMaxTextureHandle = 1024;
    //! CBV の最大数
    static constexpr uint32_t cMaxDescriptorCBV = 16;
    //! SRV の最大数
    static constexpr uint32_t cMaxDescriptorSRV = 48;
    //! UAV の最大数
    static constexpr uint32_t cMaxDescriptorUAV = 16;
    //! Sampler の最大数
    static constexpr uint32_t cMaxDescriptorSampler = 16;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12Device6>       mpDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>     mpSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Fence>         mpFence;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpGraphicsRootSignature;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpComputeRootSignature;
    std::unique_ptr<gfx::CommandQueue>          mpMainCommandQueue;
    std::unique_ptr<gfx::CommandList>           mpMainCommandList;
    HANDLE                                      mWaitFenceHandle = HANDLE();

    //-------------------------------------------------------------------------
    // internal
    //-------------------------------------------------------------------------
private:
    /*!
     * 共通のシグネチャを作る
     */
    bool createRootSignature_();

};
//-----------------------------------------------------------------------------
}
