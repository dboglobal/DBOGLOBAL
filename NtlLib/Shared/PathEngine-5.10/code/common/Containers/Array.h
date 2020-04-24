//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ARRAY_INCLUDED
#define ARRAY_INCLUDED

template <class T>
class cArray
{
    T* _data;
    tSigned32 _size;

public:

    cArray(tSigned32 size)
    {
        _data = new T[size];
        _size = size;
    }
    cArray(tSigned32 size, const T& fillWith)
    {
        _data = new T[size];
        _size = size;
        tSigned32 i;
        for(i = 0; i < _size; i++)
        {
            _data[i] = fillWith;
        }
    }
    ~cArray()
    {
        delete [] _data;
    }

    T& front()
    {
        return _data[0];
    }
    const T& front() const
    {
        return _data[0];
    }
    T& back()
    {
        return _data[_size - 1];
    }
    const T& back() const
    {
        return _data[_size - 1];
    }

    T& operator[](tSigned32 index)
    {
        return _data[index];
    }
    const T& operator[](tSigned32 index) const
    {
        return _data[index];
    }

    tSigned32 size() const
    {
        return _size;
    }
};

#endif
