/*!
 * @file   belHidModule.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "common/belIModule.h"

namespace bel::hid { class Controller; }

namespace bel::hid {
//-----------------------------------------------------------------------------
// ヒューマンインタフェースモジュール
//-----------------------------------------------------------------------------
class HidModule : public IModule
{
    //-------------------------------------------------------------------------
public:
    //! 最大コントローラー数
    static constexpr uint32_t cMaxController = 4;

    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 
    HidModule();
    //!
    virtual ~HidModule();

    //-------------------------------------------------------------------------
    // override
    //-------------------------------------------------------------------------
public:
    //! 初期化
    virtual bool initialize() override;

    //! 更新
    virtual void update() override;

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! コントローラーを取得
    const Controller& getController(uint32_t index) const
    {
        BEL_ASSERT(index < cMaxController);
        return mControllers[index];
    }

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<Controller[]> mControllers;
};
//-----------------------------------------------------------------------------
} // bel::hid::
