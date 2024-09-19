/*!
 * @file   belGraphicsTextureCopyQueue.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsCommandQueue.h"
#include "graphics/common/belGraphicsCommandList.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/internal/belGraphicsTextureCopyQueue.h"
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceTexture.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// ctor / dtor
//-----------------------------------------------------------------------------
TextureCopyQueue::TextureCopyQueue() {}
//-----------------------------------------------------------------------------
TextureCopyQueue::~TextureCopyQueue() {}
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool TextureCopyQueue::initialize()
{
    // コピーキューを作る
    mpCopyQueue = std::make_unique<CommandQueue>();
    if (!mpCopyQueue->initialize(CommandType::cCopy))
    {
        BEL_ERROR("コピーキューの生成に失敗しました\n");
        return false;
    }

    // コマンドリストを作る
    mpCopyCommand = std::make_unique<CommandList>();
    if (!mpCopyCommand->initialize(CommandType::cCopy))
    {
        BEL_ERROR("コピーコマンドの生成に失敗しました\n");
        return false;
    }

    // フェンスハンドル
    mFenceHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!mFenceHandle)
    {
        BEL_ERROR("フェンス用イベントの生成に失敗しました\n");
        return false;
    }

    // フェンス
    if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence))))
    {
        BEL_ERROR("テクスチャーコピーキュー用のフェンス生成に失敗しました\n");
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
// execute
//-----------------------------------------------------------------------------
bool TextureCopyQueue::executeCopy(Texture& texture, const res::Texture& resource)
{
    // データアップロードに必要なサイズを取得します
    D3D12_RESOURCE_DESC                resource_desc = texture.getResource().GetDesc();
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT layout;
    UINT64                             row_size_in_bytes = 0;
    UINT                               row_counts         = 0;
    size_t                             resource_size      = 0;
    GraphicsEngine::GetInstance().getDevice().GetCopyableFootprints(
        &resource_desc, 0, 1, 0,
        &layout, &row_counts, &row_size_in_bytes, &resource_size
    );

    // アップロード用リソースを生成します
    Microsoft::WRL::ComPtr<ID3D12Resource> p_upload_resource;
    {
        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment          = 0;
        desc.Width              = resource_size;
        desc.Height             = 1;
        desc.DepthOrArraySize   = 1;
        desc.MipLevels          = 1;
        desc.Format             = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags              = D3D12_RESOURCE_FLAG_NONE;

        D3D12_HEAP_PROPERTIES props = {};
        props.Type                 = D3D12_HEAP_TYPE_UPLOAD;
        props.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        props.CreationNodeMask     = 1;
        props.VisibleNodeMask      = 1;

        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &props, D3D12_HEAP_FLAG_NONE,
            &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr, IID_PPV_ARGS(&p_upload_resource)
        )))
        {
            BEL_ERROR("テクスチャーアップロード用リソースの生成に失敗しました\n");
            return false;
        }

        // リソースにデータをコピー
        uint8_t* memory_ptr = nullptr;
        p_upload_resource->Map(0, nullptr, reinterpret_cast<void**>(&memory_ptr));
        std::memcpy(
            memory_ptr + layout.Offset,
            resource.getImageMemoryPtr(),
            resource_size
        );
        p_upload_resource->Unmap(0, nullptr);
    }

    // コマンドリストを初期化
    gfx::CommandContext command(*mpCopyCommand);
    mpCopyCommand->begin();
    {
        // コピーコマンドを積む
        D3D12_TEXTURE_COPY_LOCATION src;
        D3D12_TEXTURE_COPY_LOCATION dst;

        src.pResource       = p_upload_resource.Get();
        src.Type            = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src.PlacedFootprint = layout;

        dst.pResource        = &texture.getResource();
        dst.Type             = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dst.SubresourceIndex = 0;

        mpCopyCommand->getCommandList().CopyTextureRegion(
            &dst, 0, 0, 0, &src, nullptr
        );
    }
    mpCopyCommand->end();

    {
        // コマンド実行
        ID3D12CommandList* commands[1] = {&mpCopyCommand->getCommandList()};
        mpCopyQueue->getCommandQueue().ExecuteCommandLists(1, commands);

        // 終了待ち
        ResetEvent(mFenceHandle);
        mpFence->Signal(0);
        mpCopyQueue->getCommandQueue().Signal(mpFence.Get(), 1);

        // フェンスが1になるまで待機
        mpFence->SetEventOnCompletion(1, mFenceHandle);
        if (mpFence->GetCompletedValue() < 1)
        {
            WaitForSingleObject(mFenceHandle, INFINITE);
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
}