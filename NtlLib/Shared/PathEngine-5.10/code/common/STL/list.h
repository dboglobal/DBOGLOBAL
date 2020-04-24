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
#include <list>
#else
#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#include "common/Containers/ReplacementList.h"
#include "common/interface/Assert.h"

namespace std
{

template <class T>
class list
{
    typedef list<T> tThis;

    typedef nList_Internal::cNode<T> cNode;
    cNode m_head;
    tUnsigned32 m_size;

public:

    typedef nList_Internal::const_iterator<T> const_iterator;
    typedef nList_Internal::iterator<T> iterator;
    typedef tUnsigned32 size_type;

    list()
    {
        m_size=0;
        m_head.initAsHead();
//        checkvalid();
    }
    ~list()
    {
        while(!empty())
            pop_front();
    }

    list<T>(const list<T>& rhs)
    {
        m_size=0;
        m_head.initAsHead();
        insert(begin(), rhs.begin(), rhs.end());
    }
    list<T>& operator=(const list<T>& rhs)
    {
        if (this != &rhs)
        {
            iterator F1 = begin();
            iterator L1 = end();
            const_iterator F2 = rhs.begin();
            const_iterator L2 = rhs.end();
            for (; F1 != L1 && F2 != L2; ++F1, ++F2)
                *F1 = *F2;
            erase(F1, L1);
            insert(L1, F2, L2);
        }
        return (*this);
    }

    bool operator==(const tThis& rhs) const
    {
        if(size() != rhs.size())
        {
            return false;
        }
        const_iterator i, j;
        i = begin();
        j = rhs.begin();
        while(i != end())
        {
            if(*i != *j)
            {
                return false;
            }
            ++i;
            ++j;
        }
        return true;
    }
    bool operator!=(const tThis& rhs) const
    {
        return !(*this == rhs);
    }

    void clear()
    {
        while(!empty())
            pop_front();
    }

    iterator begin()
    {
        return iterator(m_head.getNext());
    }
    const_iterator begin() const
    {
        return const_iterator(m_head.getNext());
    }
    iterator end()
    {
        return iterator(&m_head);
    }
    const_iterator end() const
    {
        return const_iterator(&m_head);
    }

    static iterator invalidIterator()
    {
        return iterator(0);
    }

    T& front() {return *begin();}
    const T& front() const {return *begin();}
    T& back() {return m_head.getPrev()->m_object;}
    const T& back() const {return m_head.getPrev()->m_object;}

    void push_front(const T& aObject)
    {
        cNode *node=new cNode;
        node->m_object=aObject;
        node->insertAfter(&m_head);
        m_size++;
//        checkvalid();
    }
    void pop_front()
    {
        m_size--;
        cNode* toDelete = m_head.getNext();
        toDelete->remove();
        delete toDelete;
    }
    void push_back(const T& aObject)
    {
        cNode *node=new cNode;
        node->m_object=aObject;
        node->insertAfter(m_head.getPrev());
        m_size++;
//        checkvalid();
    }
    void pop_back()
    {
        assertD(m_size);
        cNode *todelete=m_head.getPrev();
        todelete->remove();
        delete todelete;
        m_size--;
//        checkvalid();
    }

    iterator insert(iterator i, const T& value)
    {
        --i;
        cNode *node=new cNode;
        node->m_object = value;
        node->insertAfter(i.getNode());
        ++i;
        m_size++;
//        checkvalid();
        return i;
    }
    void insert(iterator P, const_iterator F, const_iterator L)
    {
        for (; F != L; ++F)
            insert(P, *F);
    }

    iterator erase(iterator i)
    {
        iterator returnvalue=i;
        returnvalue++;
        i.getNode()->remove();
        delete i.getNode();
        m_size--;
//        checkvalid();
        return returnvalue;
    }
    iterator erase(iterator F, iterator L)
    {
        while (F != L)
            erase(F++);
        return (F);
    }

    void splice(iterator P, list<T>& X)
    {
        if (!X.empty())
        {
            Splice(P, X, X.begin(), X.end());
            m_size += X.m_size;
            X.m_size = 0;
        }
//        checkvalid();
//        X.checkvalid();
    }
    void splice(iterator P, list<T>& X, iterator F)
    {
        iterator L = F;
        if (P != F && P != ++L)
        {
            Splice(P, X, F, L);
            ++m_size;
            --X.m_size;
        }
//        checkvalid();
//        X.checkvalid();
    }
    void splice(iterator P, list<T>& X, iterator F, iterator L)
    {
        if (F != L)
        {
            if (&X != this)
            {
                tSigned32 nodes;
                nodes=0;
                iterator i=F;
                do
                {
                    i++;
                    nodes++;
                }
                while(i!=L);
                m_size += nodes;
                X.m_size -= nodes;
            }
            Splice(P, X, F, L); 
        }
//        checkvalid();
//        X.checkvalid();
    }
    void swap(list<T>& rhs)
    {
        {
            cNode temp=m_head;
            m_head=rhs.m_head;
            rhs.m_head=temp;
        }
        {
            tSigned32 temp=m_size;
            m_size=rhs.m_size;
            rhs.m_size=temp;
        }
        if(m_size)
        {
            m_head.getNext()->setPrev(&m_head);
            m_head.getPrev()->setNext(&m_head);
        }
        else
        {
            m_head.setNext(&m_head);
            m_head.setPrev(&m_head);
        }
        if(rhs.m_size)
        {
            rhs.m_head.getNext()->setPrev(&rhs.m_head);
            rhs.m_head.getPrev()->setNext(&rhs.m_head);
        }
        else
        {
            rhs.m_head.setPrev(&rhs.m_head);
            rhs.m_head.setNext(&rhs.m_head);
        }
//        checkvalid();
//        rhs.checkvalid();
    }
    
    tUnsigned32 size() const
    {
        return m_size;
    }
    bool empty() const
    {
        return m_size==0;
    }

//    void checkvalid()
//    {
//        tSigned32 size;
//        cNode *node=&m_head;
//        size=0;
//        while(node->getNext()!=&m_head)
//        {
//            size++;
//            assertD(node->getNext()->getPrev()==node);
//            node=node->getNext();
//        }
//        assertD(m_head.getPrev()==node);
//        assertD(size==m_size);
//    }
private:
    void Splice(iterator P, list<T>& X, iterator F, iterator L)
    {
        L.getNode()->getPrev()->setNext(P.getNode());
        F.getNode()->getPrev()->setNext(L.getNode());
        P.getNode()->getPrev()->setNext(F.getNode());
        cNode *S=P.getNode()->getPrev();
        P.getNode()->setPrev(L.getNode()->getPrev());
        L.getNode()->setPrev(F.getNode()->getPrev());
        F.getNode()->setPrev(S);
    }
};


}

#endif
#endif
