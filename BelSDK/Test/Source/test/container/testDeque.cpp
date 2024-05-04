/*!
 * @file   testDeque.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "test/container/testDeque.h"

// container
#include "container/belDeque.h"

//-----------------------------------------------------------------------------
namespace {

class UintClass
{
public:
    UintClass(uint32_t x) : x(x) {  }
    ~UintClass() {  }

    UintClass(const UintClass& rhs) : x(rhs.x) {  }
    UintClass(UintClass&& rhs) noexcept : x(rhs.x) {  }
public:
    operator uint32_t() const { return x; }
private:
    uint32_t x = 0;
};

}

namespace app::test {
//-----------------------------------------------------------------------------
void Deque::test()
{
    bel::Deque<UintClass> buffer(3);
    BEL_ASSERT(buffer.empty() && buffer.max_size() == 3);

    // 追加チェック
    buffer.push_back(0);
    buffer.emplace_back(1);
    buffer.push_back(2);
    BEL_ASSERT(buffer[0] == 0 && buffer.at(1) == 1 && buffer[2] == 2);
    BEL_ASSERT(buffer.size() == 3);

    // 削除チェック
    buffer.pop_front();
    buffer.pop_front();
    BEL_ASSERT(buffer[0] == 2);
    BEL_ASSERT(buffer.size() == 1);

    // 循環チェック
    buffer.push_back(3);
    BEL_ASSERT(buffer[0] == 2 && buffer[1] == 3);
    BEL_ASSERT(buffer.size() == 2);

    // 先頭追加チェック
    buffer.push_front(4);
    BEL_ASSERT(buffer[0] == 4 && buffer[1] == 2 && buffer[2] == 3);
    BEL_ASSERT(buffer.size() == 3);

    // イテレーター
    BEL_PRINT("  iterator[");
    for (auto& c : buffer)
    {
        BEL_PRINT("%d, ", c);
    }
    BEL_PRINT("] (size: %zu)\n", buffer.size());

    // イテレーター
    BEL_PRINT("  reverse iterator[");
    for (auto it = buffer.crbegin(); it != buffer.crend(); ++it)
    {
        BEL_PRINT("%d, ", *it);
    }
    BEL_PRINT("] (size: %zu)\n", buffer.size());

    // クリア
    buffer.clear();
    BEL_ASSERT(buffer.size() == 0);
    BEL_ASSERT(buffer.empty());
}
//-----------------------------------------------------------------------------
}
