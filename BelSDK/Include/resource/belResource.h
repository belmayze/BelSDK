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
    Resource(std::unique_ptr<uint8_t[]>&& p_buffer, size_t size)
        : mpBuffer(std::move(p_buffer)), mSize(size) {}

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

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<uint8_t[]> mpBuffer;
    size_t                     mSize = 0;
};
//-----------------------------------------------------------------------------
} // bel::res::
