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
class Vector
{
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    Vector() {}
    Vector(size_t num) { allocate(num); }

    //! デストラクター
    ~Vector() { clear(); }

    //! コピー禁止
    Vector(const Vector&) = delete;
    Vector& operator=(const Vector&) = delete;

    //-------------------------------------------------------------------------
    // iterator
    //-------------------------------------------------------------------------
public:
    using iterator = std::vector<T>::iterator;
    using const_iterator = std::vector<T>::const_iterator;
    using reverse_iterator = std::vector<T>::reverse_iterator;
    using const_reverse_iterator = std::vector<T>::const_reverse_iterator;

    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    //! メモリーを確保する
    //! @note 今まで追加していた要素はすべて削除されます
    void allocate(size_t num)
    {
        clear();
        mBuffer.reserve(num);
    }

    //-------------------------------------------------------------------------
    // operation
    //-------------------------------------------------------------------------
public:
    //! 要素を末尾に追加する
    void push_back(const T& x) { mBuffer.push_back(x); }
    void push_back(T&& x) { mBuffer.push_back(std::move(x)); }

    template <class... Args>
    T& emplace_back(Args&&... args)
    {
        return mBuffer.emplace_back(std::forward<Args>(args)...);
    }

    //! 末尾から要素を削除する
    void pop_back() { mBuffer.pop_back(); }

    //! 指定した要素を削除する
    iterator erase(const_iterator it) { mBuffer.erase(it); }

    //! 要素をすべて削除する
    void clear() { mBuffer.clear(); }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 要素を取得する
    T& at(size_t n) { return mBuffer.at(n); }
    const T& at(size_t n) const { return mBuffer.at(n); }

    //! 先頭の要素を取得する
    T& front() { return mBuffer.front(); }
    const T& front() const { mBuffer.front(); }

    //! 末尾の要素を取得する
    T& back() { return mBuffer.back(); }
    const T& back() const { return mBuffer.back(); }

    //! 要素数を取得する
    size_t size() const { return mBuffer.size(); }

    //! コンテナが空であるかを調べる
    bool empty() const { return mBuffer.empty(); }

    //! 格納可能な最大の要素数を取得する
    size_t max_size() const { return mBuffer.max_size(); }

    //! キャパシティ
    size_t capacity() const { return mBuffer.capacity(); }

    //! 先頭アドレス
    T* data() { return mBuffer.data(); }
    const T* data() const { return mBuffer.data(); }

    //! 先頭要素を指すイテレーターを取得する
    iterator begin() { return mBuffer.begin(); }
    const_iterator begin() const { return mBuffer.begin(); }
    const_iterator cbegin() const { return mBuffer.cbegin(); }

    //! 末尾要素を指すイテレーターを取得する
    iterator end() { return mBuffer.end(); }
    const_iterator end() const { return mBuffer.end(); }
    const_iterator cend() const { return mBuffer.cend(); }

    //! 末尾要素を指す逆イテレーターを取得する
    reverse_iterator rbegin() { return mBuffer.rbegin(); }
    const_reverse_iterator rbegin() const { return mBuffer.rbegin(); }
    const_reverse_iterator crbegin() const { return mBuffer.crbegin(); }

    //! 先頭要素を指す逆イテレーターを取得する
    reverse_iterator rend() { return mBuffer.rend(); }
    const_reverse_iterator rend() const { return mBuffer.rend(); }
    const_reverse_iterator crend() const { return mBuffer.crend(); }

    //-------------------------------------------------------------------------
    // operator
    //-------------------------------------------------------------------------
public:
    T& operator[](size_t n) { return mBuffer[n]; }
    const T& operator[](size_t n) const { return mBuffer[n]; }

    //-------------------------------------------------------------------------
private:
    std::vector<T> mBuffer;
};
//-----------------------------------------------------------------------------
}
