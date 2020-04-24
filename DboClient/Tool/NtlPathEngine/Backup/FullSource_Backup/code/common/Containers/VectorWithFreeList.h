//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_WITH_FREE_LIST_INCLUDED
#define VECTOR_WITH_FREE_LIST_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL_Helpers.h"

template <class T>
class cVectorWithFreeList
{
    std::vector<T> _v;
    tSigned32 _firstFree;
    std::vector<tSigned32> _nextFree;
    tSigned32 _numberInUse;

public:

    cVectorWithFreeList()
    {
        _firstFree = -1;
        _numberInUse = 0;
    }

    void
    completelyClear()
    {
        CompletelyClear(_v);
        CompletelyClear(_nextFree);
        _firstFree = -1;
        _numberInUse = 0;
    }

    tSigned32 size() const
    {
        return SizeL(_v);
    }
    const T& operator[](tSigned32 index) const
    {
        return _v[index];
    }
    T& operator[](tSigned32 index)
    {
        return _v[index];
    }

    bool indexIsInUse(tSigned32 index) const
    {
        assertD(index >= 0);
        assertD(index < static_cast<tSigned32>(_v.size()));
        return _nextFree[index] == -2;
    }
    
    tSigned32 numberInUse() const
    {
        return _numberInUse;
    }

    tSigned32 reuseOrPushBack(const T& value)
    {
        tSigned32 result;
        if(_firstFree == -1)
        {
            result = SizeL(_v);
            _v.push_back(value);
            _nextFree.push_back(-2);
        }
        else
        {
            result = _firstFree;
            assertD(_nextFree[result] >= -1);
            _firstFree = _nextFree[result];
            _nextFree[result] = -2;
            _v[result] = value;
        }
        _numberInUse++;
        return result;
    }
    void release(tSigned32 index)
    {
        assertD(_nextFree[index] == -2);
        _nextFree[index] = _firstFree;
        _firstFree = index;
        _numberInUse--;
    }
};

#endif
