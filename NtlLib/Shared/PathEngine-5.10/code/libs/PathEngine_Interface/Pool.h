//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POOL_INCLUDED
#define POOL_INCLUDED

#include "common/STL/vector.h"
#include "common/MutexLock.h"

template <class T>
class cPool
{
    cMutex _mutex;
    std::vector<T*> _pool;
    tSigned32 _firstFree;
    std::vector<tSigned32> _nextFree;
    tSigned32 _numberFree; //... just for checking

public:

    // for construction in Singletons.cpp
    //...... make these private again
    cPool(tSigned32 size)
    {
        _pool.resize(size);
        _nextFree.resize(size);
        tSigned32* ptr_to_index=&_firstFree;        
        tSigned32 i;
        for(i = 0; i < size; i++)
        {
            _pool[i] = new T;
            _pool[i]->setPoolIndex(i);
            *ptr_to_index = i;
            ptr_to_index = &_nextFree[i];
        }
        *ptr_to_index = -1;
        _numberFree = size;
    }
    ~cPool()
    {
        assertD(_numberFree == _pool.size());
        tSigned32 i;
        for(i = 0; i < SizeL(_pool); i++)
        {
            _pool[i]->setPoolIndex(-1);
            delete _pool[i];
        }
    }

    void resize(tSigned32 newSize)
    {
        cMutexLock lock(_mutex);
        tSigned32 i;
        tSigned32 oldSize = SizeL(_pool);
        if(newSize >= oldSize)
        {
            _pool.resize(newSize);
            _nextFree.resize(newSize);
            for(i = oldSize; i < newSize; i++)
            {
                _pool[i] = new T;
                _pool[i]->setPoolIndex(i);
                _nextFree[i] = _firstFree;
                _firstFree = i;
                _numberFree++;
            }
        }
        else
        {
          // mark all above new size as no longer being managed by the pool
            for(i = newSize; i < oldSize; i++)
            {
                assertD(_pool[i]->getPoolIndex() == i);
                _pool[i]->setPoolIndex(-1);
            }

          // iterate through free list
          // all currently free above the new size get deleted and removed from free list
            tSigned32* ptr_to_index = &_firstFree;
            tSigned32 index = *ptr_to_index;
            while(index != -1)
            {
                if(index >= newSize)
                {
                    delete _pool[index];
                    index = _nextFree[index];
                    *ptr_to_index = index;
                    _numberFree--;
                }
                else
                {
                    ptr_to_index = &_nextFree[index];
                    index = *ptr_to_index;
                }
            }

            _pool.resize(newSize);
            _nextFree.resize(newSize);
        }
    }

    T* get()
    {
        cMutexLock lock(_mutex);
        T* result;
        if(_firstFree == -1)
        {
            result = new T();
            result->setPoolIndex(-1);
        }
        else
        {
            result = _pool[_firstFree];
            _firstFree = _nextFree[_firstFree];
            _numberFree--;
        }
        return result;
    }
    void release(T* ptr)
    {
        cMutexLock lock(_mutex);
        tSigned32 index = ptr->getPoolIndex();
        if(index == -1)
        {
            delete ptr;
        }
        else
        {
            _nextFree[index] = _firstFree;
            _firstFree = index;
            _numberFree++;
        }
    }
};

#endif
