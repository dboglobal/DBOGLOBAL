//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LAZY_HANDLE_VECTOR_INCLUDED
#define LAZY_HANDLE_VECTOR_INCLUDED

#include "common/Handle.h"
#include "common/STL/vector.h"
#include "common/STL_Helpers.h"

template <class T>
class cLazyHandleVector
{
    std::vector<cHandle<T> > _vector;

public:

    void
    completelyClear()
    {
        CompletelyClear(_vector);
    }

    void set(tSigned32 index, T* value)
    {
        if(index >= SizeL(_vector))
        {
            if(value == 0)
            {
                return;
            }
            _vector.resize(index + 1, 0);
        }
        _vector[index] = value;
    }
    cHandle<T> get(tSigned32 index) const
    {
        if(index >= SizeL(_vector))
        {
            return 0;
        }
        return _vector[index];
    }
    tSigned32 size() const
    {
        return SizeL(_vector);
    }
};

#endif
