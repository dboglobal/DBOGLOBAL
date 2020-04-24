//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPLACEMENT_LIST_INCLUDED
#define REPLACEMENT_LIST_INCLUDED

#include "common/interface/Assert.h"

namespace nList_Internal
{
    template <class T>
    class cNode
    {
    public:
        cNode<T> *_next;
        cNode<T> *_prev;
        T m_object;

        void insertAfter(cNode<T>* insertAfter)
        {
            _next = insertAfter->_next;
            _prev = insertAfter;
            _prev->_next = this;
            _next->_prev = this;
        }
        void initAsHead()
        {
            _next = this;
            _prev = this;
        }
        void remove()
        {
            _next->_prev = _prev;
            _prev->_next = _next;
        }
        cNode<T>* getNext()
        {
            return _next;
        }
        cNode<T>* getPrev()
        {
            return _prev;
        }
        const cNode<T>* getNext() const
        {
            return _next;
        }
        const cNode<T>* getPrev() const
        {
            return _prev;
        }
        void setNext(cNode<T>* value)
        {
            _next = value;
        }
        void setPrev(cNode<T>* value)
        {
            _prev = value;
        }
    };



    template <class T> class iterator;
    template <class T>
    class const_iterator
    {
    protected:
        typedef cNode<T> tNode;
        cNode<T>* _node;
    public:
        const_iterator() {}
        const_iterator(cNode<T> *node)
            :_node(node) {}
        const_iterator(const cNode<T> *node)
        // I promise not to change it!
            :_node(const_cast<cNode<T>*>(node)) {}
        const T& operator*() const
        {
            return _node->m_object;
        }
        const T* const operator->() const
        {
            return &**this;
        }
        const_iterator operator++(int)        // postfix
        {
            const_iterator temporary=*this;
            ++*this;
            return temporary;
        }
        const_iterator& operator++()            // prefix
        {
            _node=_node->getNext();
            return *this;
        }
        const_iterator operator--(int)        // postfix
        {
            const_iterator temporary=*this;
            --*this;
            return temporary;
        }
        const_iterator& operator--()            // prefix
        {
            _node=_node->getPrev();
            return *this;
        }
        bool operator==(const const_iterator &rhs) const
        {
            return _node==rhs._node;
        }
        bool operator!=(const const_iterator &rhs) const
        {
            return _node!=rhs._node;
        }

        void setInvalid()
        {
            _node = 0;
        }
        bool valid()
        {
            return _node != 0;
        }

        //... lose this
        bool isInvalid()
        {
            return _node==0;
        }
    };

    template <class T>
    class iterator : public const_iterator<T>
    {
    public:
        iterator()
        {}
        iterator(cNode<T> *node)
            :const_iterator<T>(node)
        {}
        T& operator*()
        {
            return this->_node->m_object;
        }
        T* operator->()
        {
            return &**this;
        }
        iterator operator++(int)        // postfix
        {
            iterator temporary = *this;
            ++*this;
            return temporary;
        }
        iterator& operator++()            // prefix
        {
            this->_node = this->_node->getNext();
            return *this;
        }
        iterator operator--(int)        // postfix
        {
            iterator temporary=*this;
            --*this;
            return temporary;
        }
        iterator& operator--()            // prefix
        {
            this->_node = this->_node->getPrev();
            return *this;
        }
        cNode<T> *getNode()
        {
            return this->_node;
        }
    };

    template <class T>
    class erasable_iterator : public iterator<T>
    {
    public:
        erasable_iterator()
        {}
        erasable_iterator(cNode<T> *node)
            :iterator<T>(node)
        {}
        void erase()
        {
            this->_node->_next->_prev = this->_node->_prev;
            this->_node->_prev->_next = this->_node->_next;
            delete this->_node;
        }
        void eraseAndSetInvalid()
        {
            this->_node->_next->_prev = this->_node->_prev;
            this->_node->_prev->_next = this->_node->_next;
            cNode<T>* to_delete = this->_node;
            this->_node = 0;
            delete to_delete;
        }
    };

//    template <class T>
//    class cInitInvalidIterator
//    {
//        cNode<T> *_node;
//    public:
//        cInitInvalidIterator()
//        {
//            _node=0;
//        }
//        operator const_iterator<T>()
//        {
//            return const_iterator(_node)
//        }
//    };
}

