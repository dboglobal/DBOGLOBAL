//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef HANDLE_LIST_INCLUDED
#define HANDLE_LIST_INCLUDED

#include "common/Handle.h"
#include "common/HandleTarget.h"
#include "common/STL/list.h"

template <class T>
class cHandleList : public bHandleTarget
{
public:

    typedef typename std::list<cHandle<T> >::iterator iterator;
    typedef typename std::list<cHandle<T> >::const_iterator const_iterator;
    typedef typename std::list<cHandle<T> >::size_type size_type;

    std::list<cHandle<T> > _list;

    cHandleList()
    {
    }
    cHandleList(std::list<cHandle<T> >& list)
    {
        _list.swap(list);
    }
    cHandleList(const cHandleList<T>& rhs) :
        _list(rhs._list)
    {
    }

    void initFromValueSequence(std::list<T>& sequence)
    {
        while(!sequence.empty())
        {
            _list.push_back(new T);
            _list.back()->swap(sequence.front());
            sequence.pop_front();
        }
    }

    bool operator==(const cHandleList<T>& rhs) const
    {
        return _list == rhs._list;
    }
    bool operator!=(const cHandleList<T>& rhs) const
    {
        return !((*this) == rhs);
    }

    size_type size() const {return _list.size();}
    void clear() {_list.clear();}
    iterator begin() {return _list.begin();}
    const_iterator begin() const {return _list.begin();}
    iterator end() {return _list.end();}
    const_iterator end() const {return _list.end();}
    void push_front(cHandle<T> value) {_list.push_front(value);}
    void push_back(cHandle<T> value) {_list.push_back(value);}
    void pop_front() {_list.pop_front();}
    void pop_back() {_list.pop_back();}
    cHandle<T> front() {return _list.front();}
    const cHandle<T> front() const {return _list.front();}
    cHandle<T> back() {return _list.back();}
    const cHandle<T> back() const {return _list.back();}
    bool empty() const {return _list.empty();}
};

#endif
