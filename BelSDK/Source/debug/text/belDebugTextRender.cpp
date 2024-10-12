/*!
 * @file   belDebugTextRender.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "debug/text/belDebugTextRender.h"
#include "graphics/common/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/dev/belGraphicsDevMeshHolder.h"
#include "resource/belResourceLoader.h"
#include "resource/belResourceTexture.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
void TextRender::initialize(uint32_t max_length)
{
    // @todo: 本当は共用なので ResourceHolder 的な所に置く
    {
        auto resource = res::Loader::GetInstance().loadSyncAs<bel::res::Texture>("Texture/jis_font.btex");
        mTexture.initialize(resource);
    }

    // @todo: シェーダーも共用
    {
        auto resource = res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/DebugText.bsh");

        // @debug: 暫定) パイプライン
        gfx::Pipeline::InitializeArg init_arg;
        init_arg.num_render_target        = 1;
        init_arg.render_target_formats[0] = bel::gfx::TextureFormat::cR16G16B16A16_Float; // @todo: 暫定
        init_arg.num_texture              = 1;
        init_arg.num_constant_buffer      = 1;
        mPipeline.initialize(init_arg, resource);
    }

    // @todo: 定数バッファー
    {
        gfx::ConstantBuffer::InitializeArg init_arg;
        init_arg.num_buffer  = 2;
        init_arg.buffer_size = sizeof(InstancesCB);
        mConstantBuffer.initialize(init_arg);
    }
}
//-----------------------------------------------------------------------------
// process
//-----------------------------------------------------------------------------
void TextRender::draw(gfx::CommandContext& command, const char* text, const Vector2& position, float size) const
{
    // @todo: レイアウトサイズは仮で 1920 x 1080
    constexpr Vector2 cLayoutSizeInv(1.f / 1920.f, 1.f / 1080.f);

    // @todo: このタイミングはまずいけどひとまず定数バッファー更新
    uint32_t num_instance = 0;
    Vector2 offset;
    {
        mConstantBuffer.swapBuffer();
        InstancesCB& cb = mConstantBuffer.getStruct<InstancesCB>();
        cb.size = size * 0.16f;

        // ASCII
        size_t text_buffer_index = 0;
        while (text[text_buffer_index] != '\0')
        {
            bool    not_supported = false;
            float   font_scale = 1.f;
            uint8_t x          = 0;
            uint8_t y          = 0;

            unsigned char c = text[text_buffer_index];
            if (c == 0x0A)
            {
                // \n
                offset.x() = 0.f;
                offset.y() -= 0.16f;
                not_supported = true;
            }
            else if (c < 0x20)
            {
                // コード
                not_supported = true;
            }
            else if (c < 0x7F)
            {
                // ASCII (0x20 ~ 0x7F)
                x = static_cast<uint8_t>(c - 0x20);
                y = 0;
                font_scale = 0.5f;
            }
            else if (c <= 0x9F)
            {
                // 日本語（マルチバイト）
                ++text_buffer_index;
                unsigned char c2 = text[text_buffer_index];

                if (c == 0x80)
                {
                    // 未使用
                    not_supported = true;
                }
                else if (c <= 0x84)
                {
                    // 0x8140 ~ 0x81FF
                    // 0x8240 ~ 0x82FF
                    // 0x8340 ~ 0x83FF
                    // 0x8440 ~ 0x84FF
                    if (c2 < 0x40)
                    {
                        // 未使用
                        not_supported = true;
                    }
                    else
                    {
                        x = c2 % 0x40;
                        y = (c - 0x81) * 3 + (c2 - 0x40) / 0x40 + 2;
                    }
                }
                else if (c <= 0x86)
                {
                    // 未使用
                    not_supported = true;
                }
                else
                {
                    // 0x8740 ~ 0x87FF
                    // 0x8840 ~ 0x88FF
                    // ...
                    // 0x9F40 ~ 0x9FFF
                    x = c2 % 0x40;
                    y = (c - 0x87) * 3 + (c2 - 0x40) / 0x40 + 14;
                }
            }
            else if (c <= 0xDF)
            {
                // 半角日本語 (0xA0 ~ 0xDF)
                x = static_cast<uint8_t>(c - 0xA0);
                y = 1;
                font_scale = 0.5f;
            }
            else
            {
                // 日本語（マルチバイト）
                ++text_buffer_index;
                unsigned char c2 = text[text_buffer_index];

                if (c <= 0xEE)
                {
                    // 0xE040 ~ 0xE0FF
                    // 0xE140 ~ 0xE1FF
                    // ...
                    // 0xEE40 ~ 0xEEFF
                    if (c2 < 0x40)
                    {
                        // 未使用
                        not_supported = true;
                    }
                    else
                    {
                        x = c2 % 0x40;
                        y = (c - 0xE0) * 3 + (c2 - 0x40) / 0x40 + 86;
                    }
                }
                else if (c <= 0xF9)
                {
                    // 未使用
                    not_supported = true;
                }
                else if (c <= 0xFC)
                {
                    // 0xFA40 ~ 0xFAFF
                    // 0xFB40 ~ 0xFBFF
                    // 0xFC40 ~ 0xFCFF
                    if (c2 < 0x40)
                    {
                        // 未使用
                        not_supported = true;
                    }
                    else
                    {
                        x = c2 % 0x40;
                        y = (c - 0xFA) * 3 + (c2 - 0x40) / 0x40 + 119;
                    }
                }
                else
                {
                    // 未使用
                    not_supported = true;
                }
            }
            ++text_buffer_index;

            // サポート外ならスキップ
            if (not_supported) { continue; }

            // UBO 計算
            InstanceCB& instance = cb.instances[num_instance++];
            instance.position = position + offset;
            instance.uv_scale.x()  = font_scale / 64.f;
            instance.uv_scale.y()  = 1.f / 128.f;
            instance.uv_offset.x() = x * instance.uv_scale.x();
            instance.uv_offset.y() = y * instance.uv_scale.y();
            instance.size.x() = font_scale;
            instance.size.y() = 1.f;

            // カーソル位置
            offset.x() += 0.1f * font_scale;
        }
    }

    // クラス
    bel::gfx::DynamicDescriptorHeap& descriptor_heap = bel::gfx::DynamicDescriptorHeap::GetInstance();

    // 描画
    {
        gfx::DynamicDescriptorHandle handle = descriptor_heap.allocate(mPipeline.getNumDescriptor());

        mPipeline.activateConstantBuffer(handle, 0, mConstantBuffer);
        mPipeline.activateTexture(handle, 0, mTexture);
        mPipeline.setPipeline(command);
        descriptor_heap.setDescriptorHeap(handle, command);
        gfx::dev::MeshHolder::GetInstance().getMesh(gfx::dev::MeshHolder::Type::cQuad).drawIndexedInstanced(command, num_instance);
    }
}
//-----------------------------------------------------------------------------
}
