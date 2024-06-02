/*!
 * @file   belGraphicsMesh.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx {
//-----------------------------------------------------------------------------
// メッシュ
//-----------------------------------------------------------------------------
class Mesh
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        const void*            mpVertexBuffer     = nullptr;
        const void*            mpIndexBuffer      = nullptr;
        size_t                 mVertexBufferSize  = 0;
        size_t                 mVertexStride      = 0;
        size_t                 mIndexBufferSize   = 0;
        DXGI_FORMAT            mIndexBufferFormat = DXGI_FORMAT_R16_UINT;
        D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * メッシュを初期化します
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * 描画
     * @param[in] command      コマンド
     * @param[in] num_instance 描画インスタンス数
     */
    void drawIndexedInstanced(CommandContext& command, uint32_t num_instance = 1) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexResource;
    Microsoft::WRL::ComPtr<ID3D12Resource> mpIndexResource;
    D3D_PRIMITIVE_TOPOLOGY                 mPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    D3D12_VERTEX_BUFFER_VIEW               mVertexBufferView;
    D3D12_INDEX_BUFFER_VIEW                mIndexBufferView;
    uint32_t                               mIndexCount;
};
//-----------------------------------------------------------------------------
}
