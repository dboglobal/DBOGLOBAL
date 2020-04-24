//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FLAG_VECTOR_INCLUDED
#define FLAG_VECTOR_INCLUDED

#include "common/interface/Assert.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"

class cFlagVector
{
    cBitVector _flags;
    std::vector<tSigned32> _list;
    tSigned32 _numberSet;

public:

    cFlagVector(tSigned32 size) :
     _flags(size, false),
     _list(size),
     _numberSet(0)
    {
    }

    tSigned32 size() const
    {
        return static_cast<tSigned32>(_flags.size());
    }
    void resize(tSigned32 newSize)
    {
        assertD(newSize > size());
        _flags.resize(newSize, false);
        _list.resize(newSize);
    }

    void clear()
    {
        tSigned32 i;
        for(i = 0; i < _numberSet; i++)
        {
            _flags.setBit(_list[i], false);
        }
        _numberSet = 0;
    }

    void set(tSigned32 index)
    {
        assertD(index >= 0 && index < SizeL(_flags));
        assertD(!_flags[index]);
        _flags.setBit(index, true);
        _list[_numberSet++] = index;
    }

    void setIfNotAlreadySet(tSigned32 index)
    {
        assertD(index >= 0 && index < SizeL(_flags));
        if(!_flags[index])
        {
            _flags.setBit(index, true);
            _list[_numberSet++] = index;
        }
    }

    bool operator[](tSigned32 index) const
    {
        return _flags[index] != 0;
    }
    tSigned32 numberSet() const
    {
        return _numberSet;
    }
    tSigned32 indexSet(tSigned32 i) const
    {
        return _list[i];
    }
};

#endif
