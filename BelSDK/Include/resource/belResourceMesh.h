/*!
 * @file   belResourceMesh.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel

namespace bel::res {

//-----------------------------------------------------------------------------
//! リソース単体で使われることはなく、res::Model に内包されて使用する
class Mesh
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    Mesh() {}

    //-------------------------------------------------------------------------
    // 情報取得
    //-------------------------------------------------------------------------
public:
    //! 頂点バッファー
    void* getVertexBuffer() const { return nullptr; }
    //! 頂点バッファーサイズ
    size_t getVertexBufferSize() const { return 0; }
    //! 頂点バッファーストライドサイズ
    size_t getVertexStride() const { return 0; }

    //! インデックスバッファー
    void* getIndexBuffer() const { return nullptr; }
    //! インデックスバッファーサイズ
    size_t getIndexBufferSize() const { return 0; }
    //! インデックスバッファーフォーマット
    DXGI_FORMAT getIndexBufferFormat() const { return DXGI_FORMAT_R16_UINT; }

    //! トポロジー
    D3D_PRIMITIVE_TOPOLOGY getPrimitiveTopology() const { return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }

    //-------------------------------------------------------------------------
private:

};

//-----------------------------------------------------------------------------
} // bel::res::
