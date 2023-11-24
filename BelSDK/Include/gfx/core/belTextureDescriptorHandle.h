/*!
 * @file   belTextureDescriptorHandle.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <optional>

namespace bel::gfx { class TextureDescriptorRegistry; }

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * テクスチャーデスクリプターハンドル
 */
class TextureDescriptorHandle
{
public:
    //! デフォルトコンストラクター
    TextureDescriptorHandle() = default;
    ~TextureDescriptorHandle();
    //! コピー禁止
    TextureDescriptorHandle(const TextureDescriptorHandle&) = delete;
    TextureDescriptorHandle& operator=(const TextureDescriptorHandle&) = delete;
    //! move
    TextureDescriptorHandle(TextureDescriptorHandle&& handle) noexcept;
    TextureDescriptorHandle& operator=(TextureDescriptorHandle&& rhs) noexcept;

public:
    //! 値が含まれているか
    bool hasValue() const { return mId.has_value(); }
    //! 値
    uint32_t getId() const { return mId.value(); }

    //! 解放
    void release();

public:
    //! accessor
    class Accessor
    {
        friend TextureDescriptorRegistry;
    private:
        TextureDescriptorHandle& mInstance;
        Accessor(TextureDescriptorHandle& instance) : mInstance(instance) {}
    private:
        void setId(uint32_t id) { mInstance.mId = id; }
        void reset() { mInstance.mId.reset(); }
    };

private:
    std::optional<uint32_t> mId;
};
//-----------------------------------------------------------------------------

}
