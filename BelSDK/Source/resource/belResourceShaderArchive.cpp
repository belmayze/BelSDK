/*!
 * @file   belResourceShaderArchive.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResourceShaderArchive.h"

namespace bel::res {
//-----------------------------------------------------------------------------
// resource
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
ShaderArchive ShaderArchiveFactory::MakeResource(Resource&& resource)
{
    return ShaderArchive();
}
//-----------------------------------------------------------------------------

} // bel::res::

// loadSyncAs 特殊化
namespace bel::res {

template <>
ShaderArchive Loader::loadSyncAs<ShaderArchive>(const std::string& filepath)
{
    return ShaderArchiveFactory::MakeResource(loadSync(filepath));
}

}
