//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SWAPOUT_INCLUDED
#define SWAPOUT_INCLUDED

#include "common/STL/vector.h"
#include "common/STL/utility.h"
#include "common/interface/Assert.h"

// returns true if the top element was swapped down
// so true returned means the element at i was modified
template <class T> bool
SwapOut(std::vector<T>& v, tSigned32 i)
{
    assertD(i >= 0 && i < SizeL(v));
    if(i != v.size() - 1)
    {
        std::swap(v[i], v.back());
        v.pop_back();
        return true;
    }
    v.pop_back();
    return false;
}

#endif
