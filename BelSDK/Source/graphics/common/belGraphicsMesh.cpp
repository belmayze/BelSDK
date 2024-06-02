/*!
 * @file   belGraphicsMesh.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsMesh.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool Mesh::initialize(const InitializeArg& arg)
{
    // バッファーを転送
    Microsoft::WRL::ComPtr<ID3D12Resource> p_vertex_buffer, p_index_buffer;

    // 頂点バッファー
    {
        D3D12_HEAP_PROPERTIES props = {};
        props.Type             = D3D12_HEAP_TYPE_UPLOAD;
        props.CreationNodeMask = 1;
        props.VisibleNodeMask  = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width            = arg.mVertexBufferSize;
        desc.Height           = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels        = 1;
        desc.SampleDesc.Count = 1;
        desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &props, D3D12_HEAP_FLAG_NONE,
            &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr, IID_PPV_ARGS(&p_vertex_buffer))))
        {
            return false;
        }

        // データ転送
        uint8_t* dst_ptr = nullptr;
        if (FAILED(p_vertex_buffer->Map(0, nullptr, reinterpret_cast<void**>(&dst_ptr)))) { return false; }
        std::memcpy(dst_ptr, arg.mpVertexBuffer, arg.mVertexBufferSize);
        p_vertex_buffer->Unmap(0, nullptr);
    }

    // インデックスバッファー
    {
        D3D12_HEAP_PROPERTIES props = {};
        props.Type             = D3D12_HEAP_TYPE_UPLOAD;
        props.CreationNodeMask = 1;
        props.VisibleNodeMask  = 1;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Width            = arg.mIndexBufferSize;
        desc.Height           = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels        = 1;
        desc.SampleDesc.Count = 1;
        desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &props, D3D12_HEAP_FLAG_NONE,
            &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr, IID_PPV_ARGS(&p_index_buffer))))
        {
            return false;
        }

        // データ転送
        uint8_t* dst_ptr = nullptr;
        if (FAILED(p_index_buffer->Map(0, nullptr, reinterpret_cast<void**>(&dst_ptr)))) { return false; }
        std::memcpy(dst_ptr, arg.mpIndexBuffer, arg.mIndexBufferSize);
        p_index_buffer->Unmap(0, nullptr);
    }

    // リソース保持
    mpVertexResource = std::move(p_vertex_buffer);
    mpIndexResource  = std::move(p_index_buffer);

    //
    BEL_ASSERT(arg.mIndexBufferFormat == DXGI_FORMAT_R16_UINT || arg.mIndexBufferFormat == DXGI_FORMAT_R32_UINT);
    size_t index_size = (arg.mIndexBufferFormat == DXGI_FORMAT_R16_UINT) ? sizeof(uint16_t) : sizeof(uint32_t);

    // ビューの作成
    mVertexBufferView.BufferLocation = mpVertexResource->GetGPUVirtualAddress();
    mVertexBufferView.SizeInBytes    = static_cast<uint32_t>(arg.mVertexBufferSize);
    mVertexBufferView.StrideInBytes  = static_cast<uint32_t>(arg.mVertexStride);

    mIndexBufferView.BufferLocation = mpIndexResource->GetGPUVirtualAddress();
    mIndexBufferView.SizeInBytes    = static_cast<uint32_t>(arg.mIndexBufferSize);
    mIndexBufferView.Format         = arg.mIndexBufferFormat;

    mIndexCount        = static_cast<uint32_t>(arg.mIndexBufferSize / index_size);
    mPrimitiveTopology = arg.mPrimitiveTopology;

    return true;
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void Mesh::drawIndexedInstanced(CommandContext& command, uint32_t num_instance) const
{
    // 描画設定
    command.getCommandList().IASetPrimitiveTopology(mPrimitiveTopology);

    // バッファー設定
    command.getCommandList().IASetVertexBuffers(0, 1, &mVertexBufferView);
    command.getCommandList().IASetIndexBuffer(&mIndexBufferView);

    // 描画
    command.getCommandList().DrawIndexedInstanced(mIndexCount, num_instance, 0, 0, 0);
}
//-----------------------------------------------------------------------------
}