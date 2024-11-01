/*!
 * @file   testMatrix.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "test/math/testMatrix.h"

namespace app::test {
//-----------------------------------------------------------------------------
void Matrix::test()
{
#   if 0
    // 計測
    auto start_time = std::chrono::system_clock::now();
    bel::Matrix44 m;
    {
        m.setRow(0, bel::Vector4(0.001f, 0.001f, 0.001f, 0.001f));
        m.setRow(1, bel::Vector4(0.001f, 0.001f, 0.001f, 0.001f));
        m.setRow(2, bel::Vector4(0.001f, 0.001f, 0.001f, 0.001f));
        m.setRow(3, bel::Vector4(0.001f, 0.001f, 0.001f, 0.001f));

        for (uint32_t i = 1; i <= 2000; ++i)
        {
            bel::Matrix44 m0, m1 = m;
            m0.setRow(0, bel::Vector4(i * 0.001f, i * 0.001f, i * 0.001f, i * 0.001f));
            m0.setRow(1, bel::Vector4(i * 0.001f, i * 0.001f, i * 0.001f, i * 0.001f));
            m0.setRow(2, bel::Vector4(i * 0.001f, i * 0.001f, i * 0.001f, i * 0.001f));
            m0.setRow(3, bel::Vector4(i * 0.001f, i * 0.001f, i * 0.001f, i * 0.001f));

            m0.mul(m, m1);
        }
    }
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start_time).count();
    BEL_INFO_LOG("Matrix mul: %d us\n", diff);
    BEL_INFO_LOG("Matrix [0]: %f %f %f %f\n", m.m(0, 0), m.m(0, 1), m.m(0, 2), m.m(0, 3));
    BEL_INFO_LOG("       [1]: %f %f %f %f\n", m.m(1, 0), m.m(1, 1), m.m(1, 2), m.m(1, 3));
    BEL_INFO_LOG("       [2]: %f %f %f %f\n", m.m(2, 0), m.m(2, 1), m.m(2, 2), m.m(2, 3));
    BEL_INFO_LOG("       [3]: %f %f %f %f\n", m.m(3, 0), m.m(3, 1), m.m(3, 2), m.m(3, 3));

    // 演算テスト
    m.setRow(0, bel::Vector4(1.f, 2.f, 3.f, 4.f));
    m.setRow(1, bel::Vector4(5.f, 6.f, 7.f, 8.f));
    m.setRow(2, bel::Vector4(9.f, 10.f, 11.f, 12.f));
    m.setRow(3, bel::Vector4(13.f, 14.f, 15.f, 16.f));
    {
        bel::Matrix44 m0 = m, m1 = m;
        m0.mul(m, m1);
    }
    BEL_INFO_LOG("Matrix [0]: %f %f %f %f\n", m.m(0, 0), m.m(0, 1), m.m(0, 2), m.m(0, 3));
    BEL_INFO_LOG("       [1]: %f %f %f %f\n", m.m(1, 0), m.m(1, 1), m.m(1, 2), m.m(1, 3));
    BEL_INFO_LOG("       [2]: %f %f %f %f\n", m.m(2, 0), m.m(2, 1), m.m(2, 2), m.m(2, 3));
    BEL_INFO_LOG("       [3]: %f %f %f %f\n", m.m(3, 0), m.m(3, 1), m.m(3, 2), m.m(3, 3));
#   elif 0
    // 逆行列テスト
    bel::Matrix44 m(
        2.0f, 0.5f, 0.2f, 0.1f,
        0.1f, 1.5f, 0.6f, 0.7f,
        0.9f, 0.8f, 2.2f, 0.5f,
        0.1f, 0.5f, 0.3f, 2.f
    );
    bel::Matrix44 m_inv;
    m.inverse(m_inv);
    bel::Matrix44 c;
    m.mul(c, m_inv);

    BEL_INFO_LOG("Matrix [0]: %f %f %f %f\n", c.m(0, 0), c.m(0, 1), c.m(0, 2), c.m(0, 3));
    BEL_INFO_LOG("       [1]: %f %f %f %f\n", c.m(1, 0), c.m(1, 1), c.m(1, 2), c.m(1, 3));
    BEL_INFO_LOG("       [2]: %f %f %f %f\n", c.m(2, 0), c.m(2, 1), c.m(2, 2), c.m(2, 3));
    BEL_INFO_LOG("       [3]: %f %f %f %f\n", c.m(3, 0), c.m(3, 1), c.m(3, 2), c.m(3, 3));
#   endif
}
//-----------------------------------------------------------------------------
}
