/*!
 * @file   belGraphicsConstantBuffer.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/internal/belGraphicsGlobalDescriptorHandle.h"

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
        size_t   buffer_size = 0;
        uint32_t num_buffer  = 1;
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

    /*!
     * バッファー転送
     * @param[in] ptr    コピー元バッファー
     * @param[in] size   サイズ
     * @param[in] offset オフセット
     */
    void copyBuffer(void* ptr, size_t size, size_t offset = 0);

    /*!
     * 構造体でバッファー転送
     * @param[in] data 構造体
     */
    template <typename T>
    void copyStruct(T& data, size_t offset = 0) { copyStructs(&data, 1, offset); }

    /*!
     * 複数の構造体でバッファー転送
     * @param[in] data      構造体
     * @param[in] num_array 配列数
     */
    template <typename T>
    void copyStructs(T* data, size_t num_array, size_t offset = 0) { copyBuffer(data, sizeof(T) * num_array, offset); }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
     //! デスクリプターハンドルを取得
    const GlobalDescriptorHandle& getCurrentDescriptorHandle() const { return mDescriptorHandles[mBufferIndex]; }

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<Microsoft::WRL::ComPtr<ID3D12Resource>[]> mpResources;
    std::unique_ptr<uint8_t* []> mBufferPtrs;
    size_t                       mBufferSize = 0;
    uint32_t                     mNumBuffer   = 1;
    uint32_t                     mBufferIndex = 0;

    std::unique_ptr<GlobalDescriptorHandle[]> mDescriptorHandles;
};
//-----------------------------------------------------------------------------
}
