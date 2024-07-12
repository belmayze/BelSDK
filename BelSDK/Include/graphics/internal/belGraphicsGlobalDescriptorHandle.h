/*!
 * @file   belGraphicsGlobalDescriptorHandle.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class GlobalDescriptorRegistry; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// テクスチャーハンドル
//-----------------------------------------------------------------------------
class GlobalDescriptorHandle
{
    //-------------------------------------------------------------------------
public:
    //! デフォルトコンストラクター
    GlobalDescriptorHandle() = default;
    ~GlobalDescriptorHandle();
    //! コピー禁止
    GlobalDescriptorHandle(const GlobalDescriptorHandle&) = delete;
    GlobalDescriptorHandle& operator=(const GlobalDescriptorHandle&) = delete;
    //! move
    GlobalDescriptorHandle(GlobalDescriptorHandle&& handle) noexcept;
    GlobalDescriptorHandle& operator=(GlobalDescriptorHandle&& rhs) noexcept;

    //-------------------------------------------------------------------------
public:
    //! 値を持っているか
    bool hasValue() const { return mId.has_value(); }
    //! 値
    uint32_t getId() const { return mId.value(); }
    //! 解放
    void release();

public:
    //! ハンドルの操作ができるのは管理クラスのみ
    class Accessor
    {
        friend GlobalDescriptorRegistry;
    private:
        GlobalDescriptorHandle& mInstance;
        Accessor(GlobalDescriptorHandle& instance) : mInstance(instance) {}
    private:
        void setId(uint32_t id) { mInstance.mId = id; }
        void reset() { mInstance.mId.reset(); }
    };

private:
    std::optional<uint32_t> mId;
};
//-----------------------------------------------------------------------------
}
