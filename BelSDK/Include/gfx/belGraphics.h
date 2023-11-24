/*!
 * @file   belGraphics.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#define NOMINMAX
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <wrl/client.h>

namespace bel::gfx { class CommandList; }
namespace bel::gfx { class CommandQueue; }
namespace bel::gfx { class RenderTarget; }
namespace bel::gfx { class Texture; }

namespace bel
{

//-----------------------------------------------------------------------------
/*!
 * プラットフォームの内部処理を行います
 */
class Graphics : public Singleton<Graphics>
{
    //-------------------------------------------------------------------------
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

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! D3D12 デバイス
    ID3D12Device6& getDevice() const { BEL_ASSERT(mpDevice); return *mpDevice.Get(); }

    //-------------------------------------------------------------------------
    // パラメーター
    // @todo: 最終的には外部から与えられる定数にする
    //-------------------------------------------------------------------------
public:
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
    uint32_t mNumBuffer          = 2;
    uint32_t mCurrentBufferIndex = 0;

    Microsoft::WRL::ComPtr<ID3D12Device6>      mpDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>    mpSwapChain;

    std::unique_ptr<gfx::Texture[]>      mpColorBuffers;
    std::unique_ptr<gfx::RenderTarget[]> mpRenderTargets;

    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12Fence>[]>    mpFences;

    std::unique_ptr<gfx::CommandList[]> mpCommandLists;
    std::unique_ptr<gfx::CommandQueue>  mpCommandQueue;

private:
    friend class Singleton<Graphics>;
    Graphics();
    ~Graphics();
};
//-----------------------------------------------------------------------------

}
