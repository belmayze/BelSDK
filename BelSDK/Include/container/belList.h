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
        : mBuffer(std::make_unique<uint8_t[]>(sizeof(list_node)* num))
        , mFreeBuffer(num)
        , mMaxSize(num)
    {
        // すべてフリーリストに入れる
        for (size_t i = 0; i < num; ++i)
        {
            mFreeBuffer.push_back(&get_(i));
        }
    }

    //! デストラクター
    ~List() {}

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
        using value_type = T;

    public:
        const_iterator(list_node* p_node, const List<T>* p_instance) : mpInstance(p_instance), mpNode(p_node) {}
        const_iterator(const const_iterator& rhs) { *this = rhs; }

        const_iterator& operator=(const const_iterator& rhs) { mpInstance = rhs.mpInstance; mpNode = rhs.mpNode; return *this; }

        const T& operator*() const { return mpNode->instance; }
        const T* operator->() const { return &mpNode->instance; }

    private:
        const List<T>* mpInstance = nullptr;
        list_node* mpNode;
    };

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
        // 空きコンテナ取得
        list_node& node = *mFreeBuffer.at(mFreeBuffer.size() - 1);
        mFreeBuffer.pop_back();

        // ポインター接続
        if (mpHead)
        {
            node.next = mpHead;
            node.prev = mpHead->prev;

            mpHead->prev->next = &node;
            mpHead->prev = &node;
        }
        else
        {
            node.next = &node;
            node.prev = &node;

            mpHead = &node;
        }

        // 加算
        mSize++;

        // メモリー確保
        T* p = new (&node.instance) T(std::forward<Args>(args)...);
        return *p;
    }

    //! 任意の要素を削除する
    void erase(T& x)
    {
        // belList で確保された T であれば、このアドレスの前にポインターが入っている
        uint8_t* p = reinterpret_cast<uint8_t*>(&x);
        list_node* node = reinterpret_cast<list_node*>(p - sizeof(list_node*) * 2);

        // 接続を切る
        if (mSize == 1)
        {
            // 最後ならすべて削除
            mpHead = nullptr;
        }
        else
        {
            node->next->prev = node->prev;
            node->prev->next = node->next;
            if (mpHead == node) { mpHead = node->next; }
        }
        node->next = nullptr;
        node->prev = nullptr;

        // デストラクター
        --mSize;
        node->instance.~T();

        // フリーへ
        mFreeBuffer.push_back(node);
    }
    void erase(const_iterator it)
    {

        erase(it.mpNode->instance);
    }

    // 要素をすべて削除する
    void clear() { while (!empty()) { erase(mpHead->instance); } }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 要素数を取得する
    size_t size() const { return mSize; }

    //! コンテナが空であるかを調べる
    bool empty() const { return size() == 0; }

    //! 格納可能な最大の要素数を取得する
    size_t max_size() const { return mMaxSize; }

    //! 先頭要素を指すイテレーターを取得する
    const_iterator begin() const { return const_iterator(mpHead, this); }

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<uint8_t[]> mBuffer;
    Vector<list_node*>         mFreeBuffer;
    size_t     mMaxSize = 0;
    size_t     mSize    = 0;
    list_node* mpHead  = nullptr;

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
