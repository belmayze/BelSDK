/*!
 * @file   belImageConverter.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "image/belImageConverter.h"

namespace bel::img {
//-----------------------------------------------------------------------------
Image Converter::ConvertFormat(const Image& input, gfx::TextureFormat format)
{
    // 同じフォーマットだった場合はコピーして返す
    if (input.getFormat() == format)
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

    // @TODO: 異なるフォーマットの場合は変換します
    Image output;

    return output;
}
//-----------------------------------------------------------------------------
}