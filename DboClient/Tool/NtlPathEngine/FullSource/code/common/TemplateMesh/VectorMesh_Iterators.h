//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_MESH_ITERATORS_INCLUDED
#define VECTOR_MESH_ITERATORS_INCLUDED

#include "common/interface/Assert.h"
#include "common/STL/vector.h"

template <class F,class E,class V> class cVectorMesh;

namespace nVectorMesh_Internal
{
    template <class F,class E,class V>
    class cFace
    {
    public:
        F _userData;
        tSigned32 firstEdge;
        cFace() {}
        cFace(const F &_userData) :
            _userData(_userData)
        {
            // firstEdge must be initialised after construction
        }
        void markAsDead(tSigned32 nextDead)
        {
            assertD(nextDead >= -1);
            //.. decreases size limit slightly
            firstEdge = -2 - nextDead;
        }
        bool isDead() const
        {
            return firstEdge < 0;
        }
        tSigned32 nextDead() const
        {
            return -2 - firstEdge;
        }
        template <class F2,class E2,class V2> void
        initFrom(const cFace<F2,E2,V2>& rhs)
        {
            firstEdge = rhs.firstEdge;
            if(!rhs.isDead())
            {
                _userData = rhs._userData;
            }
        }
        void
        remap(const tSigned32* edgeRemaps)
        {
            assertD(!isDead());
            firstEdge = edgeRemaps[firstEdge];
        }
        void
        offset(tSigned32 edgeOffset)
        {
            assertD(!isDead());
            firstEdge += edgeOffset;
        }
    };
    template <class F,class E,class V>
    class cEdge
    {
    public:
        E _userData;
        tSigned32 prev;
        tSigned32 next;
        tSigned32 twin;
        tSigned32 face;
        tSigned32 vert;
        cEdge() {}
        cEdge(const E& _userData) :
            _userData(_userData)
        {
        }
        void markAsDead(tSigned32 nextDead)
        {
            assertD(nextDead >= -1);
            //.. decreases size limit slightly
            prev = -2 - nextDead;
        }
        bool isDead() const
        {
            return prev < 0;
        }
        tSigned32 nextDead() const
        {
            return -2 - prev;
        }
        template <class F2,class E2,class V2> void
        initFrom(const cEdge<F2,E2,V2>& rhs)
        {
            prev = rhs.prev;
            if(!rhs.isDead())
            {
                next = rhs.next;
                twin = rhs.twin;
                face = rhs.face;
                vert = rhs.vert;
                _userData = rhs._userData;
            }
        }
        void
        remap(  const tSigned32* faceRemaps,
                const tSigned32* edgeRemaps,
                const tSigned32* vertRemaps
                )
        {
            assertD(!isDead());
            prev = edgeRemaps[prev];
            next = edgeRemaps[next];
            if(twin != -1)
            {
                twin = edgeRemaps[twin];
            }
            face = faceRemaps[face];
            vert = vertRemaps[vert];
        }
        void
        remapFace(const tSigned32* faceRemaps)
        {
            assertD(!isDead());
            face = faceRemaps[face];
        }
        void
        remapVert(const tSigned32* vertRemaps)
        {
            assertD(!isDead());
            vert = vertRemaps[vert];
        }
        void
        offset( tSigned32 faceOffset,
                tSigned32 edgeOffset,
                tSigned32 vertOffset
                )
        {
            assertD(!isDead());
            prev += edgeOffset;
            next += edgeOffset;
            if(twin != -1)
            {
                twin += edgeOffset;
            }
            face += faceOffset;
            vert += vertOffset;
        }
    };
    template <class F,class E,class V>
    class cVert
    {
    public:
        V _userData;
        tSigned32 firstEdge;
        tSigned32 lastEdge;
        cVert() {}
        cVert(const V& _userData) :
            _userData(_userData)
        {
            // firstEdge and lastEdge must be initialised after construction
        }
        void markAsDead(tSigned32 nextDead)
        {
            assertD(nextDead >= -1);
            //.. decreases size limit slightly
            firstEdge = -3 - nextDead;
        }
        bool isDead() const
        {
            return firstEdge < -1;
        }
        tSigned32 nextDead() const
        {
            return -3 - firstEdge;
        }
        template <class F2,class E2,class V2> void
        initFrom(const cVert<F2,E2,V2>& rhs)
        {
            firstEdge = rhs.firstEdge;
            if(!rhs.isDead())
            {
                lastEdge = rhs.lastEdge;
                _userData = rhs._userData;
            }
        }
        void
        remap(const tSigned32* edgeRemaps)
        {
            assertD(!isDead());
            if(firstEdge != -1) // support meshes with 'uninitialised verts', for fast construction
            {
                firstEdge = edgeRemaps[firstEdge];
            }
            if(lastEdge != -1)
            {
                lastEdge = edgeRemaps[lastEdge];
            }
        }
        void
        offset(tSigned32 edgeOffset)
        {
            assertD(!isDead());
            if(firstEdge != -1) // support meshes with 'uninitialised verts', for fast construction
            {
                firstEdge += edgeOffset;
            }
            if(lastEdge != -1) // support meshes with 'uninitialised verts', for fast construction
            {
                lastEdge += edgeOffset;
            }
        }
    };

