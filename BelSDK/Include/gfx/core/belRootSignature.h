/*!
 * @file   belRootSignature.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
 // C++
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
    ID3D12RootSignature& getRootSignature() { BEL_ASSERT(mpRootSignature); return *mpRootSignature.Get(); }

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> mpRootSignature;

    //-------------------------------------------------------------------------
private:
    friend class Singleton<RootSignature>;
    RootSignature();
    ~RootSignature();
};
//-----------------------------------------------------------------------------

}
