//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BIT_VECTOR_INCLUDED
#define BIT_VECTOR_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL_Helpers.h"

class cBitVector
{
    tSigned32 _size;
    std::vector<unsigned char> _v;

    static tSigned32 byteSize(tSigned32 size)
    {
        return (size + 7) / 8;
    }
    static tSigned32 byteIndex(tSigned32 index)
    {
        return index / 8;
    }
    static unsigned char maskFor(tSigned32 index)
    {
        return 1 << (index & 7);
    }

public:

    cBitVector() :
      _size(0)
    {
    }

    cBitVector(tSigned32 size) :
     _size(size),
     _v(byteSize(size))
    {
    }
    cBitVector(tSigned32 size, bool initialiser) :
     _size(size),
     _v(byteSize(size), initialiser ? 0xff : 0)
    {
    }

    void
    clear()
    {
        _v.resize(0);
        _size = 0;
    }
    void
    completelyClear()
    {
        CompletelyClear(_v);
    }

    void swap(cBitVector& rhs)
    {
        std::swap(_size, rhs._size);
        _v.swap(rhs._v);
    }

    bool empty() const {return _size == 0;}
    tSigned32 size() const {return _size;}

    bool operator[](tSigned32 i) const
    {
        return (_v[byteIndex(i)] & maskFor(i)) != 0;
    }

    void push_back(bool value)
    {
        _v.resize(byteSize(_size + 1));
        _size++;
        setBit(_size - 1, value);
    }
    void pop_back()
    {
        _size--;
        _v.resize(byteSize(_size));
    }
    void resize(tSigned32 newSize)
    {
        _v.resize(byteSize(newSize));
        _size = newSize;
    }
    void resize(tSigned32 newSize, bool initialiser)
    {
        tSigned32 sizeBefore = _size;
        _v.resize(byteSize(newSize), initialiser ? 0xff : 0);
        _size = newSize;
        while((sizeBefore & 7) && sizeBefore < newSize)
        {
            setBit(sizeBefore, initialiser);
            sizeBefore++;
        }
    }

    void setBit(tSigned32 i, bool value)
    {
        if(value)
        {
            _v[byteIndex(i)] |= maskFor(i);
        }
        else
        {
            unsigned char inverseMask = ~maskFor(i);
            _v[byteIndex(i)] &= inverseMask;
        }
    }
    void setBit(tSigned32 i)
    {
        _v[byteIndex(i)] |= maskFor(i);
    }
    void setLastBit(bool value)
    {
        setBit(_size - 1, value);
    }
    void fill(bool value)
    {
//        tUnsigned32 i;
//        unsigned char fillWith = value ? 0xff : 0;
//        for(i = 0; i != _v.size(); ++i)
//        {
//            _v[i] = fillWith;
//        }
        _v.assign(_v.size(),value ? 0xff : 0);
    }
};

#endif
