/*!
 * @file   belUtilEnum.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {

//! enum のキャスト
template <typename T>
constexpr std::underlying_type_t<T> to_underlying(T t)
{
    static_assert(std::is_enum_v<T>, "to_underlying only used to enum type");
    return static_cast<std::underlying_type_t<T>>(t);
}

}
