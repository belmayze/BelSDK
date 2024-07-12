/*!
 * @file   belMath.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
class Math
{
    //-------------------------------------------------------------------------
    // 定数
    //-------------------------------------------------------------------------
public:
    //! 円周率
    static constexpr float cPi = 3.1415927f;

    //-------------------------------------------------------------------------
    // 関数
    //-------------------------------------------------------------------------
public:
    /*!
     * 切り上げ
     * @note C++23 から constexpr にできる
     * @param[in] a     入力
     * @param[in] times 倍数
     */
    static inline int Roundup(float a, float times)
    {
        float t = a / times;
        return static_cast<int>(std::ceil(t) * times);
    }
    static inline int Roundup(int a, int times)
    {
        float t = static_cast<float>(a) / static_cast<float>(times);
        return static_cast<int>(std::ceil(t) * times);
    }
    static inline size_t Roundup(size_t a, size_t times)
    {
        double t = static_cast<double>(a) / static_cast<double>(times);
        return static_cast<size_t>(std::ceil(t) * times);
    }

};
//-----------------------------------------------------------------------------
}

// math includes
#include "math/belColor.h"
#include "math/belVector2.h"
#include "math/belVector3.h"
#include "math/belVector4.h"
