/*!
 * @file   belGraphicsDynamicDescriptorHandle.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx {
//-----------------------------------------------------------------------------
// 動的確保するデスクリプターヒープ
//-----------------------------------------------------------------------------
class DynamicDescriptorHandle
{
    //-------------------------------------------------------------------------
    // setter
    //-------------------------------------------------------------------------
public:
    /*!
     * デスクリプターをセットする
     * @param[in] cpu_handle CPU ハンドル
     * @param[in] gpu_handle GPU ハンドル
     */
    void setProperty(D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
    {
        mCpuHandle = cpu_handle;
        mGpuHandle = gpu_handle;
        mValid = true;
    }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 有効判定
    bool isValid() const { return mValid; }

    //! CPU ハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE getCpuHandle() const { BEL_ASSERT(isValid()); return mCpuHandle; }

    //! GPU ハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE getGpuHandle() const { BEL_ASSERT(isValid()); return mGpuHandle; }

     //-------------------------------------------------------------------------
public:
    D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle;
    D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle;
    bool                        mValid = false;
};
//-----------------------------------------------------------------------------
}
