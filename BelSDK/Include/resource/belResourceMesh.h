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
    //! 先頭アドレスで初期化
    Mesh(uint8_t* ptr) : mPtr(ptr) {}

    //-------------------------------------------------------------------------
    // 情報取得
    //-------------------------------------------------------------------------
public:
    //! 頂点バッファー
    void* getVertexBuffer() const
    {
        return getVertexBufferPtr_();
    }
    //! 頂点バッファーサイズ
    size_t getVertexBufferSize() const
    {
        return getFileHeader_().vertex_buffer_size;
    }
    //! 頂点バッファーストライドサイズ
    size_t getVertexStride() const
    {
        return getFileHeader_().vertex_stride_size;
    }

    //! インデックスバッファー
    void* getIndexBuffer() const
    {
        return getIndexBufferPtr_();
    }
    //! インデックスバッファーサイズ
    size_t getIndexBufferSize() const
    {
        return getFileHeader_().index_buffer_size;
    }
    //! インデックスバッファーフォーマット
    DXGI_FORMAT getIndexBufferFormat() const
    {
        return static_cast<DXGI_FORMAT>(getFileHeader_().index_buffer_format);
    }

    //! トポロジー
    D3D_PRIMITIVE_TOPOLOGY getPrimitiveTopology() const
    {
        return static_cast<D3D_PRIMITIVE_TOPOLOGY>(getFileHeader_().primitive_topology);
    }

    //! 頂点レイアウトの個数
    uint32_t getNumInputLayout() const
    {
        return getFileHeader_().num_input_layout;
    }
    //! 頂点レイアウト
    void getInputLayout(D3D12_INPUT_ELEMENT_DESC& out, uint32_t index) const
    {
        const InputLayout& layout = getInputLayout_(index);

        out.AlignedByteOffset    = layout.offset;
        out.Format               = static_cast<DXGI_FORMAT>(layout.format);
        out.InputSlot            = layout.input_slot;
        out.InputSlotClass       = static_cast<D3D12_INPUT_CLASSIFICATION>(layout.input_slot_class);
        out.InstanceDataStepRate = layout.instance_step_rate;
        out.SemanticIndex        = layout.semantic_index;
        // データが改変され、終端文字が消えても大丈夫か？
        out.SemanticName         = getStringPtr_(layout.semantic_name_offset, layout.semantic_name_strlen);
    }

    //-------------------------------------------------------------------------
    // ファイルフォーマット
    // 
    // +------------------+--------------+
    // | Header           | 80 bytes     |
    // +------------------+--------------+
    // | Input layouts    | 12 bytes * N |
    // +------------------+--------------+
    // | Vertex buffer    | variable     |
    // +------------------+--------------+
    // | Index buffer     | variable     |
    // +------------------+--------------+
    // | Strings          | variable     |
    // +------------------+--------------+
    // 
    //-------------------------------------------------------------------------
private:
    //! ヘッダー構造体
    struct FileHeader
    {
        float    world_matrix[4 * 3] = { 0.f }; // Matrix34 は alignment を要求してくるので float に
        uint32_t vertex_buffer_size  = 0;
        uint32_t index_buffer_size   = 0;
        uint16_t vertex_stride_size  = 0;
        uint8_t  index_buffer_format = DXGI_FORMAT_R16_UINT;
        uint8_t  primitive_topology  = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        uint32_t mesh_name_offset    = 0;
        uint8_t  mesh_name_strlen    = 0;
        uint8_t  num_input_layout    = 0;
    };
    static_assert(sizeof(FileHeader) == 68);

    //! 頂点レイアウト構造体
    struct InputLayout
    {
        uint8_t  offset               = 0;
        uint8_t  format               = DXGI_FORMAT_R32G32B32_FLOAT;
        uint8_t  input_slot           = 0;
        uint8_t  input_slot_class     = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        uint8_t  instance_step_rate   = 0;
        uint8_t  semantic_index       = 0;
        uint8_t  semantic_name_strlen = 0;
        uint8_t  reserved = 0;
        uint32_t semantic_name_offset = 0;
    };
    static_assert(sizeof(InputLayout) == 12);

    //-------------------------------------------------------------------------
private:
    uint8_t* mPtr = nullptr;

    //-------------------------------------------------------------------------
    // internal function
    //-------------------------------------------------------------------------
private:
    //! ファイルヘッダーを取得
    const FileHeader& getFileHeader_() const
    {
        BEL_ASSERT(mPtr != nullptr);
        return *reinterpret_cast<const FileHeader*>(mPtr);
    }
    //! 頂点レイアウトを取得
    const InputLayout& getInputLayout_(uint32_t index) const
    {
        BEL_ASSERT(mPtr != nullptr);
        BEL_ASSERT(index < getFileHeader_().num_input_layout);
        return reinterpret_cast<const InputLayout*>(mPtr + sizeof(FileHeader))[index];
    }
    //! 頂点バッファーを取得
    void* getVertexBufferPtr_() const
    {
        BEL_ASSERT(mPtr != nullptr);
        const FileHeader& file_header = getFileHeader_();
        size_t offset = sizeof(FileHeader) + sizeof(InputLayout) * file_header.num_input_layout;
        return reinterpret_cast<void*>(mPtr + offset);
    }
    //! インデックスバッファーを取得
    void* getIndexBufferPtr_() const
    {
        BEL_ASSERT(mPtr != nullptr);
        const FileHeader& file_header = getFileHeader_();
        size_t offset = sizeof(FileHeader) + sizeof(InputLayout) * file_header.num_input_layout + file_header.vertex_buffer_size;
        return reinterpret_cast<void*>(mPtr + offset);
    }
    //! 文字列取得
    const char* getStringPtr_(size_t offset, size_t) const
    {
        BEL_ASSERT(mPtr != nullptr);
        return reinterpret_cast<const char*>(mPtr + offset);
    }
};

//-----------------------------------------------------------------------------
} // bel::res::
