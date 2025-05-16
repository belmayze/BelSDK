/*!
 * @file   belVector.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// vector
// std::vector を内部に持ち、push_back 時にメモリー再確保を行わない仕組みにしています
//-----------------------------------------------------------------------------
template <typename T>
class Vector : public std::vector<T>
{
public:
    using Base = std::vector<T>;

    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    Vector() {}
    Vector(size_t num) { allocate(num); }

    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    //! メモリーを確保する
    //! @note 今まで追加していた要素はすべて削除されます
    void allocate(size_t num)
    {
        Base::clear();
        Base::reserve(num);
    }
};
//-----------------------------------------------------------------------------
}
