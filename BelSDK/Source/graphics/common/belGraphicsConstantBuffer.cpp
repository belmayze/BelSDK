/*!
 * @file   belGraphicsConstantBuffer.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsConstantBuffer.h"
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool ConstantBuffer::initialize(const InitializeArg& arg)
{
    // 保持
    mNumBuffer = arg.mNumBuffer;
    // バッファーサイズ計算
    mBufferSize = Math::Roundup(arg.mBufferSize, static_cast<size_t>(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT));

    // バッファーを生成
    auto p_resources        = std::make_unique<Microsoft::WRL::ComPtr<ID3D12Resource>[]>(mNumBuffer);
    auto buffer_ptrs        = std::make_unique<uint8_t* []>(mNumBuffer);
    auto descriptor_handles = std::make_unique<GlobalDescriptorHandle[]>(mNumBuffer);

    // 定数バッファーを作成
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        D3D12_HEAP_PROPERTIES props = {};
        props.Type             = D3D12_HEAP_TYPE_UPLOAD;
        props.CreationNodeMask = 1;
        props.VisibleNodeMask  = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width            = static_cast<UINT64>(mBufferSize);
        desc.Height           = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels        = 1;
        desc.SampleDesc.Count = 1;
        desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        // 作成
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &props, D3D12_HEAP_FLAG_NONE,
            &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr, IID_PPV_ARGS(&p_resources[i_buffer]))))
        {
            return false;
        }

        // メモリーの先頭アドレスを取得しておく
        // 定数バッファーは終了するまで Unmap する必要はない
        if (FAILED(p_resources[i_buffer]->Map(0, nullptr, reinterpret_cast<void**>(&buffer_ptrs[i_buffer]))))
        {
            return false;
        }

        // デスクリプターに登録
        descriptor_handles[i_buffer] = GlobalDescriptorRegistry::GetInstance().registerConstantBuffer(
            p_resources[i_buffer]->GetGPUVirtualAddress(), mBufferSize
        );
    }

    // バッファー保持
    mpResources = std::move(p_resources);
    mBufferPtrs = std::move(buffer_ptrs);
    mDescriptorHandles = std::move(descriptor_handles);

    return true;
}
//-----------------------------------------------------------------------------
void ConstantBuffer::swapBuffer()
{
    if (++mBufferIndex >= mNumBuffer) { mBufferIndex = 0; }
}
//-----------------------------------------------------------------------------
void ConstantBuffer::copyBuffer(void* ptr, size_t size, size_t offset)
{
    BEL_ASSERT(size + offset <= mBufferSize);
    std::memcpy(
        mBufferPtrs[mBufferIndex] + offset,
        ptr,
        size
    );
}
//-----------------------------------------------------------------------------
}