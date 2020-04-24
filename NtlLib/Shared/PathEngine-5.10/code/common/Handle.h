//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COMMON_HANDLE_INCLUDED
#define COMMON_HANDLE_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/algorithm.h"

template <typename T>
class cHandle
{
    T* _ptr;

public:
    
    cHandle(T* p = 0)
    {
        _ptr = p;
        if(_ptr)
        {
            _ptr->incRef();
        }
    }
    cHandle(const cHandle<T>& rhs)
    {
        _ptr = rhs._ptr;
        if(_ptr)
        {
            _ptr->incRef();
        }
    }

    ~cHandle()
    {
        if(_ptr)
        {
            _ptr->decRef();
        }
    }
    
    cHandle& operator=(T* rhs)
    {
        if(_ptr != rhs)
        {
            if(rhs)
            {
                rhs->incRef();
            }

            T* toDecRef = _ptr;
            _ptr = rhs;
            if(toDecRef)
            {
                toDecRef->decRef();
            }            
        }
        return *this;
    }
    cHandle& operator=(const cHandle<T>& rhs)
    {
        if(_ptr != rhs._ptr)
        {
            if(rhs._ptr)
            {
                rhs._ptr->incRef();
            }

            T* toDecRef = _ptr;
            _ptr = rhs._ptr;
            if(toDecRef)
            {
                toDecRef->decRef();
            }
        }
        return *this;
    }

    T* get() const
    {
        return _ptr;
    }
    T* operator->() const
    {
        assertD(_ptr);
        return _ptr;
    }
    T& operator* () const
    {
        assertD(_ptr);
        return *_ptr;
    }

    //... get rid of this
    operator bool() const
    {
        return _ptr != 0;
    }

    bool isSet() const
    {
        return _ptr != 0;
    }
    void swap(cHandle<T>& rhs)
    {
        std::swap(_ptr, rhs._ptr);
    }
};

template <class T, class U>
inline bool operator==(const cHandle<T>& lhs, const cHandle<U>& rhs)
{
    T* l = lhs.get();
    U* r = rhs.get();
    if(l && r)
    {
        return *l == *r;
    }
    else
    {
        return !l && !r;
    }        
}

template <class T, class U>
inline bool operator!=(const cHandle<T>& lhs, const cHandle<U>& rhs)
{
    T* l = lhs.get();
    U* r = rhs.get();
    if(l && r)
    {
        return *l != *r;
    }
    else
    {
        return l || r;
    }        
}

template <class T, class U>
inline bool operator<(const cHandle<T>& lhs, const cHandle<U>& rhs)
{
    T* l = lhs.get();
    U* r = rhs.get();
    if(l && r)
    {
        return *l < *r;
    }
    else
    {
        return !l && r;
    }
}

#endif
