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
    mFreeList.emplace_back(size, 0, 0);
}
//-----------------------------------------------------------------------------
// allocator
//-----------------------------------------------------------------------------
std::optional<size_t> MemoryBlockAllocator::allocate(size_t size, size_t alignment)
{
    // 確保されたメモリーブロック
    MemoryBlock* allocated_block = nullptr;

    // フリーリストから確保できるメモリーブロックを取得
    for (auto it = mFreeList.begin(), end = mFreeList.end(); it != end; ++it)
    {
        MemoryBlock& block = *it;

        // アライメントチェック
        size_t offset = block.offset - Math::Roundup(block.offset, alignment);
        size += offset;

        // 確保可能チェック
        if (block.size == size)
        {
            // 一致した場合はそのまま入れる
            allocated_block = &mUseList.emplace_back(size, block.offset, block.offset + offset);
            mFreeList.erase(it);
            break;
        }
        else if (block.size > size)
        {
            // 空きの方が大きい場合は分割する
            allocated_block = &mUseList.emplace_back(size, block.offset, block.offset + offset);
            //
            block.size -= size;
            block.offset += size;
            block.aligned_offset += size;
            break;
        }
    }

    return allocated_block ? std::optional<size_t>(allocated_block->aligned_offset) : std::nullopt;
}
//-----------------------------------------------------------------------------
void MemoryBlockAllocator::free(size_t offset)
{
    // 同じオフセットのメモリーブロックを検索
    for (auto it = mUseList.begin(), end = mUseList.end(); it != end; ++it)
    {
        if (it->aligned_offset == offset)
        {
            // 連続したメモリーブロックを検索
            auto free_forward_it = mFreeList.end();
            auto free_backward_it = mFreeList.end();

            for (auto free_it = mFreeList.begin(), free_end = mFreeList.end(); free_it != free_end; ++free_it)
            {
                if ((free_it->offset + free_it->size) == it->offset)
                {
                    free_forward_it = free_it;
                }
                else if ((it->offset + it->size) == free_it->offset)
                {
                    free_backward_it = free_it;
                }
            }

            //
            if (free_forward_it != mFreeList.end() && free_backward_it != mFreeList.end())
            {
                // 両方のメモリーブロックに連続する場合は３つつなげる
                free_forward_it->size += it->size;
                free_forward_it->size += free_backward_it->size;

                mFreeList.erase(free_backward_it);
            }
            else if (free_forward_it != mFreeList.end())
            {
                // 前方のメモリーブロックのみ連続する場合は２つつなげる
                free_forward_it->size += it->size;
            }
            else if (free_backward_it != mFreeList.end())
            {
                // 後方のメモリーブロックのみ連続する場合は２つつなげる
                free_backward_it->size += it->size;
                free_backward_it->offset -= it->size;
            }
            else
            {
                // 一致しない場合はそのままフリーに入れる
                mFreeList.emplace_back(it->size, it->offset, 0);
            }

            // リストから破棄して終了
            mUseList.erase(it);
            break;
        }
    }
}
//-----------------------------------------------------------------------------
} // bel::
