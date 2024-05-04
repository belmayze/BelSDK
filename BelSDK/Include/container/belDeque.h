/*!
 * @file   belDeque.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// Double-ended queue
// std::dequeue と同じような仕組みですが最大数は固定です
// 内部実装はリングバッファー
//-----------------------------------------------------------------------------
template <typename T>
class Deque
{
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    Deque() {}
    Deque(size_t num) : mBuffer(std::make_unique<uint8_t[]>(sizeof(T) * num)), mMaxSize(num) {}

    //! デストラクター
    ~Deque() {}

    //! コピー禁止
    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    //-------------------------------------------------------------------------
    // iterator
    //-------------------------------------------------------------------------
public:
    //! const_iterator
    class const_iterator
    {
    public:
        using value_type = T;

    public:
        const_iterator(size_t index, const Deque<T>* p_instance) : mIndex(index), mpInstance(p_instance) {}
        const_iterator(const const_iterator& rhs) { *this = rhs; }

        const_iterator& operator=(const const_iterator& rhs) { mpInstance = rhs.mpInstance; mIndex = rhs.mIndex; return *this; }

        const T& operator*() const  { return mpInstance->get_(mIndex); }
        const T* operator->() const { return &mpInstance->get_(mIndex); }
        const T& operator[](ptrdiff_t index) const { return *(*this + index); }
        bool operator==(const const_iterator& rhs) const { BEL_ASSERT(mpInstance == rhs.mpInstance); return mIndex == rhs.mIndex; }

        const_iterator& operator++() { ++mIndex; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++*this; return tmp; }
        const_iterator& operator--() { --mIndex; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; ++*this; return tmp; }
        const_iterator& operator+=(ptrdiff_t diff) { mIndex += diff; return *this; }
        const_iterator operator+(ptrdiff_t diff) { const_iterator tmp = *this; tmp += diff; return tmp; }
        const_iterator& operator-=(ptrdiff_t diff) { return *this += -diff; }
        const_iterator operator-(ptrdiff_t diff) { const_iterator tmp = *this; tmp -= diff; return tmp; }

        friend const_iterator operator+(ptrdiff_t diff, const_iterator next) { next += diff; return next; }

        ptrdiff_t operator-(const const_iterator& rhs) const { BEL_ASSERT(mpInstance == rhs.mpInstance); return static_cast<ptrdiff_t>(mIndex - rhs.mIndex); }
        std::strong_ordering operator<=>(const const_iterator& rhs) { BEL_ASSERT(mpInstance == rhs.mpInstance); return mIndex <=> rhs.mIndex; }

    protected:
        const Deque<T>* mpInstance = nullptr;
        size_t          mIndex;
    };

    //! iterator
    class iterator : public const_iterator
    {
        using Base = const_iterator;

    public:
        using value_type = T;

    public:
        using Base::Base;

        T& operator*() const { return const_cast<T&>(Base::operator*()); }
        T* operator->() const { return const_cast<T*>(Base::operator->()); }
        T& operator[](ptrdiff_t index) const { return const_cast<T&>(Base::operator[](index)); }

        iterator& operator++() { Base::operator++(); return *this; }
        iterator operator++(int) { iterator tmp = *this; Base::operator++(); return tmp; }
        iterator& operator--() { Base::operator--(); return *this; }
        iterator operator--(int) { iterator tmp = *this; Base::operator--(); return tmp; }
        iterator& operator+=(ptrdiff_t diff) { Base::operator+=(diff); return *this; }
        iterator operator+(ptrdiff_t diff) { iterator tmp = *this; tmp += diff; return tmp; }
        iterator& operator-=(ptrdiff_t diff) { Base::operator-=(diff); return *this; }
        iterator operator-(ptrdiff_t diff) { iterator tmp = *this; tmp += diff; return tmp; }

        friend iterator operator+(ptrdiff_t diff, iterator next) { next += diff; return next; }

        using Base::operator-;
    };

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    //! メモリーを確保する
    //! @note 今まで追加していた要素はすべて削除されます
    void allocate(size_t num)
    {
        clear();
        mBuffer = std::make_unique<uint8_t[]>(sizeof(T) * num);
        mMaxSize = num;
    }

    //-------------------------------------------------------------------------
    // operation
    //-------------------------------------------------------------------------
public:
    //! 要素を末尾に追加する
    void push_back(const T& x) { emplace_back(x); }
    void push_back(T&& x) { emplace_back(std::move(x)); }

    template <class... Args>
    T& emplace_back(Args&&... args)
    {
        T* p = new (&get_(mBottom)) T(std::forward<Args>(args)...);
        ++mBottom;
        return *p;
    }

    //! 要素を先頭に追加する
    void push_front(const T& x) { emplace_front(x); }
    void push_front(T&& x) { emplace_front(std::move(x)); }

    template <class... Args>
    T& emplace_front(Args&&... args)
    {
        --mTop;
        T* p = new (&get_(mTop)) T(std::forward<Args>(args)...);
        return *p;
    }

    //! 末尾から要素を削除する
    void pop_back()
    {
        --mBottom;
        get_(mBottom).~T();
    }

    //! 先頭から要素を削除する
    void pop_front()
    {
        get_(mTop).~T();
        ++mTop;
    }

    //! 指定した要素を削除する
    //iterator erase(const_iterator it); // @TODO

    //! 要素をすべて削除する
    void clear() { while (!empty()) { pop_back(); } }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 要素を取得する
    T& at(size_t n)             { BEL_ASSERT(n < size()); size_t index = mTop + n; return get_(index); }
    const T& at(size_t n) const { BEL_ASSERT(n < size()); size_t index = mTop + n; return get_(index); }

    //! 要素数を取得する
    size_t size() const { return mBottom - mTop; }

    //! コンテナが空であるかを調べる
    bool empty() const { return size() == 0; }

    //! 格納可能な最大の要素数を取得する
    size_t max_size() const { return mMaxSize; }

    //! 先頭要素を指すイテレーターを取得する
    iterator begin() { return iterator(mTop, this); }
    const_iterator begin() const { return const_iterator(mTop, this); }
    const_iterator cbegin() const { return begin(); }

    //! 末尾要素を指すイテレーターを取得する
    iterator end() { return iterator(mBottom, this); }
    const_iterator end() const { return const_iterator(mBottom, this); }
    const_iterator cend() const { return end(); }

    //! 末尾要素を指す逆イテレーターを取得する
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const { return rbegin(); }

    //! 先頭要素を指す逆イテレーターを取得する
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const { return rend(); }

    //-------------------------------------------------------------------------
    // operator
    //-------------------------------------------------------------------------
public:
    T& operator[](size_t n) { return at(n); }
    const T& operator[](size_t n) const { return at(n); }

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<uint8_t[]> mBuffer;
    size_t mMaxSize = 0;
    size_t mTop     = 0;
    size_t mBottom  = 0;

    //-------------------------------------------------------------------------
    // internal
    //-------------------------------------------------------------------------
private:
    /*!
     * バッファーインデックス取得
     * @param[in] index インデックス
     */
    size_t getBufferIndex_(size_t index) const { return index % mMaxSize; }

    /*!
     * トップ要素を取得する
     */
    T& get_(size_t index) const
    {
        index = getBufferIndex_(index);
        return reinterpret_cast<T*>(mBuffer.get())[index];
    }
};
//-----------------------------------------------------------------------------
}
