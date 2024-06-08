/*!
 * @file   belEnumFlags.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <bitset>

namespace bel {
//-----------------------------------------------------------------------------
// 列挙フラグ
//-----------------------------------------------------------------------------
template <typename T>
class EnumFlags
{
    // enum のみ許可
    static_assert(std::is_enum_v<T>, "列挙型のみ指定可能です");

    // 列挙体の基本型
    using BaseT = std::make_unsigned_t<std::underlying_type_t<T>>;

    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    constexpr EnumFlags() {}

    //! 指定したフラグを立てるコンストラクター
    constexpr EnumFlags(std::initializer_list<T> flags) : mBits(to_bitset(flags)) {}

    //-------------------------------------------------------------------------
    // setter
    //-------------------------------------------------------------------------
public:
    //! 任意の位置のビットを設定する
    constexpr EnumFlags& set(T e, bool v = true)
    {
        mBits.set(static_cast<BaseT>(e), v);
        return *this;
    }

    //! 任意の位置のビットを0にする
    constexpr EnumFlags& reset(T e)
    {
        set(e, false);
        return *this;
    }

    //! すべてのビットを0にする
    constexpr EnumFlags& reset()
    {
        mBits.reset();
        return *this;
    }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 任意の位置のビットが1になっているかを判定する
    constexpr bool test(T e) const { return mBits.test(static_cast<BaseT>(e)); }

    //! 全てのビットが1になっているかを判定する
    constexpr bool all() const { return mBits.all(); }

    //! いずれかのビットが1になっているかを判定する
    constexpr bool any() const { return mBits.any(); }

    //! 全てのビットが0になっているかを判定する
    constexpr bool none() const { return mBits.none(); }

    //! ビット数を取得する
    constexpr size_t size() const { return mBits.size(); }

    //! 1になっているビットの数を取得する
    constexpr size_t count() const { return mBits.count(); }

    //! 任意の位置のビットにアクセスする
    constexpr bool operator[](T e) const { return mBits[static_cast<BaseT>(e)]; }

    //-------------------------------------------------------------------------
private:
    std::bitset<static_cast<BaseT>(T::cNum)> mBits;

    //-------------------------------------------------------------------------
private:
    //! initializer_list からビットセットを作る
    static constexpr uint64_t to_bitset(std::initializer_list<T> flags)
    {
        uint64_t v = 0;
        for (auto&& flag : flags)
        {
            v |= 1 << static_cast<uint64_t>(flag);
        }
        return v;
    }
};
//-----------------------------------------------------------------------------
}
