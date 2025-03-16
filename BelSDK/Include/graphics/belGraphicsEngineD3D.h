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
namespace bel::gfx { class TextureCopyQueue; }
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
     * メインキューのコマンド実行完了を待機する
     */
    virtual void waitCommandQueue() override;

    /*!
     * 更新
     */
    virtual void update() override;

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

    //! デフォルトレンダーターゲット取得
    gfx::RenderTarget& getDefaultRenderTarget() const;

    //! デフォルトレンダーバッファー取得
    gfx::RenderBuffer& getDefaultRenderBuffer() const;

    //! テクスチャーコピーキューを取得
    gfx::TextureCopyQueue& getTextureCopyQueue() { BEL_ASSERT(mpTextureCopyQueue.get()); return *mpTextureCopyQueue.get(); }

    //! タイムスタンプ用の周波数を取得
    uint64_t getTimestampFrequency() const;

    //! trueの場合、HDR出力されている
    bool isHDREnabled() const { return mIsSupportedHDR; }

    //! HDR Paper White の情報を取得
    float getHDRPaperWhite() const { return mHDRPaperWhite; }

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
    //! デスクリプターハンドルの最大数
    static constexpr uint32_t cMaxDynamicDescriptorHandle = 8192;
    //! ダイナミックテクスチャーリソースサイズ
    static constexpr size_t cMaxDynamicTextureResourceSize = 10 * 1024 * 1024;

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
    Microsoft::WRL::ComPtr<ID3D12Device6>   mpDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> mpSwapChain;
    Microsoft::WRL::ComPtr<ID3D12Fence>     mpFence;
    std::unique_ptr<gfx::CommandQueue>      mpMainCommandQueue;
    std::unique_ptr<gfx::CommandList[]>     mpMainCommandLists;
    std::unique_ptr<gfx::TextureCopyQueue>  mpTextureCopyQueue;
    std::unique_ptr<gfx::Texture[]>         mSwapChainTextures;
    std::unique_ptr<gfx::RenderTarget[]>    mSwapChainRenderTargets;
    std::unique_ptr<gfx::RenderBuffer[]>    mSwapChainRenderBuffers;
    uint32_t                                mSwapChainBufferIndex = 0;
    uint32_t                                mCommandBufferIndex   = 0;
    HANDLE                                  mWaitFenceHandle = HANDLE();

    bool  mIsSupportedHDR = false;
    bool  mHDRPathInfoEnabled = false;
    DISPLAYCONFIG_PATH_INFO mHDRPathInfo = {};
    float mHDRPaperWhite  = 80.f;

    //-------------------------------------------------------------------------
private:
    /*!
     * ペーパーホワイトの情報を取得
     */
    void updateHDRPaperWhite_();
};
//-----------------------------------------------------------------------------
}
