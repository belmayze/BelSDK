/*!
 * @file   belResource.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::res {
//-----------------------------------------------------------------------------
class Resource
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    Resource() {}
    
    //! リソース初期化
    Resource(std::unique_ptr<uint8_t[]>&& p_buffer, size_t size)
        : mpBuffer(std::move(p_buffer)), mSize(size) {}

    //! コピー禁止
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    //! move
    Resource(Resource&& rhs) noexcept;
    Resource& operator=(Resource&& rhs) noexcept;

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    /*!
     * バッファーを取得する
     */
    const void* getBuffer() const { return mpBuffer.get(); }

    /*!
     * サイズを取得する
     */
    size_t getSize() const { return mSize; }

    /*!
     * 有効判定
     */
    bool isValid() const { return mSize != 0; }

    //-------------------------------------------------------------------------
    // operator
    //-------------------------------------------------------------------------
public:
    //! bool オペレーター
    operator bool() const { return isValid(); }

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<uint8_t[]> mpBuffer;
    size_t                     mSize = 0;
};
//-----------------------------------------------------------------------------
} // bel::res::
