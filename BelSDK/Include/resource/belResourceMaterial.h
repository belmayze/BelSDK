/*!
 * @file   belResourceMaterial.h
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
class Material
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    Material() {}

    //-------------------------------------------------------------------------
    // 情報取得
    //-------------------------------------------------------------------------
public:
    //! 使用するメッシュ番号
    uint32_t getMeshIndex() const { return 0; }
    //! 使用するメッシュ開始インデックス
    size_t getMeshStartIndex() const { return 0; }
    //! 使用するメッシュインデックス数
    size_t getMeshIndexCount() const { return std::numeric_limits<uint32_t>().max(); }
};

//-----------------------------------------------------------------------------
} // bel::res::