    template <class F,class E,class V>
    class cIsDeadPredicate
    {
    public:
        bool operator()(const cFace<F,E,V>& f)
        {
            return f.isDead();
        }
        bool operator()(const cVert<F,E,V>& v)
        {
            return v.isDead();
        }
    };

    template <class F,class E,class V>
    class cBuffers
    {
    public:
        std::vector<cFace<F,E,V> > _faces;
        std::vector<cEdge<F,E,V> > _edges;
        std::vector<cVert<F,E,V> > _verts;
    };

    template <class F,class E,class V> class edge;
    template <class F,class E,class V>
    class face
    {
        friend class cVectorMesh<F,E,V>;
        friend class edge<F,E,V>;
        cBuffers<F,E,V>* _buffers;
        tSigned32 _i;

        std::vector<cFace<F,E,V> >& buffer()
        {
            return _buffers->_faces;
        }
        const std::vector<cFace<F,E,V> >& buffer() const
        {
            return _buffers->_faces;
        }
        cFace<F,E,V>& entry()
        {
            return buffer()[_i];
        }
        const cFace<F,E,V>& entry() const
        {
            return buffer()[_i];
        }

        void incrementPastDeadEntriesAndInvalidateIfAtEnd()
        {
            while(_i < SizeL(buffer()) && buffer()[_i].isDead())
            {
                ++_i;
            }
            if(_i == SizeL(buffer()))
            {
                 _i = -1;
            }
        }

    public:

        bool entryIsDead() const
        {
            return entry().isDead();
        }
        tSigned32 index() const
        {
            return _i;
        }
        bool valid() const
        {
            return _i != -1;
        }
        bool operator==(const face<F,E,V>& rhs) const
        {
            assertD(_i == -1 || rhs._i == -1 || _buffers == rhs._buffers);
            return rhs._i == _i;
        }
        bool operator!=(const face<F,E,V>& rhs) const
        {
            assertD(_i == -1 || rhs._i == -1 || _buffers == rhs._buffers);
            return rhs._i != _i;
        }
        face<F,E,V>& operator++()            // prefix
        {
            ++_i;
            incrementPastDeadEntriesAndInvalidateIfAtEnd();
            return *this;
        }
        face<F,E,V> operator++(int)        // postfix
        {
            face<F,E,V> temporary = *this;
            ++*this;
            return temporary;
        }
        F& operator*() const
        {
            return const_cast<F&>(buffer()[_i]._userData);
        }
        F* operator->() const
        {
            return const_cast<F*>(&(buffer()[_i]._userData));
        }
        void clear()
        {
            _i = -1;
        }
        void
        remap(const std::vector<tSigned32>& remaps)
        {
            assertD(_i >= 0 && _i < SizeL(remaps));
            _i = remaps[_i];
            assertD(_i != -1);
        }
    };

    template <class F,class E,class V>
    class vertex
    {
        friend class cVectorMesh<F,E,V>;
        friend class edge<F,E,V>;
        cBuffers<F,E,V>* _buffers;
        tSigned32 _i;

        std::vector<cVert<F,E,V> >& buffer()
        {
            return _buffers->_verts;
        }
        const std::vector<cVert<F,E,V> >& buffer() const
        {
            return _buffers->_verts;
        }
        cVert<F,E,V>& entry()
        {
            return buffer()[_i];
        }
        const cVert<F,E,V>& entry() const
        {
            return buffer()[_i];
        }

        void incrementPastDeadEntriesAndInvalidateIfAtEnd()
        {
            while(_i < SizeL(buffer()) && buffer()[_i].isDead())
            {
                ++_i;
            }
            if(_i == SizeL(buffer()))
            {
                 _i = -1;
            }
        }

    public:

        bool entryIsDead() const
        {
            return entry().isDead();
        }
        tSigned32 index() const
        {
            return _i;
        }
        bool valid() const
        {
            return _i != -1;
        }
        bool operator==(const vertex<F,E,V>& rhs) const
        {
            assertD(_i == -1 || rhs._i == -1 || _buffers == rhs._buffers);
            return rhs._i == _i;
        }
        bool operator!=(const vertex& rhs) const
        {
            assertD(_i == -1 || rhs._i == -1 || _buffers == rhs._buffers);
            return rhs._i != _i;
        }
        vertex<F,E,V>& operator++()            // prefix
        {
            ++_i;
            incrementPastDeadEntriesAndInvalidateIfAtEnd();
            return *this;
        }
        vertex<F,E,V> operator++(int)        // postfix
        {
            vertex<F,E,V> temporary = *this;
            ++*this;
            return temporary;
        }
        V& operator*() const
        {
            return const_cast<V&>(buffer()[_i]._userData);
        }
        V* operator->() const
        {
            return const_cast<V*>(&(buffer()[_i]._userData));
        }
        void clear()
        {
            _i = -1;
        }
        bool isInternalVertex() const
        {
            return buffer()[_i].lastEdge == -1 && buffer()[_i].firstEdge != -1;
        }
        void
        remap(const std::vector<tSigned32>& remaps)
        {
            assertD(_i >= 0 && _i < SizeL(remaps));
            _i = remaps[_i];
            assertD(_i != -1);
        }
    };

