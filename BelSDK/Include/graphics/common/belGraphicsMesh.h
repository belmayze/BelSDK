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
        const void*            p_vertex_buffer     = nullptr;
        const void*            p_index_buffer      = nullptr;
        size_t                 vertex_buffer_size  = 0;
        size_t                 vertex_stride       = 0;
        size_t                 index_buffer_size   = 0;
        DXGI_FORMAT            index_buffer_format = DXGI_FORMAT_R16_UINT;
        D3D_PRIMITIVE_TOPOLOGY primitive_topology  = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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
     * インデックスバッファーを使った描画
     * @param[in] command      コマンド
     * @param[in] num_instance 描画インスタンス数
     * @param[in] start_index  描画するインデックスの開始番号
     * @param[in] index_count  描画するインデックスの数
     */
    void drawIndexedInstanced(
        CommandContext& command,
        uint32_t num_instance = 1,
        uint32_t start_index = 0,
        uint32_t index_count = std::numeric_limits<uint32_t>().max()) const;

    /*!
     * インデックスバッファーを使用しない描画
     * @param[in] command      コマンド
     * @param[in] num_instance 描画インスタンス数
     */
    void drawInstanced(CommandContext& command, uint32_t num_instance = 1) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mpVertexResource;
    Microsoft::WRL::ComPtr<ID3D12Resource> mpIndexResource;
    D3D_PRIMITIVE_TOPOLOGY                 mPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    D3D12_VERTEX_BUFFER_VIEW               mVertexBufferView = {};
    D3D12_INDEX_BUFFER_VIEW                mIndexBufferView = {};
    uint32_t                               mIndexCount = 0;
};
//-----------------------------------------------------------------------------
}
