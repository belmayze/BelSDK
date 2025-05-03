/*!
 * @file   belMemoryBlockAllocator.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel
{
//-----------------------------------------------------------------------------
/*!
 * メモリーブロック割り当て
 */
class MemoryBlockAllocator
{
    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * サイズを指定してメモリーブロックの領域を指定します
     * @param[in] size      サイズ
     * @param[in] max_block 最大ブロック数
     */
    void initialize(size_t size, size_t max_blcok = 1024);

    //-------------------------------------------------------------------------
    // allocator
    //-------------------------------------------------------------------------
public:
    /*!
     * メモリーブロック割り当て
     * @param[in] size      サイズ
     * @param[in] alignment アライメント
     * @return オフセットを返します
     */
    std::optional<size_t> allocate(size_t size, size_t alignment);

    /*!
     * メモリーブロック破棄
     * @param[in] offset オフセット
     */
    void free(size_t offset);

    //-------------------------------------------------------------------------
    // internal struct
    //-------------------------------------------------------------------------
private:
    //! メモリーブロック
    struct MemoryBlock
    {
        size_t size = 0;
        size_t offset = 0;
        size_t aligned_offset = 0;

        MemoryBlock(size_t size, size_t offset, size_t aligned_offset)
            : size(size), offset(offset), aligned_offset(aligned_offset) {
        }
    };

    //-------------------------------------------------------------------------
    // メンバー変数
    //-------------------------------------------------------------------------
private:
    List<MemoryBlock> mFreeList;
    List<MemoryBlock> mUseList;
};
//-----------------------------------------------------------------------------
}
