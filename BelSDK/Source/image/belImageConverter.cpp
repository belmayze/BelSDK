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
        init_arg.array_size = input.getArraySize();
        init_arg.format     = format;
        if (!output.initialize2D(init_arg))
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
        init_arg.array_size = input.getArraySize();
        init_arg.format     = format;
        if (!output.initialize2D(init_arg))
        {
            return Image();
        }

        // @TODO: コンバート

    }

    BEL_ERROR("変換に対応していないフォーマットです");
    return Image();
}
//-----------------------------------------------------------------------------
}