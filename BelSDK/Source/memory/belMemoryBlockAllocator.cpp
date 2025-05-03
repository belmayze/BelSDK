/*!
 * @file   belMemoryBlockAllocator.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "memory/belMemoryBlockAllocator.h"

namespace bel {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
void MemoryBlockAllocator::initialize(size_t size, size_t max_blcok)
{
    // 確保
    mFreeList.allocate(max_blcok);
    mUseList.allocate(max_blcok);

    // 全体のメモリーブロックをフリーリストに入れる
    mFreeList.push_back({ size, 0 });
}
//-----------------------------------------------------------------------------
// allocator
//-----------------------------------------------------------------------------
std::optional<size_t> MemoryBlockAllocator::allocate(size_t size, size_t alignment)
{
    // 確保されたメモリーブロック
    MemoryBlock* allocated_block = nullptr;

    // フリーリストから確保できるメモリーブロックを取得
    for ( MemoryBlock& block : mFreeList )
    {
        // アライメントチェック
        size_t offset = block.offset - Math::Roundup(block.offset, alignment);
        size += offset;

        // 確保できる場合はメモリーブロックを分割する
        if (block.size == size)
        {
            // 一致した場合はそのまま入れる
        }
        if (block.size > size)
        {
            // メモリーブロックを
        }
    }

    return std::nullopt;
}
//-----------------------------------------------------------------------------
void MemoryBlockAllocator::free(size_t offset)
{

}
//-----------------------------------------------------------------------------
} // bel::