    template <class F,class E,class V>
    class edge
    {
        friend class cVectorMesh<F,E,V>;
        cBuffers<F,E,V>* _buffers;
        tSigned32 _i;

        std::vector<cEdge<F,E,V> >& buffer()
        {
            return _buffers->_edges;
        }
        const std::vector<cEdge<F,E,V> >& buffer() const
        {
            return _buffers->_edges;
        }
        cEdge<F,E,V>& entry()
        {
            return buffer()[_i];
        }
        const cEdge<F,E,V>& entry() const
        {
            return buffer()[_i];
        }

    public:

        bool entryIsDead() const
        {
            return entry().isDead();
        }
        tSigned32 index() const
        {
            return _i;
        }
        void clear()
        {
            _i = -1;
        }
        bool valid() const
        {
            return _i != -1;
        }
        bool operator==(const edge<F,E,V>& rhs) const
        {
            assertD(_i == -1 || rhs._i == -1 || _buffers == rhs._buffers);
            return rhs._i == _i;
        }
        bool operator!=(const edge& rhs) const
        {
            assertD(_i == -1 || rhs._i == -1 || _buffers == rhs._buffers);
            return rhs._i != _i;
        }
        edge<F,E,V> twin() const
        {
            edge<F,E,V> result;
            result._buffers = _buffers;
            result._i = buffer()[_i].twin;
            return result;
        }
        edge<F,E,V> next() const
        {
            edge<F,E,V> result;
            result._buffers = _buffers;
            result._i = buffer()[_i].next;
            return result;
        }
        edge<F,E,V> prev() const
        {
            edge<F,E,V> result;
            result._buffers = _buffers;
            result._i = buffer()[_i].prev;
            return result;
        }
        edge<F,E,V> operator++(int)        // postfix
        {
            edge<F,E,V> temporary = *this;
            ++*this;
            return temporary;
        }
        edge<F,E,V>& operator++()            // prefix
        {
            _i = buffer()[_i].next;
            return *this;
        }
        edge<F,E,V> operator--(int)        // postfix
        {
            edge<F,E,V> temporary = *this;
            --*this;
            return temporary;
        }
        edge<F,E,V>& operator--()            // prefix
        {
            _i = buffer()[_i].prev;
            return *this;
        }

        E& operator*() const
        {
            return const_cast<E&>(buffer()[_i]._userData);
        }
        E* operator->() const
        {
            return const_cast<E*>(&(buffer()[_i]._userData));
        }
        face<F,E,V> face() const
        {
            nVectorMesh_Internal::face<F,E,V> result;
            result._buffers = _buffers;
            result._i = buffer()[_i].face;
            return result;
        }
        vertex<F,E,V> vertex() const
        {
            nVectorMesh_Internal::vertex<F,E,V> result;
            result._buffers = _buffers;
            result._i = buffer()[_i].vert;
            return result;
        }
        edge<F,E,V> nextExternal() const
        {
            assertD(buffer()[_i].twin == -1);
            assertD(buffer()[_i].next == vertex().entry().lastEdge);
            edge<F,E,V> result;
            result._buffers = _buffers;
            result._i = vertex().entry().firstEdge;
            return result;
        }
        edge<F,E,V> prevExternal() const
        {
            assertD(entry().twin == -1);
            assertD(_i == vertex().entry().firstEdge);
            assertD(vertex().entry().lastEdge != -1);
            edge<F,E,V> result;
            result._buffers = _buffers;
            result._i = buffer()[vertex().entry().lastEdge].prev;
            return result;
        }

        bool toNextInMesh()
        {
            nVectorMesh_Internal::face<F,E,V> f = face();  // Compiler bug? VC 7.1.3088 complains without the namespace
            ++(*this);
            if(_i == f.entry().firstEdge)
            {
                ++f;
                if(f.valid())
                {
                    _i = f.entry().firstEdge;
                    return true;
                }
                _i = -1;
                return false;
            }
            return true;
        }
        bool toNextInFace()
        {
            nVectorMesh_Internal::face<F,E,V> f = face();  // Compiler bug? VC 7.1.3088 complains without the namespace
            ++(*this);
            if(_i == f.entry().firstEdge)
            {
                _i = -1;
                return false;
            }
            return true;
        }
        bool toNextAroundVertex()
        {
            --(*this);
            _i = entry().twin;
            if(_i != -1 && _i == vertex().entry().firstEdge)
            {
                assertD(vertex().entry().lastEdge == -1);
                _i = -1;
            }
            return _i != -1;
        }
        void
        remap(const std::vector<tSigned32>& remaps)
        {
            assertD(_i >= 0 && _i < SizeL(remaps));
            _i = remaps[_i];
            assertD(_i != -1);
        }
    };
}

#endif

