/*!
 * @file   belGraphicsConstantBuffer.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx {
//-----------------------------------------------------------------------------
// 定数バッファー
//-----------------------------------------------------------------------------
class ConstantBuffer
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        size_t   mBufferSize = 0;
        uint32_t mNumBuffer  = 1;
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * メッシュを初期化します
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    /*!
     * バッファー切り替え
     */
    void swapBuffer();

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12Resource>[]> mpResources;
    std::unique_ptr<uint8_t* []> mBufferPtrs;
    size_t                       mBufferSize = 0;
    uint32_t                     mNumBuffer   = 1;
    uint32_t                     mBufferIndex = 0;
};
//-----------------------------------------------------------------------------
}
