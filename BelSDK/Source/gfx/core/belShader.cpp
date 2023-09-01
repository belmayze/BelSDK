/*!
 * @file   belShader.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belShader.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
void Shader::initializeVertexPixel(void* vs_ptr, size_t vs_size, void* ps_ptr, size_t ps_size)
{
    VertexPixel& object = mShaderObject.emplace<VertexPixel>();
    object.mVertexShaderPtr  = vs_ptr;
    object.mVertexShaderSize = vs_size;
    object.mPixelShaderPtr   = ps_ptr;
    object.mPixelShaderSize  = ps_size;
}
//-----------------------------------------------------------------------------
void Shader::initializeCompute(void* ptr, size_t size)
{
    Compute& object = mShaderObject.emplace<Compute>();
    object.mComputeShaderPtr  = ptr;
    object.mComputeShaderSize = size;
}
//-----------------------------------------------------------------------------
void Shader::initializeAmplificationMesh(void* as_ptr, size_t as_size, void* ms_ptr, size_t ms_size)
{
    AmplificationMesh& object = mShaderObject.emplace<AmplificationMesh>();
    object.mAmplificationShaderPtr  = as_ptr;
    object.mAmplificationShaderSize = as_size;
    object.mMeshShaderPtr           = ms_ptr;
    object.mMeshShaderSize          = ms_size;
}
//-----------------------------------------------------------------------------
}
