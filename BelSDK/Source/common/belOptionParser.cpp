/*!
 * @file   belOptionParser.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "common/belOptionParser.h"

namespace bel {

//-----------------------------------------------------------------------------
// parse
//-----------------------------------------------------------------------------
void OptionParser::parse(int argc, const char* argv[])
{
    // メモリー確保
    mNumOption = 0;
    mpOptions  = std::make_unique<OptionValue[]>(argc - 1);

    // 解析
    // @note 先頭には実行ファイルパスが含まれるので除外する
    const char* p_token = nullptr;
    for (int i_arg = 1; i_arg < argc; ++i_arg)
    {
        if (*argv[i_arg] == '-')
        {
            // - から始まればキー
            if (p_token)
            {
                // token があれば、無指定でオプション追加
                mpOptions[mNumOption] = OptionValue(p_token, "");
                ++mNumOption;
            }
            p_token = argv[i_arg];

            // -h の場合はヘルプフラグを立てる
            if (strcmp(p_token, "-h") == 0) { mHasHelp = true; }
        }
        else
        {
            // それ以外はバリュー
            mpOptions[mNumOption] = OptionValue(p_token ? p_token : "", argv[i_arg]);
            ++mNumOption;
            p_token = nullptr;
        }
    }

    // ここで p_token が残ってた時のケア
    if (p_token)
    {
        mpOptions[mNumOption] = OptionValue(p_token, "");
        ++mNumOption;
        p_token = nullptr;
    }
}
//-----------------------------------------------------------------------------

} // bel::res::
