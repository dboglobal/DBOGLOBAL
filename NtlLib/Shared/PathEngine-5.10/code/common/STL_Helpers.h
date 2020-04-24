//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef STL_HELPERS_INCLUDED
#define STL_HELPERS_INCLUDED

#include "common/STL/algorithm.h"
#include "common/STL/vector.h"

template <class T, class T2> void
Fill(T& container, T2 value)
{
    std::fill(container.begin(),container.end(),value);
}

template <class T, class T2> bool
Contains(T& container, T2 value)
{
    return std::find(container.begin(),container.end(),value)!=container.end();
}

template <class T> void
ShrinkToFit(T& container)
{
    if(container.capacity() != container.size())
    {
        T(container).swap(container);
    }
}
template <class T> void
ShrinkToFit_Compound(T& container)
{
    T(container).swap(container);
}
template <class T> void
CompletelyClear(T& container)
{
    if(container.capacity() != 0)
    {
        T().swap(container);
    }
}

template <class T> T*
VectorBuffer(std::vector<T>& v)
{
    if(v.empty())
    {
        return 0;
    }
    return &v.front();
}
template <class T> const T*
VectorBuffer(const std::vector<T>& v)
{
    if(v.empty())
    {
        return 0;
    }
    return &v.front();
}

//... not currently used
//template <class T> tSigned32
//FindInVector(const T& v, const T& value)
//{
//    tSigned32 i;
//    for(i = 0; i != SizeL(v); ++i)
//    {
//        if(v[i] == value)
//        {
//            return i;
//        }
//    }
//    return -1;
//}

#endif
