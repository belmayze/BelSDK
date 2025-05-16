/*!
 * @file   belList.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// リスト
// std::list と同じような仕組みですが最大数は固定です
// 内部実装は双方向リスト
//-----------------------------------------------------------------------------
template <typename T>
class List
{
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    List() {}
    List(size_t num)
        : mBuffer(std::make_unique<uint8_t[]>(sizeof(list_node) * ( num + 1 )))
        , mFreeBuffer(num)
        , mMaxSize(num)
    {
        // すべてフリーリストに入れる
        for (size_t i = 0; i < num; ++i)
        {
            mFreeBuffer.push_back(&get_(i));
        }

        // 最後は end 用のバッファー
        mpHead = &get_(num);
        mpHead->next = mpHead;
        mpHead->prev = mpHead;
    }

    //! デストラクター
    ~List() { clear(); }

    //! コピー禁止
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    //-------------------------------------------------------------------------
    // iterator
    //-------------------------------------------------------------------------
public:
    //! node
    struct list_node
    {
        list_node* next = nullptr;
        list_node* prev = nullptr;
        T          instance;
    };

    //! const_iterator
    class const_iterator
    {
        friend class List<T>;

    public:
        using iterator_category = std::bidirectional_iterator_tag;

        using value_type = T;
        using difference_type = std::ptrdiff_t;

    public:
        const_iterator(list_node* p_node, const List<T>* p_instance) : mpInstance(p_instance), mpNode(p_node) {}
        const_iterator(const const_iterator& rhs) { *this = rhs; }

        const_iterator& operator=(const const_iterator& rhs) { mpInstance = rhs.mpInstance; mpNode = rhs.mpNode; return *this; }

        const T& operator*() const { return mpNode->instance; }
        const T* operator->() const { return &mpNode->instance; }
        bool operator==(const const_iterator& rhs) const { BEL_ASSERT(mpInstance == rhs.mpInstance); return mpNode == rhs.mpNode; }

        const_iterator& operator++() { mpNode = mpNode->next; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++*this; return tmp; }
        const_iterator& operator--() { mpNode = mpNode->prev; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --*this; return tmp; }

    protected:
        const List<T>* mpInstance = nullptr;
        list_node* mpNode;
    };

    //! iterator
    class iterator : public const_iterator
    {
        friend class List<T>;
        using Base = const_iterator;

    public:
        using value_type = T;

    public:
        using Base::Base;

        T& operator*() const { return const_cast<T&>(Base::operator*()); }
        T* operator->() const { return const_cast<T*>(Base::operator->()); }

        iterator& operator++() { Base::operator++(); return *this; }
        iterator operator++(int) { iterator tmp = *this; Base::operator++(); return tmp; }
        iterator& operator--() { Base::operator--(); return *this; }
        iterator operator--(int) { iterator tmp = *this; Base::operator--(); return tmp; }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
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
        mBuffer = std::make_unique<uint8_t[]>(sizeof(list_node) * ( num + 1));
        mMaxSize = num;
        mFreeBuffer.allocate(num);

        // すべてフリーリストに入れる
        for (size_t i = 0; i < num; ++i)
        {
            mFreeBuffer.push_back(&get_(i));
        }

        // 最後は end 用のバッファー
        mpHead = &get_(num);
        mpHead->next = mpHead;
        mpHead->prev = mpHead;
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
        return *emplace(end(), std::forward<Args>(args)...);
    }

    //! 任意のイテレーターの前に要素を追加する
    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args)
    {
        // 空きコンテナ取得
        list_node& node = *mFreeBuffer.at(mFreeBuffer.size() - 1);
        mFreeBuffer.pop_back();
        BEL_ASSERT(node.next == nullptr && node.prev == nullptr);

        // ポインター接続
        node.next = position.mpNode;
        node.prev = position.mpNode->prev;

        node.next->prev = &node;
        node.prev->next = &node;

        // 加算
        mSize++;

        // メモリー確保
        T* p = new (&node.instance) T(std::forward<Args>(args)...);
        return iterator(&node, this);
    }

    //! 任意の要素を削除する
    void erase(T& x)
    {
        // belList で確保された T であれば、このアドレスの前にポインターが入っている
        uint8_t* p = reinterpret_cast<uint8_t*>(&x);
        list_node* node = reinterpret_cast<list_node*>(p - sizeof(list_node*) * 2);
        BEL_ASSERT(node->next != nullptr && node->prev != nullptr);

        // 接続を切る
        node->next->prev = node->prev;
        node->prev->next = node->next;

        node->next = nullptr;
        node->prev = nullptr;

        // デストラクター
        --mSize;
        node->instance.~T();

        // フリーへ
        mFreeBuffer.push_back(node);
    }
    iterator erase(iterator it)
    {
        iterator tmp = it;
        ++tmp;
        erase(it.mpNode->instance);
        return tmp;
    }
    const_iterator erase(const_iterator it)
    {
        const_iterator tmp = it;
        ++tmp;
        erase(it.mpNode->instance);
        return tmp;
    }

    // 要素をすべて削除する
    void clear() { while (!empty()) { erase(begin()); } }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 先頭の要素を取得する
    T& front() { BEL_ASSERT(!empty()); return mpHead->next->instance; }
    const T& front() const { BEL_ASSERT(!empty()); return mpHead->next->instance; }

    //! 末尾の要素を取得する
    T& back() { BEL_ASSERT(!empty()); return mpHead->prev->instance; }
    const T& back()const { BEL_ASSERT(!empty()); return mpHead->prev->instance; }

    //! 要素数を取得する
    size_t size() const { return mSize; }

    //! コンテナが空であるかを調べる
    bool empty() const { return size() == 0; }

    //! 格納可能な最大の要素数を取得する
    size_t max_size() const { return mMaxSize; }

    //! 先頭要素を指すイテレーターを取得する
    iterator begin() { return iterator(mpHead->next, this); }
    const_iterator begin() const { return const_iterator(mpHead->next, this); }
    const_iterator cbegin() const { return begin(); }

    //! 末尾要素を指すイテレーターを取得する
    iterator end() { return iterator(mpHead, this); }
    const_iterator end() const { return const_iterator(mpHead, this); }
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
private:
    std::unique_ptr<uint8_t[]> mBuffer;
    Vector<list_node*>         mFreeBuffer;
    size_t     mMaxSize = 0;
    size_t     mSize = 0;
    list_node* mpHead = nullptr;

    //-------------------------------------------------------------------------
    // internal
    //-------------------------------------------------------------------------
private:
    /*!
     * トップ要素を取得する
     */
    list_node& get_(size_t index) const
    {
        return reinterpret_cast<list_node*>(mBuffer.get())[index];
    }
};
//-----------------------------------------------------------------------------
}
