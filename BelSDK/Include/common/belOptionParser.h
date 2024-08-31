/*!
 * @file   belOptionParser.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <cassert>
#include <memory>
#include <string>

namespace bel {

//-----------------------------------------------------------------------------
class OptionParser
{
    //-------------------------------------------------------------------------
public:
    using OptionValue = std::pair<std::string, std::string>;

    //-------------------------------------------------------------------------
    // parse
    //-------------------------------------------------------------------------
public:
    /*!
     * 文字列からオプションをパースします
     * @param[in] argc
     * @param[in] argv
     */
    void parse(int argc, const char* argv[]);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! オプションの個数を取得
    uint32_t getNumOption() const { return mNumOption; }

    //! オプションを取得
    const OptionValue& getOption(uint32_t index) const
    {
        assert(mpOptions.get() != nullptr);
        return mpOptions[index];
    }

    //-------------------------------------------------------------------------
private:
    uint32_t                       mNumOption = 0;
    std::unique_ptr<OptionValue[]> mpOptions;
};
//-----------------------------------------------------------------------------
} // bel::
