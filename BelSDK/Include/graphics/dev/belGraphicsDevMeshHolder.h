/*!
 * @file   belGraphicsDevMeshHolder.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsMesh.h"
#include "memory/belSingleton.h"

namespace bel::gfx::dev {
//-----------------------------------------------------------------------------
// 開発用の基礎メッシュを保持するクラス
//-----------------------------------------------------------------------------
class MeshHolder : public Singleton<MeshHolder>
{
    //-------------------------------------------------------------------------
public:
    //! メッシュの種別
    enum class Type
    {
        cTriangle,
        cQuad,
        cCube,
        cSphere,
        cSphereIcosahedron,
        cNum
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * メッシュを初期化します
     * @param[in] arg 初期化引数
     */
    bool initialize();

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    /*!
     * メッシュを取得します
     * @param[in] type 種別
     */
    const Mesh& getMesh(Type type) const { return mMeshes[to_underlying(type)]; }

    //-------------------------------------------------------------------------
private:
    //
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 texcoord;
    };

     //-------------------------------------------------------------------------
private:
    std::array<Mesh, to_underlying(Type::cNum)> mMeshes;
};
//-----------------------------------------------------------------------------
}
