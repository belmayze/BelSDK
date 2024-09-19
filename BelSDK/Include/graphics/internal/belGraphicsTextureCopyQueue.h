/*!
 * @file   belGraphicsTextureCopyQueue.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class CommandQueue; }
namespace bel::gfx { class CommandList; }
namespace bel::gfx { class Texture; }
namespace bel::res { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// グローバルなデスクリプター登録
//-----------------------------------------------------------------------------
class TextureCopyQueue
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    TextureCopyQueue();
    ~TextureCopyQueue();

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * テクスチャーコピー用キューを作る
     */
    bool initialize();

    //-------------------------------------------------------------------------
    // execute
    //-------------------------------------------------------------------------
public:
    /*!
     * テクスチャーをコピーする
     * @param[in] texture  テクスチャー
     * @param[in] resource リソース
     */
    bool executeCopy(Texture& texture, const res::Texture& resource);

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<CommandQueue>       mpCopyQueue;
    std::unique_ptr<CommandList>        mpCopyCommand;
    Microsoft::WRL::ComPtr<ID3D12Fence> mpFence;
    HANDLE                              mFenceHandle = HANDLE();
};
//-----------------------------------------------------------------------------
}
