/*!
 * @file   testApplication.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "base/belIApplicationCallback.h"
#include "resource/belResourceShaderArchive.h"

namespace app::test {
//-----------------------------------------------------------------------------
class Application : public bel::IApplicationCallback
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     */
    void initialize();

    //-------------------------------------------------------------------------
    // callback
    //-------------------------------------------------------------------------
public:
    /*!
     * 描画コマンド生成前の計算タイミングで呼ばれます
     */
    virtual void onCalc() override;

    /*!
     * 描画コマンドを生成するタイミングで呼ばれます
     * @param[in] command_context コマンドコンテキスト
     */
    virtual void onMakeCommand(bel::gfx::CommandContext& command_context) const override;

private:
    bel::res::ShaderArchive mResShaderArchive;
};
//-----------------------------------------------------------------------------
}
