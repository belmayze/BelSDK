/*!
 * @file   belGraphicsGlobalDescriptorRegistry.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsConstantBuffer.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------
bool GlobalDescriptorRegistry::initialize(uint32_t num)
{
    // グローバルで大きなテクスチャー用デスクリプターヒープを作る
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = num;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mpDescriptorHeap))))
        {
            return false;
        }
    }

    // フリーリストの生成
    mFreeIndices.allocate(num);
    for (uint32_t index = 0; index < num; ++index)
    {
        mFreeIndices.emplace_back(index);
    }

    return true;
}
//-----------------------------------------------------------------------------
GlobalDescriptorHandle GlobalDescriptorRegistry::registerTexture(const Texture& texture)
{
    // フリーリストから取得
    uint32_t index = 0;
    {
        std::lock_guard lock(mFreeListMutex);
        if (mFreeIndices.empty()) { return GlobalDescriptorHandle(); }
        index = mFreeIndices.back();
        mFreeIndices.pop_back();
    }

    // 登録情報生成
    D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Shader4ComponentMapping = texture.getComponentMapping();
    desc.Format                  = to_native(texture.getFormat());
    desc.ViewDimension           = to_native_srv(texture.getDimension());
    switch (desc.ViewDimension)
    {
    case D3D12_SRV_DIMENSION_TEXTURE1D:
        desc.Texture1D.MipLevels = texture.getMipLevels();
        break;

    case D3D12_SRV_DIMENSION_TEXTURE1DARRAY:
        desc.Texture1DArray.MipLevels = texture.getMipLevels();
        desc.Texture1DArray.ArraySize = texture.getDepth();
        break;

    case D3D12_SRV_DIMENSION_TEXTURE2D:
        desc.Texture2D.MipLevels = texture.getMipLevels();
        break;

    case D3D12_SRV_DIMENSION_TEXTURE2DARRAY:
        desc.Texture2DArray.MipLevels = texture.getMipLevels();
        desc.Texture2DArray.ArraySize = texture.getDepth();
        break;

    case D3D12_SRV_DIMENSION_TEXTURE3D:
        desc.Texture3D.MipLevels = texture.getMipLevels();
        break;

    case D3D12_SRV_DIMENSION_TEXTURECUBE:
        desc.TextureCube.MipLevels = texture.getMipLevels();
        break;

    case D3D12_SRV_DIMENSION_TEXTURECUBEARRAY:
        desc.TextureCubeArray.MipLevels = texture.getMipLevels();
        desc.TextureCubeArray.NumCubes = texture.getDepth() / 6;
        break;
    }

    // デプスの SRV は作れないので代替えで生成する
    if (desc.Format == DXGI_FORMAT_D16_UNORM)                 { desc.Format = DXGI_FORMAT_R16_UNORM; }
    else if (desc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)    { desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; }
    else if (desc.Format == DXGI_FORMAT_D32_FLOAT)            { desc.Format = DXGI_FORMAT_R32_FLOAT; }
    else if (desc.Format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT) { desc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS; }

    // SRV 作成
    D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    descriptor_handle.ptr += GraphicsEngine::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
    GraphicsEngine::GetInstance().getDevice().CreateShaderResourceView(&texture.getResource(), &desc, descriptor_handle);

    GlobalDescriptorHandle handle;
    GlobalDescriptorHandle::Accessor(handle).setId(index);
    return handle;
}
//-----------------------------------------------------------------------------
GlobalDescriptorHandle GlobalDescriptorRegistry::registerConstantBuffer(D3D12_GPU_VIRTUAL_ADDRESS location, size_t size)
{
    // フリーリストから取得
    uint32_t index = 0;
    {
        std::lock_guard lock(mFreeListMutex);
        if (mFreeIndices.empty()) { return GlobalDescriptorHandle(); }
        index = mFreeIndices.back();
        mFreeIndices.pop_back();
    }

    //
    D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
    desc.BufferLocation = location;
    desc.SizeInBytes    = static_cast<UINT>(size);

    // CBV 作成
    D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    descriptor_handle.ptr += GraphicsEngine::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;
    GraphicsEngine::GetInstance().getDevice().CreateConstantBufferView(&desc, descriptor_handle);

    GlobalDescriptorHandle handle;
    GlobalDescriptorHandle::Accessor(handle).setId(index);
    return handle;
}
//-----------------------------------------------------------------------------
D3D12_CPU_DESCRIPTOR_HANDLE GlobalDescriptorRegistry::getDescriptorHandle(const GlobalDescriptorHandle& handle)
{
    if (!handle.hasValue())
    {
        // エラー
        BEL_ERROR_LOG("ハンドルが未登録なので動作が不安定です\n");
        return mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE descriptor_handle = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    descriptor_handle.ptr += GraphicsEngine::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * handle.getId();
    return descriptor_handle;
}
//-----------------------------------------------------------------------------
// EraseAccessor
//-----------------------------------------------------------------------------
void GlobalDescriptorRegistry::EraseAccessor::Erase(uint32_t id)
{
    GlobalDescriptorRegistry& instance = GlobalDescriptorRegistry::GetInstance();

    // フリーリストに登録
    {
        std::lock_guard lock(instance.mFreeListMutex);
        instance.mFreeIndices.emplace_back(id);
    }
}
//-----------------------------------------------------------------------------
}