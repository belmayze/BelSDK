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
        : mBuffer(std::make_unique<uint8_t[]>(sizeof(Container)* num))
        , mFreeBuffer(num)
        , mMaxSize(num)
    {
        // すべてフリーリストに入れる
        for (size_t i = 0; i < num; ++i)
        {
            mFreeBuffer.push_back(get_(i));
        }
    }

    //! デストラクター
    ~List() {}

    //! コピー禁止
    List(const List&) = delete;
    List& operator=(const List&) = delete;

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
        Container& container = mFreeBuffer.at(mFreeBuffer.size() - 1);
        mFreeBuffer.pop_back();

        // ポインター接続
        if (mpBegin)
        {
            container.next = mpBegin;
            container.prev = mpBegin->prev;

            mpBegin->prev->next = &container;
            mpBegin->prev = &container;
        }
        else
        {
            container.next = &container;
            container.prev = &container;

            mpBegin = &container;
        }

        // 加算
        mSize++;

        // メモリー確保
        T* p = new (&container.instance) T(std::forward<Args>(args)...);
        return *p;
    }

    //! 任意の要素を削除する
    void erase(T& x)
    {
        // belList で確保された T であれば、このアドレスの前にポインターが入っている
        uint8_t* p = reinterpret_cast<uint8_t*>(&x);
        Container* container = reinterpret_cast<Container*>(p - sizeof(Container*) * 2);

        // 接続を切る
        if (mSize == 1)
        {
            // 最後ならすべて削除
            mpBegin = nullptr;
        }
        else
        {
            container->next->prev = container->prev;
            container->prev->next = container->next;
            if (mpBegin == container) { mpBegin = container->next; }
        }
        container->next = nullptr;
        container->prev = nullptr;

        // デストラクター
        --mSize;
        container->instance.~T();

        // フリーへ
        mFreeBuffer.push_back(container);
    }

    // 要素をすべて削除する
    void clear() { while (!empty()) { erase(mpBegin->instance); } }

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

    //-------------------------------------------------------------------------
private:
    //! コンテナ
    struct Container
    {
        Container* next = nullptr;
        Container* prev = nullptr;
        T          instance;
    };

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<uint8_t[]> mBuffer;
    Vector<Container*>         mFreeBuffer;
    size_t     mMaxSize = 0;
    size_t     mSize    = 0;
    Container* mpBegin  = nullptr;

    //-------------------------------------------------------------------------
    // internal
    //-------------------------------------------------------------------------
private:
    /*!
     * トップ要素を取得する
     */
    Container& get_(size_t index) const
    {
        return reinterpret_cast<Container*>(mBuffer.get())[index];
    }
};
//-----------------------------------------------------------------------------
}
