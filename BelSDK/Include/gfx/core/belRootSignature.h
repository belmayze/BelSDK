/*!
 * @file   belRootSignature.h
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
// bel
#include "memory/belSingleton.h"

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * ルート署名
 * @note 基本的には使いまわすため Singleton で確保します
 */
class RootSignature : public Singleton<RootSignature>
{
    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 作成
     */
    bool create();

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! Root signature
    ID3D12RootSignature& getGraphicsRootSignature() { BEL_ASSERT(mpGraphicsRootSignature); return *mpGraphicsRootSignature.Get(); }
    ID3D12RootSignature& getComputeRootSignature() { BEL_ASSERT(mpComputeRootSignature); return *mpComputeRootSignature.Get(); }

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpGraphicsRootSignature;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpComputeRootSignature;

    //-------------------------------------------------------------------------
private:
    friend class Singleton<RootSignature>;
    RootSignature();
    ~RootSignature();
};
//-----------------------------------------------------------------------------

}
