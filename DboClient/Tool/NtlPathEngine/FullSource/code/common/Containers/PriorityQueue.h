//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PRIORITY_QUEUE_INCLUDED
#define PRIORITY_QUEUE_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"
#include "common/STL/algorithm.h"

template <class T>
class cPriorityQueue
{
    std::vector<T> _heap;

    class cPredicate
    {
    public:
    	bool operator()(const T lhs, const T rhs) const
		{
	    	return *rhs < *lhs;
		}
    };

public:

    cPriorityQueue(tSigned32 reservedSize) :
      _heap(reservedSize)
    {
        _heap.resize(0);
    }

    void clear()
    {
        _heap.resize(0);
    }
    void swap(cPriorityQueue& rhs)
    {
        _heap.swap(rhs._heap);
    }

    bool empty() const {return _heap.empty();}
    tSigned32 size() const {return static_cast<tSigned32>(_heap.size());}

    const T& front() const
    {
        return _heap.front();
    }

    const T& operator[](tSigned32 index) const
    {
        return _heap[index];
    }

    void insert(T value)
    {
        _heap.push_back(value);
        std::push_heap(_heap.begin(), _heap.end(), cPredicate());
    }
    T popFront()
    {
        assertD(!empty());
        T result = _heap.front();
        std::pop_heap(_heap.begin(), _heap.end(), cPredicate());
        _heap.pop_back();
        return result;
    }
    void updateValueForwards(T value)
    {
        typename std::vector<T>::iterator it;
        for(it = _heap.begin(); it != _heap.end(); ++it)
        {
            if((*it) == value)
            {
                std::push_heap(_heap.begin(), it + 1, cPredicate());
                return;
            }
        }
        invalid();
    }
};

#endif
