/*!
 * @file   belImageConverter.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsTextureFormatInfo.h"
#include "image/belImageConverter.h"

namespace
{

/*!
 * スキャンラインをコンバートする
 * @param[out] output 出力先
 * @param[in]  input  入力元
 * @param[in]  
 */

}

namespace bel::img {
//-----------------------------------------------------------------------------
Image Converter::ConvertFormat(const Image& input, gfx::TextureFormat format)
{
    // 同じフォーマット
    bool is_same = (input.getFormat() == format);
    // sRGB 違いならそのまま返す
    switch (input.getFormat())
    {
    case gfx::TextureFormat::cR8G8B8A8_sRGB:
    case gfx::TextureFormat::cR8G8B8A8_uNorm:
        is_same = is_same || format == gfx::TextureFormat::cR8G8B8A8_sRGB || format == gfx::TextureFormat::cR8G8B8A8_uNorm;
        break;
    }

    // 同じフォーマットだった場合はコピーして返す
    if (is_same)
    {
        // 画像生成
        Image output;
        Image::InitializeArg init_arg;
        init_arg.width      = input.getWidth();
        init_arg.height     = input.getHeight();
        init_arg.mip_levels = input.getMipLevels();
        init_arg.depth      = input.getDepth();
        init_arg.format     = format;
        init_arg.component_mapping = input.getComponentMapping();
        if (!output.initialize(init_arg))
        {
            return Image();
        }

        // コピー
        std::memcpy(output.getMemoryPtr(0), input.getMemoryPtr(0), input.getMemorySize());

        return output;
    }

    // 異なるフォーマットの場合は変換します
    {
        Image output;

        Image::InitializeArg init_arg;
        init_arg.width      = input.getWidth();
        init_arg.height     = input.getHeight();
        init_arg.mip_levels = input.getMipLevels();
        init_arg.depth      = input.getDepth();
        init_arg.format     = format;
        init_arg.component_mapping = gfx::TextureFormatInfo::DefaultComponentMapping(format);
        if (!output.initialize(init_arg))
        {
            return Image();
        }

        // RGBA
        if (input.getFormat() == gfx::TextureFormat::cR8G8B8A8_uNorm ||
            input.getFormat() == gfx::TextureFormat::cR8G8B8A8_sRGB)
        {
            if (format == gfx::TextureFormat::cR8_uNorm)
            {
                // RGBA -> R
                for (size_t i_depth = 0; i_depth < input.getDepth(); ++i_depth)
                {
                    for (size_t i_mip = 0; i_mip < input.getMipLevels(); ++i_mip)
                    {
                        // ミップマップのメモリー取得
                        const Image::ImageProperty& mip_property = input.getImageProperty(i_mip);
                        const uint8_t* src_ptr = input.getMemoryPtr(mip_property.memory_offset);
                        uint8_t*       dst_ptr = output.getMemoryPtr(output.getImageProperty(i_mip).memory_offset);

                        // 変換
                        for (size_t i_height = 0; i_height < mip_property.height; ++i_height)
                        {
                            for (size_t i_width = 0; i_width < mip_property.width; ++i_width)
                            {
                                *dst_ptr = *src_ptr;
                                dst_ptr += 1;
                                src_ptr += 4;
                            }
                        }
                    }
                }
            }
            return output;
        }

        // @TODO: コンバート

    }

    BEL_ERROR_LOG("変換に対応していないフォーマットです");
    return Image();
}
//-----------------------------------------------------------------------------
}