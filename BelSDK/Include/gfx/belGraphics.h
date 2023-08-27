/*!
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

private:
    Microsoft::WRL::ComPtr<ID3D12Device6>      mpDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain4>    mpSwapChain;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;

private:
    friend class Singleton<Graphics>;
    Graphics() {};
};
//-----------------------------------------------------------------------------

}
