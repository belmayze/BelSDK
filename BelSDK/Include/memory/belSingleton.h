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
 * �V���O���g��
 */
template <class T>
class Singleton
{
public:
    /*!
     * �C���X�^���X���擾���܂�
     */
    static T& GetInstance()
    {
        if (!mpInstance) { Create_(); }
        return *mpInstance;
    }

    //-------------------------------------------------------------------------
private:
    /*!
     * �C���X�^���X�𐶐����܂�
     */
    static void Create_()
    {
        mpInstance.reset(new T());
    }
    /*!
     * �C���X�^���X��j�����܂�
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
