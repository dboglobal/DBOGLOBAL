//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPLACE_STL
#include <vector>
#else
#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED

#include "common/STL/utility.h"
#include "common/STL/new.h"

namespace std
{

template <class T>
class vector
{
    typedef vector<T> tThis;

    tUnsigned32 _size;
    tUnsigned32 _capacity;
    T* _data;

public:

    typedef T* iterator;
    typedef const T* const_iterator;

    vector()
    {
        _size = 0;
        _capacity = 0;
        _data = 0;
    }
    vector(tUnsigned32 size)
    {
        _size = size;
        _capacity = size;
        _data = (T*) operator new (_capacity * sizeof(T));
        tUnsigned32 i;
        for(i = 0; i < _size; i++)
        {
	        new ((void*)(_data + i)) T();
        }
    }
    vector(tUnsigned32 size, const T& fillWith)
    {
        _size = size;
        _capacity = size;
        _data = (T*) operator new (_capacity * sizeof(T));
        tUnsigned32 i;
        for(i = 0; i < _size; i++)
        {
	        new ((void*)(_data + i)) T(fillWith);
        }
    }
    vector(const tThis& rhs)
    {
        _size = rhs._size;
        _capacity = rhs._capacity;
        _data = (T*) operator new (_capacity * sizeof(T));
        tUnsigned32 i;
        for(i = 0; i < _size; i++)
        {
	        new ((void*)(_data + i)) T(rhs[i]);
        }
    }

    ~vector()
    {
        tUnsigned32 i;
        for(i = 0; i < _size; i++)
        {
            _data[i].~T();
        }
        operator delete(_data);
    }

    bool operator==(const tThis& rhs)
    {
        if(rhs.size() != size())
        {
            return false;
        }
        tUnsigned32 i;
        for(i = 0; i < size(); i++)
        {
            if((*this)[i] != rhs[i])
            {
                return false;
            }
        }
        return true;
    }

	tThis& operator=(const tThis& rhs)
	{
        if(&rhs == this)
        {
            return *this;
        }

        if(rhs.size() <= _size)
        {
            tUnsigned32 i;
            for(i = 0; i < rhs.size(); i++)
            {
                _data[i] = rhs[i];
            }
            for(; i < _size; i++)
            {
                _data[i].~T();
            }
            _size = rhs.size();
        }
        else
        if(rhs.size() <= _capacity)
        {
            tUnsigned32 i;
            for(i = 0; i < _size; i++)
            {
                _data[i] = rhs[i];
            }
            for(; i < _capacity; i++)
            {
    	        new ((void*)(_data + i)) T(rhs[i]);
            }
            _size = rhs.size();
        }
        else
        {
            tUnsigned32 i;
            for(i = 0; i < _size; i++)
            {
                _data[i].~T();
            }
            operator delete(_data);
            _size = rhs.size();
            _capacity = rhs.size();
            _data = (T*) operator new (_capacity * sizeof(T));
            for(i = 0; i < _size; i++)
            {
	            new ((void*)(_data + i)) T(rhs[i]);
            }
        }

		return *this;
    }

    bool empty() const {return _size == 0;}
    void clear() {resize(0);}

    void swap(vector<T>& rhs)
    {
        std::swap(_data, rhs._data);
        std::swap(_size, rhs._size);
        std::swap(_capacity, rhs._capacity);
    }

    T* begin() const
    {
        return _data;
    }
    T* end() const
    {
        return _data + _size;
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

    T& operator[](tUnsigned32 index)
    {
        return _data[index];
    }
    const T& operator[](tUnsigned32 index) const
    {
        return _data[index];
    }

    tUnsigned32 size() const
    {
        return _size;
    }

    void push_back(const T& value)
    {
        resize(_size + 1, value);
    }
    void pop_back()
    {
        _size--;
        _data[_size].~T();
    }

    void resize(tUnsigned32 newSize)
    {
        if(newSize <= _size)
        {
            tUnsigned32 i;
            for(i = newSize; i < _size; i++)
            {
                _data[i].~T();
            }
            _size = newSize;
            return;
        }
        if(newSize <= _capacity)
        {
            tUnsigned32 i;
            for(i = _size; i < newSize; i++)
            {
    	        new ((void*)(_data + i)) T();
            }
            _size = newSize;
            return;
        }
        tUnsigned32 newCapacity = newSize;
        if(newCapacity < _size * 2)
        {
            newCapacity = _size * 2;
        }
        T* newData = (T*) operator new (newCapacity * sizeof(T));
        tUnsigned32 i;
        for(i = 0; i < _size; i++)
        {
	        new ((void*)(newData + i)) T(_data[i]);
            _data[i].~T();
        }
        for(; i < newSize; i++)
        {
	        new ((void*)(newData + i)) T();
        }
        operator delete(_data);
        _data = newData;
        _size = newSize;
        _capacity = newCapacity;
        return;
    }

    void resize(tUnsigned32 newSize, const T& fillWith)
    {
        if(newSize <= _size)
        {
            tUnsigned32 i;
            for(i = newSize; i < _size; i++)
            {
                _data[i].~T();
            }
            _size = newSize;
            return;
        }
        if(newSize <= _capacity)
        {
            tUnsigned32 i;
            for(i = _size; i < newSize; i++)
            {
    	        new ((void*)(_data + i)) T(fillWith);
            }
            _size = newSize;
            return;
        }
        tUnsigned32 newCapacity = newSize;
        if(newCapacity < _size * 2)
        {
            newCapacity = _size * 2;
        }
        T* newData = (T*) operator new (newCapacity * sizeof(T));
        tUnsigned32 i;
        for(i = 0; i < _size; i++)
        {
	        new ((void*)(newData + i)) T(_data[i]);
            _data[i].~T();
        }
        for(; i < newSize; i++)
        {
	        new ((void*)(newData + i)) T(fillWith);
        }
        operator delete(_data);
        _data = newData;
        _size = newSize;
        _capacity = newCapacity;
        return;
    }
};

}

#endif
#endif
