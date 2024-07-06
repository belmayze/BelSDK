/*!
 * @file   belGraphicsEngineD3D.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"
#include "graphics/belGraphicsEngine.h"
#include "memory/belSingleton.h"

namespace bel::gfx { class CommandList; }
namespace bel::gfx { class CommandQueue; }
namespace bel::gfx { class RenderTarget; }
namespace bel::gfx { class RenderBuffer; }
namespace bel::gfx { class Texture; }
namespace bel { class Application; }

namespace bel {
//-----------------------------------------------------------------------------
// アプリケーションのウィンドウ処理を行います
//-----------------------------------------------------------------------------
class GraphicsEngineD3D : public IGraphicsEngine,
                          public Singleton<GraphicsEngineD3D>
{
    //-------------------------------------------------------------------------
public:
    //! CBV の最大数
    static constexpr uint32_t cMaxDescriptorCBV = 16;
    //! SRV の最大数
    static constexpr uint32_t cMaxDescriptorSRV = 48;
    //! UAV の最大数
    static constexpr uint32_t cMaxDescriptorUAV = 16;
    //! Sampler の最大数
    static constexpr uint32_t cMaxDescriptorSampler = 16;

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
     * コマンド生成前に必要なコマンドを積みます
     * @param[in] command コマンド
     */
    virtual void makeInitialCommand(gfx::CommandContext& command) const override;

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

    //! 共有のルート証明取得
    ID3D12RootSignature& getCommonGraphicsRootSignature() { BEL_ASSERT(mpGraphicsRootSignature.Get()); return *mpGraphicsRootSignature.Get(); }
    ID3D12RootSignature& getCommonComputeRootSignature()  { BEL_ASSERT(mpComputeRootSignature.Get());  return *mpComputeRootSignature.Get();  }

    //! デフォルトレンダーターゲット取得
    gfx::RenderTarget& getDefaultRenderTarget() const;

    //! デフォルトレンダーバッファー取得
    gfx::RenderBuffer& getDefaultRenderBuffer() const;

    //-------------------------------------------------------------------------
public:
    //! アプリケーションクラスから取得できる関数
    class ApplicationAccessor
    {
        friend Application;
    private:
        gfx::CommandQueue& getMainCommandQueue();
        gfx::CommandList&  getMainCommandList();
    };

    //-------------------------------------------------------------------------
    // パラメーター
    // @note 最終的には外部から指定できるようにする
    //-------------------------------------------------------------------------
private:
    //! フォーマット
    static constexpr gfx::TextureFormat cSDRTextureFormat     = gfx::TextureFormat::cR8G8B8A8_uNorm;
    static constexpr gfx::TextureFormat cSDRsRGBTextureFormat = gfx::TextureFormat::cR8G8B8A8_sRGB;
    static constexpr gfx::TextureFormat cHDRTextureFormat     = gfx::TextureFormat::cR10G10B10A2_uNorm;

    //! バッファー数
    static constexpr uint32_t cNumBuffer = 3;
    //! テクスチャーハンドルの最大数
    static constexpr uint32_t cMaxTextureHandle = 1024;

    //-------------------------------------------------------------------------
private:
    //! 色空間
    struct DisplayChromaticity
    {
        float red_x,   red_y;
        float green_x, green_y;
        float blue_x,  blue_y;
        float white_x, white_y;
    };

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12Device6>       mpDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>     mpSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Fence>         mpFence;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpGraphicsRootSignature;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpComputeRootSignature;
    std::unique_ptr<gfx::CommandQueue>          mpMainCommandQueue;
    std::unique_ptr<gfx::CommandList>           mpMainCommandList;
    std::unique_ptr<gfx::Texture[]>             mSwapChainTextures;
    std::unique_ptr<gfx::RenderTarget[]>        mSwapChainRenderTargets;
    std::unique_ptr<gfx::RenderBuffer[]>        mSwapChainRenderBuffers;
    HANDLE                                      mWaitFenceHandle = HANDLE();

    bool mIsSupportedHDR = false;

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