template <class T>
class cList_NoSize
{
    typedef nList_Internal::cNode<T> cNode;
    cNode _head;
public:
    typedef nList_Internal::const_iterator<T> const_iterator;
    typedef nList_Internal::erasable_iterator<T> iterator;

    cList_NoSize()
    {
        _head.initAsHead();
    }
    ~cList_NoSize()
    {
        while(!empty())
            pop_front();
    }

    cList_NoSize<T>(const cList_NoSize<T>& rhs)
    {
        _head.initAsHead();
        insert(begin(), rhs.begin(), rhs.end());
    }
    cList_NoSize<T>& operator=(const cList_NoSize<T>& rhs)
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

    void clear()
    {
        while(!empty())
            pop_front();
    }

    iterator begin()
    {
        return iterator(_head.getNext());
    }
    const_iterator begin() const
    {
        return const_iterator(_head.getNext());
    }
    iterator end()
    {
        return iterator(&_head);
    }
    const_iterator end() const
    {
        return const_iterator(&_head);
    }

    static iterator invalidIterator()
    {
        return iterator(0);
    }

    T& front() {return *begin();}
    T& back() {return _head.getPrev()->m_object;}

    void push_front(const T& aObject)
    {
        cNode *node=new cNode;
        node->m_object=aObject;
        node->insertAfter(&_head);
    }
    void pop_front()
    {
        cNode *todelete=_head.getNext();
        todelete->remove();
        delete todelete;
    }
    void push_back(const T& aObject)
    {
        cNode *node=new cNode;
        node->m_object=aObject;
        node->insertAfter(_head.getPrev());
    }
    void pop_back()
    {
        cNode *todelete=_head.getPrev();
        todelete->remove();
        delete todelete;
    }

    iterator insert(iterator i, T* p=0)
    {
        --i;
        cNode *node=new cNode;
        node->m_object=p;
        node->insertAfter(i.getNode());
        ++i;
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
        return returnvalue;
    }
    iterator erase(iterator F, iterator L)
    {
        while (F != L)
            erase(F++);
        return (F);
    }

    void splice(iterator P, cList_NoSize<T>& X)
    {
        if (!X.empty())
        {
            Splice(P, X, X.begin(), X.end());
        }
    }
    void splice(iterator P, cList_NoSize<T>& X, iterator F)
    {
        iterator L = F;
        if (P != F && P != ++L)
        {
            Splice(P, X, F, L);
        }
    }
    void splice(iterator P, cList_NoSize<T>& X, iterator F, iterator L)
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
            }
            Splice(P, X, F, L); 
        }
    }
    void swap(cList_NoSize<T>& rhs)
    {
        bool this_was_empty=empty();
        bool rhs_was_empty=rhs.empty();
        {
            cNode temp=_head;
            _head=rhs._head;
            rhs._head=temp;
        }
        if(!this_was_empty)
        {
            _head.getNext()->setPrev(&_head);
            _head.getPrev()->setNext(&_head);
        }
        else
        {
            _head.setNext(&_head);
            _head.setPrev(&_head);
        }
        if(!rhs_was_empty)
        {
            rhs._head.getNext()->setPrev(&rhs._head);
            rhs._head.getPrev()->setNext(&rhs._head);
        }
        else
        {
            rhs._head.setPrev(&rhs._head);
            rhs._head.setNext(&rhs._head);
        }
    }
    
    bool empty() const
    {
        return _head._next==&_head;
    }

private:
    void Splice(iterator P, cList_NoSize<T>& X, iterator F, iterator L)
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


#endif

