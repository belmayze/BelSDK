/*!
 * @file   belSingleton.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <memory>

namespace bel
{
//-----------------------------------------------------------------------------
/*!
 * シングルトン
 */
template <class T>
class Singleton
{
public:
    /*!
     * インスタンスを取得します
     */
    static T& GetInstance()
    {
        if (!mpInstance) { Create_(); }
        return *mpInstance;
    }

    //-------------------------------------------------------------------------
private:
    /*!
     * インスタンスを生成します
     */
    static void Create_()
    {
        mpInstance.reset(new T());
    }
    /*!
     * インスタンスを破棄します
     */
    static void Destroy_()
    {
        mpInstance.reset();
    }

    //-------------------------------------------------------------------------
protected:
    Singleton() {}
private:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    //-------------------------------------------------------------------------
private:
    inline static std::unique_ptr<T> mpInstance;
};
//-----------------------------------------------------------------------------
}
