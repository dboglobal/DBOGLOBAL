//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LAZY_POINTER_VECTOR_INCLUDED
#define LAZY_POINTER_VECTOR_INCLUDED

#include "common/STL/vector.h"

template <class T>
class cLazyPointerVector
{
    std::vector<T*> _vector;

public:

    cLazyPointerVector()
    {
    }

//... would be nice to manage object deletion, but introducies dependencies on destructors
//    ~cLazyPointerVector()
//    {
//        tSigned32 i;
//        for(i = 0; i < _vector.size(); ++i)
//        {
//            delete _vector[i];
//        }
//    }
    ~cLazyPointerVector()
    {
        tSigned32 i;
        for(i = 0; i < SizeL(_vector); ++i)
        {
            assertD(_vector[i] == 0);
        }
    }

    void
    deleteAll()
    {
        tSigned32 i;
        for(i = 0; i < SizeL(_vector); ++i)
        {
            delete _vector[i];
            _vector[i] = 0;
        }
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
        delete _vector[index];
        _vector[index] = value;
    }

    T* operator[](tSigned32 index) const
    {
        if(index >= SizeL(_vector))
        {
            return 0;
        }
        return _vector[index];
    }
    tSigned32 size() const
    {
        return _vector.size();
    }
};

//template <class T>
//class cLazyHandleVector
//{
//    std::vector<cHandle<T> > _vector;
//
//public:
//
//    void
//    zeroAll()
//    {
//        tSigned32 i;
//        for(i = 0; i < _vector.size(); ++i)
//        {
//            _vector[i] = 0;
//        }
//    }
//
//    void set(tSigned32 index, cHandle<T> value)
//    {
//        if(index >= _vector.size())
//        {
//            if(value == 0)
//            {
//                return;
//            }
//            _vector.resize(index + 1, 0);
//        }
//        _vector[index] = value;
//    }
//
//    cHandle<T> operator[](tSigned32 index) const
//    {
//        if(index >= _vector.size())
//        {
//            return 0;
//        }
//        return _vector[index];
//    }
//    tSigned32 size() const
//    {
//        return _vector.size();
//    }
//};

#endif
