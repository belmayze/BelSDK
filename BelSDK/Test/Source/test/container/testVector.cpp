/*!
 * @file   testVector.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "test/container/testVector.h"

// container
#include "container/belVector.h"

//-----------------------------------------------------------------------------
namespace {

class UintClass
{
public:
    UintClass(uint32_t x) : x(x) {  }
    ~UintClass() {  }

    UintClass(const UintClass& rhs) : x(rhs.x) {  }
    UintClass(UintClass&& rhs) : x(rhs.x) {  }
public:
    operator uint32_t() const { return x; }
private:
    uint32_t x = 0;
};

}

namespace app::test {
//-----------------------------------------------------------------------------
void Vector::test()
{
    bel::Vector<UintClass> buffer(3);
    BEL_ASSERT(buffer.empty() && buffer.capacity() >= 3);

    // 追加チェック
    buffer.push_back(0);
    buffer.emplace_back(1);
    buffer.push_back(2);
    BEL_ASSERT(buffer[0] == 0 && buffer.at(1) == 1 && buffer[2] == 2);
    BEL_ASSERT(buffer.size() == 3);

    // 削除チェック
    buffer.pop_back();
    buffer.pop_back();
    BEL_ASSERT(buffer[0] == 0);
    BEL_ASSERT(buffer.size() == 1);

    // 循環チェック
    buffer.push_back(3);
    buffer.push_back(4);
    BEL_ASSERT(buffer[0] == 0 && buffer[1] == 3 && buffer[2] == 4);
    BEL_ASSERT(buffer.size() == 3);

    // イテレーター
    BEL_DEBUG_LOG("  iterator[");
    for (auto& c : buffer)
    {
        BEL_DEBUG_LOG("%d, ", c);
    }
    BEL_DEBUG_LOG("] (size: %zu)\n", buffer.size());

    // イテレーター
    BEL_DEBUG_LOG("  reverse iterator[");
    for (auto it = buffer.crbegin(); it != buffer.crend(); ++it)
    {
        BEL_DEBUG_LOG("%d, ", *it);
    }
    BEL_DEBUG_LOG("] (size: %zu)\n", buffer.size());

    // クリア
    buffer.clear();
    BEL_ASSERT(buffer.size() == 0);
    BEL_ASSERT(buffer.empty());
}
//-----------------------------------------------------------------------------
}
