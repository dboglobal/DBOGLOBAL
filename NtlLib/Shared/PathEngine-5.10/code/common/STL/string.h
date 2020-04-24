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
#include <string>
#else
#ifndef STRING_INCLUDED
#define STRING_INCLUDED

#include "common/STL/utility.h"
#include "common/CLib/string.h"

namespace std
{

class string
{
    typedef string tThis;
    typedef char T;

    tUnsigned32 _size;
    tUnsigned32 _capacity;
    char* _data;

    // reallocates if necessary, copying existing data
    // doesnt set the new null terminating character
    // if newSize > _size then the extra characters will be uninitialised
    void resize_DontNullTerminate(tUnsigned32 newSize)
    {
        if(newSize <= _capacity)
        {
            _size = newSize;
            return;
        }
        tUnsigned32 newCapacity = newSize;
        if(newCapacity < _size * 2)
        {
            newCapacity = _size * 2;
        }
        char* newData = new char[newCapacity + 1];
        strcpy(newData, _data);
        delete [] _data;
        _data = newData;
        _size = newSize;
        _capacity = newCapacity;
    }

public:

    string()
    {
        _size = 0;
        _capacity = 0;
        _data = new char[_capacity + 1];
        _data[_size] = 0;
    }
    string(const char* ptr)
    {
        _size = strlen(ptr);
        _capacity = _size;
        _data = new T[_capacity + 1];
        strcpy(_data, ptr);
    }
    string(const tThis& rhs)
    {
        _size = rhs._size;
        _capacity = rhs._capacity;
        _data = new T[_capacity + 1];
        strcpy(_data, rhs._data);
    }

    ~string()
    {
        delete [] _data;
    }

	tThis& operator=(const tThis& rhs)
	{
        if(&rhs == this)
        {
            return *this;
        }
        if(rhs.size() <= _capacity)
        {
            strcpy(_data, rhs._data);
            _size = rhs.size();
        }
        else
        {
            delete [] _data;
            _size = rhs.size();
            _capacity = rhs.size();
            _data = new T[_capacity + 1];
            strcpy(_data, rhs._data);
        }
		return *this;
    }

    void operator+=(const tThis& rhs)
    {
        if(&rhs == this)
        {
            tThis temp = rhs;
            (*this) += temp;
            return;
        }
        tUnsigned32 oldSize = _size;
        resize_DontNullTerminate(_size + rhs._size);
        strcpy(_data + oldSize, rhs._data);
    }

    tThis operator+(const tThis& rhs)
    {
        tThis result(*this);
        result += rhs;
        return result;
    }

    void append(const tThis& rhs)
    {
        operator+=(rhs);
    }

    void append(const char* ptr)
    {
        tUnsigned32 oldSize = _size;
        resize_DontNullTerminate(_size + strlen(ptr));
        strcpy(_data + oldSize, ptr);
    }
        
    bool empty() const {return _size == 0;}
    void clear()
    {
        _size = 0;
        _data[0] = 0;
    }

    void swap(string& rhs)
    {
        std::swap(_data, rhs._data);
        std::swap(_size, rhs._size);
        std::swap(_capacity, rhs._capacity);
    }

    const char* c_str() const
    {
        return _data;
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

    T& operator[](tSigned32 index)
    {
        return _data[index];
    }
    const T& operator[](tSigned32 index) const
    {
        return _data[index];
    }

    tUnsigned32 size() const
    {
        return _size;
    }

    void resize(tUnsigned32 newSize)
    {
        resize_DontNullTerminate(newSize);
        _data[_size] = 0;
    }

//    void push_back(const char value)
//    {
//        resize(_size + 1);
//        back() = value;
//    }
//    void pop_back()
//    {
//        _size--;
//        _data[_size] = 0;
//    }
};

inline bool
operator==(const string& lhs, const string& rhs)
{
    if(lhs.size() != rhs.size())
    {
        return false;
    }
    tUnsigned32 i;
    for(i = 0; i < lhs.size(); i++)
    {
        if(lhs[i] != rhs[i])
        {
            return false;
        }
    }
    return true;
}

inline bool
operator!=(const string& lhs, const string& rhs)
{
    return !(lhs == rhs);
}

inline bool
operator<(const string& lhs, const string& rhs)
{
    tUnsigned32 i = 0;
    while(i < lhs.size() && i < rhs.size())
    {
        if(lhs[i] < rhs[i])
        {
            return true;
        }
        if(lhs[i] > rhs[i])
        {
            return false;
        }
        ++i;
    }
    return lhs.size() < rhs.size();
}

inline bool
operator<=(const string& lhs, const string& rhs)
{
    return !(rhs < lhs);
}
inline bool
operator>(const string& lhs, const string& rhs)
{
    return rhs < lhs;
}
inline bool
operator>=(const string& lhs, const string& rhs)
{
    return !(lhs < rhs);
}

}

#endif
#endif
