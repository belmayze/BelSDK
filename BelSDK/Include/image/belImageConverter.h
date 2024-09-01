/*!
 * @file   belImageConverter.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "image/belImage.h"

namespace bel::img {
//-----------------------------------------------------------------------------
// 画像コンバート
//-----------------------------------------------------------------------------
class Converter
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 画像を任意のフォーマットに変換します
     * @param[in] image  入力画像
     * @param[in] format 変換フォーマット
     */
    static Image ConvertFormat(const Image& input, gfx::TextureFormat format);

     //-------------------------------------------------------------------------
private:
};
//-----------------------------------------------------------------------------
}
