//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

template <class T, tSigned32 HEAP_SIZE>
class cSimpleHeap
{
    T _heap[HEAP_SIZE];
    tSigned32 _nextFree[HEAP_SIZE];
    tSigned32 _numberFree;

public:

    cSimpleHeap()
    {
        tSigned32 i;
        _numberFree = HEAP_SIZE;
        for(i = 0; i < HEAP_SIZE; i++)
        {
            _nextFree[i] = i;
        }
    }
    T *get()
    {
        if(_numberFree)
        {
            return _heap + _nextFree[--_numberFree];
        }
        return new T;
    }
    void release(T* pointer)
    {
        if(pointer >= _heap && pointer <= _heap + HEAP_SIZE - 1)
        {
            _nextFree[_numberFree++] = static_cast<tSigned32>(pointer - _heap);
        }
        else
        {
            delete pointer;
        }
    }
};
