//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VECTOR_MESH_INCLUDED
#define VECTOR_MESH_INCLUDED

#include "common/TemplateMesh/VectorMesh_Iterators.h"
#include "common/Containers/Array.h"
#include "common/STL_Helpers.h"
#include "common/STL/utility.h"
#include "common/STL/algorithm.h"

template <class F,class E,class V>
class cVectorMesh
{
public:

    typedef F tFaceData;
    typedef E tEdgeData;
    typedef V tVertexData;

    typedef nVectorMesh_Internal::face<F,E,V> face;
    typedef nVectorMesh_Internal::edge<F,E,V> edge;
    typedef nVectorMesh_Internal::vertex<F,E,V> vertex;

private:
    
    typedef cVectorMesh<F,E,V> tThis;

    typedef nVectorMesh_Internal::cEdge<F,E,V> cEdge;
    typedef nVectorMesh_Internal::cFace<F,E,V> cFace;
    typedef nVectorMesh_Internal::cVert<F,E,V> cVert;

    tSigned32 _faces;
    tSigned32 _edges;
    tSigned32 _verts;
    nVectorMesh_Internal::cBuffers<F,E,V>* _buffers;
    tSigned32 _firstDeadFace;
    tSigned32 _firstDeadEdge;
    tSigned32 _firstDeadVert;

    std::vector<cFace>& faceBuffer()
    {
        return _buffers->_faces;
    }
    const std::vector<cFace>& faceBuffer() const
    {
        return _buffers->_faces;
    }
    std::vector<cEdge>& edgeBuffer()
    {
        return _buffers->_edges;
    }
    const std::vector<cEdge>& edgeBuffer() const
    {
        return _buffers->_edges;
    }
    std::vector<cVert>& vertBuffer()
    {
        return _buffers->_verts;
    }
    const std::vector<cVert>& vertBuffer() const
    {
        return _buffers->_verts;
    }

    cFace& faceEntry(face f)
    {
        return faceBuffer()[f._i];
    }
    const cFace& faceEntry(face f) const
    {
        return faceBuffer()[f._i];
    }
    cEdge& edgeEntry(edge e)
    {
        return edgeBuffer()[e._i];
    }
    const cEdge& edgeEntry(edge e) const
    {
        return edgeBuffer()[e._i];
    }
    cVert& vertEntry(vertex v)
    {
        return vertBuffer()[v._i];
    }
    const cVert& vertEntry(vertex v) const
    {
        return vertBuffer()[v._i];
    }

//== add entry methods

    tSigned32 addFaceAtEnd(const tFaceData& value)
    {
        tSigned32 result;
        result = SizeL(faceBuffer());
        faceBuffer().push_back(cFace(value));
        _faces++;
        return result;
    }

    tSigned32 addFace(const tFaceData& value)
    {
        tSigned32 result;
        if(_firstDeadFace == -1)
        {
            result = SizeL(faceBuffer());
            faceBuffer().push_back(cFace(value));
        }
        else
        {
            result = _firstDeadFace;
            faceBuffer()[result]._userData = value;
            _firstDeadFace = faceBuffer()[result].nextDead();
        }
        _faces++;
        return result;
    }
    tSigned32 addEdge(const tEdgeData& value)
    {
        tSigned32 result;
        if(_firstDeadEdge == -1)
        {
            result = SizeL(edgeBuffer());
            edgeBuffer().push_back(cEdge(value));
        }
        else
        {
            result = _firstDeadEdge;
            edgeBuffer()[result]._userData = value;
            _firstDeadEdge = edgeBuffer()[result].nextDead();
        }
        _edges++;
        return result;
    }
    tSigned32 addVertAtEnd(const tVertexData& value)
    {
        tSigned32 result;
        result = SizeL(vertBuffer());
        vertBuffer().push_back(cVert(value));
        _verts++;
        return result;
    }
    tSigned32 addVert(const tVertexData& value)
    {
        tSigned32 result;
        if(_firstDeadVert == -1)
        {
            result = SizeL(vertBuffer());
            vertBuffer().push_back(cVert(value));
        }
        else
        {
            result = _firstDeadVert;
            vertBuffer()[result]._userData = value;
            _firstDeadVert = vertBuffer()[result].nextDead();
        }
        _verts++;
        return result;
    }

//== remove entry methods

    void
    markFaceAsDead(tSigned32 i)
    {
        faceEntry(i).markAsDead(_firstDeadFace);
        _firstDeadFace = i;
        _faces--;
    }
    void
    markEdgeAsDead(tSigned32 i)
    {
        edgeEntry(i).markAsDead(_firstDeadEdge);
        _firstDeadEdge = i;
        _edges--;
    }
    void
    markVertAsDead(tSigned32 i)
    {
        vertEntry(i).markAsDead(_firstDeadVert);
        _firstDeadVert = i;
        _verts--;
    }

//== end of add and remove entry methods

    void
    insertEdgeAfter(tSigned32 e)
    {
        tSigned32 after = addEdge(edgeEntry(e)._userData);
        edgeEntry(after).next = edgeEntry(e).next;
        edgeEntry(after).prev = e;
        edgeEntry(after).face = edgeEntry(e).face;
        edgeEntry(edgeEntry(e).next).prev = after;
        edgeEntry(e).next = after;
    }
    void
    removeEdgeAfter(tSigned32 e)
    {
        tSigned32 after = edgeEntry(e).next;
        edgeEntry(e).next = edgeEntry(after).next;
        edgeEntry(edgeEntry(after).next).prev = e;
        faceEntry(edgeEntry(e).face).firstEdge = e;
        markEdgeAsDead(after);
    }

    void
    updateVertexAfterEdgeConnect(tSigned32 e1)
    {
        tSigned32 e2 = edgeEntry(e1).twin;
        if(edgeEntry(e1).vert == edgeEntry(edgeEntry(e2).next).vert)
        {
            assertD(vertEntry(edgeEntry(e1).vert).firstEdge == e1);
            assertD(vertEntry(edgeEntry(e1).vert).lastEdge == edgeEntry(e2).next);
            vertEntry(edgeEntry(e1).vert).lastEdge = -1;
        }
        else
        {
            vertEntry(edgeEntry(e1).vert).firstEdge = vertEntry(edgeEntry(edgeEntry(e2).next).vert).firstEdge;

            tSigned32 vertToDelete = edgeEntry(edgeEntry(e2).next).vert;

            // update vertex pointers for edges around from e2->next
            tSigned32 e = edgeEntry(e2).next;
            while(1)
            {
                edgeEntry(e).vert = edgeEntry(e1).vert;
                if(e == vertEntry(edgeEntry(e1).vert).firstEdge)
                {
                    break;
                }
                assertD(edgeEntry(e).twin != -1);
                e = edgeEntry(edgeEntry(e).twin).next;
            }

            markVertAsDead(vertToDelete);
        }
    }

    void updateVertexAfterEdgeConnect_KeepVertexFromTwin(tSigned32 e1)
    {
        tSigned32 e2 = edgeEntry(e1).twin;
        if(edgeEntry(e1).vert == edgeEntry(edgeEntry(e2).next).vert)
        {
            assertD(vertEntry(edgeEntry(e1).vert).firstEdge == e1);
            assertD(vertEntry(edgeEntry(e1).vert).lastEdge == edgeEntry(e2).next);
            vertEntry(edgeEntry(e1).vert).lastEdge = -1;
        }
        else
        {
            vertEntry(edgeEntry(edgeEntry(e2).next).vert).lastEdge = vertEntry(edgeEntry(e1).vert).lastEdge;
            
            tSigned32 vertToDelete = edgeEntry(e1).vert;

            // update vertex pointers for edges around from e2->next
            tSigned32 e = e1;
            while(1)
            {
                assertD(edgeEntry(e).vert == vertToDelete);
                edgeEntry(e).vert = edgeEntry(edgeEntry(e2).next).vert;
                if(e == vertEntry(vertToDelete).lastEdge)
                {
                    break;
                }
                e = edgeEntry(e).prev;
                e = edgeEntry(e).twin;
                assertD(e != -1);
                assertD(e != e1);
            }

            markVertAsDead(vertToDelete);
        }
    }

    void updateVertexBeforeBreakEdge(tSigned32 e1, bool twinKeepsVertex)
    {
        tSigned32 e2 = edgeEntry(e1).twin;
        tSigned32 v1 = edgeEntry(e1).vert;
        if(vertEntry(v1).lastEdge == -1)
        {
        // v1 was internal
        // so it gets changed to an external vertex
            vertEntry(v1).firstEdge = e1;
            vertEntry(v1).lastEdge = edgeEntry(e2).next;
        }
        else
        {
        // v1 was external
        // so it is now broken into two vertices
            tSigned32 newVert = addVert(vertEntry(v1)._userData);

            if(twinKeepsVertex)
            {
                vertEntry(newVert).firstEdge = e1;
                vertEntry(newVert).lastEdge = vertEntry(v1).lastEdge;
                vertEntry(v1).lastEdge = edgeEntry(e2).next;
            }
            else
            {
                vertEntry(newVert).firstEdge = vertEntry(v1).firstEdge;
                vertEntry(newVert).lastEdge = edgeEntry(e2).next;
                vertEntry(v1).firstEdge = e1;
            }

            // tell edges now assigned to newVert about the change
            tSigned32 e = vertEntry(newVert).firstEdge;
            while(1)
            {
                edgeEntry(e).vert = newVert;
                if(e == vertEntry(newVert).lastEdge)
                {
                    break;
                }
                e = edgeEntry(edgeEntry(e).prev).twin;
                assertD(e != -1);
            }
        }
    }

    void chainEdges(tSigned32 numberOfEdges, tSigned32* newEdges)
    {
        tSigned32 i;
        for(i = 1; i < numberOfEdges - 1; i++)
        {
            edgeEntry(newEdges[i]).prev = newEdges[i - 1];
            edgeEntry(newEdges[i]).next = newEdges[i + 1];
        }
        edgeEntry(newEdges[0]).prev = newEdges[i];
        edgeEntry(newEdges[0]).next = newEdges[1];
        edgeEntry(newEdges[i]).prev = newEdges[i - 1];
        edgeEntry(newEdges[i]).next = newEdges[0];
    }
    void deleteEdgeChain(tSigned32 first)
    {
        tSigned32 e = first;
        do
        {
            tSigned32 toDelete = e;
            e = edgeEntry(e).next;
            markEdgeAsDead(toDelete);
        }
        while(e != first);
    }

    face addFace_Common(
        const F& faceData,
        tSigned32 numberOfEdges,
        const E* edgeData,
        const V* vertexData,
        tSigned32 newFace
        )
    {
        assertD(numberOfEdges > 1);
        cArray<tSigned32> newVertices(numberOfEdges);
        cArray<tSigned32> newEdges(numberOfEdges);

// create edges and vertices, and do most of the initialisation

        tSigned32 i;
        for(i = 0; i < numberOfEdges; i++)
        {
            if(vertexData)
                newVertices[i] = addVert(vertexData[i]);
            else
                newVertices[i] = addVert(V());

            newEdges[i] = addEdge(edgeData[i]);

            vertEntry(newVertices[i]).firstEdge = newEdges[i];
            vertEntry(newVertices[i]).lastEdge = newEdges[i];
            edgeEntry(newEdges[i]).vert = newVertices[i];
            edgeEntry(newEdges[i]).face = newFace;
            edgeEntry(newEdges[i]).twin = -1;
        }

// set edge next and prev pointers

        chainEdges(numberOfEdges, &newEdges.front());

// point face to edges

        faceEntry(newFace).firstEdge = newEdges.front();

        face result;
        result._buffers = _buffers;
        result._i = newFace;
        return result;
    }

    void
    normaliseInternalVertex(vertex v)
    {
        assertD(v.isInternalVertex());
        edge e = firstEdge(v);
        edge lowestIndexedEdge = e;
        while(e.toNextAroundVertex())
        {
            if(e.index() < lowestIndexedEdge.index())
            {
                lowestIndexedEdge = e;
            }
        }
        vertEntry(v).firstEdge = lowestIndexedEdge.index();
    }

    template <class F2,class E2,class V2>
    cVectorMesh(const cVectorMesh<F2,E2,V2>& rhs)
    {
        invalid();
    }
    //.... for some reason the templated version doesnt catch copy with same types ?!
    cVectorMesh(const cVectorMesh& rhs)
    {
        invalid();
    }
    const cVectorMesh<F,E,V> &operator=(const cVectorMesh<F,E,V>& rhs)
    {
        invalid();
    }

public:

//==== entry access methods made public to enable direct copy

    cFace& faceEntry(tSigned32 i)
    {
        return faceBuffer()[i];
    }
    const cFace& faceEntry(tSigned32 i) const
    {
        return faceBuffer()[i];
    }
    cEdge& edgeEntry(tSigned32 i)
    {
        return edgeBuffer()[i];
    }
    const cEdge& edgeEntry(tSigned32 i) const
    {
        return edgeBuffer()[i];
    }
    cVert& vertEntry(tSigned32 i)
    {
        return vertBuffer()[i];
    }
    const cVert& vertEntry(tSigned32 i) const
    {
        return vertBuffer()[i];
    }

    // also first dead entry indices
    tSigned32 getFirstDeadFace_Internal_ForCopy() const
    {
        return _firstDeadFace;
    }
    tSigned32 getFirstDeadEdge_Internal_ForCopy() const
    {
        return _firstDeadEdge;
    }
    tSigned32 getFirstDeadVert_Internal_ForCopy() const
    {
        return _firstDeadVert;
    }

//====

    cVectorMesh()
    {
        _faces = 0;
        _edges = 0;
        _verts = 0;
        _buffers = new nVectorMesh_Internal::cBuffers<F,E,V>();
        _firstDeadFace = -1;
        _firstDeadEdge = -1;
        _firstDeadVert = -1;
    }
    ~cVectorMesh()
    {
        clear();
        delete _buffers;
    }

    tSigned32 faceSize() const
    {
        return SizeL(faceBuffer());
    }
    tSigned32 edgeSize() const
    {
        return SizeL(edgeBuffer());
    }
    tSigned32 vertSize() const
    {
        return SizeL(vertBuffer());
    }

    void clear()
    {
        faceBuffer().clear();
        edgeBuffer().clear();
        vertBuffer().clear();
        _faces = 0;
        _verts = 0;
        _edges = 0;
        _firstDeadFace = -1;
        _firstDeadEdge = -1;
        _firstDeadVert = -1;
    }

    template<class F2, class E2, class V2> void
    copyStructure(cVectorMesh<F2,E2,V2>& rhs)
    {
        _faces = rhs.faces();
        _edges = rhs.edges();
        _verts = rhs.vertices();
        _firstDeadFace = rhs.getFirstDeadFace_Internal_ForCopy();
        _firstDeadEdge = rhs.getFirstDeadEdge_Internal_ForCopy();
        _firstDeadVert = rhs.getFirstDeadVert_Internal_ForCopy();
        faceBuffer().resize(rhs.faceSize());
        edgeBuffer().resize(rhs.edgeSize());
        vertBuffer().resize(rhs.vertSize());
        tSigned32 i;
        for(i = 0; i != rhs.faceSize(); ++i)
        {
            faceEntry(i).firstEdge = rhs.faceEntry(i).firstEdge;
        }
        for(i = 0; i != rhs.edgeSize(); ++i)
        {
            edgeEntry(i).prev = rhs.edgeEntry(i).prev;
            edgeEntry(i).next = rhs.edgeEntry(i).next;
            edgeEntry(i).twin = rhs.edgeEntry(i).twin;
            edgeEntry(i).face = rhs.edgeEntry(i).face;
            edgeEntry(i).vert = rhs.edgeEntry(i).vert;
        }
        for(i = 0; i != rhs.vertSize(); ++i)
        {
            vertEntry(i).firstEdge = rhs.vertEntry(i).firstEdge;
            vertEntry(i).lastEdge = rhs.vertEntry(i).lastEdge;
        }
    }

    template<class F2, class E2, class V2> void
    copy(cVectorMesh<F2,E2,V2>& rhs)
    {
        copyStructure(rhs);
        face f = beginFace();
        typename cVectorMesh<F2,E2,V2>::face rhsF = rhs.beginFace();
        while(f.valid())
        {
            *f = *rhsF;
            ++f;
            ++rhsF;
        }
        edge e = beginEdge();
        typename cVectorMesh<F2,E2,V2>::edge rhsE = rhs.beginEdge();
        while(e.valid())
        {
            *e = *rhsE;
            e.toNextInMesh();
            rhsE.toNextInMesh();
        }
        vertex v = beginVertex();
        typename cVectorMesh<F2,E2,V2>::vertex rhsV = rhs.beginVertex();
        while(v.valid())
        {
            *v = *rhsV;
            ++v;
            ++rhsV;
        }
    }

    template<class F2, class E2, class V2> void
    append(cVectorMesh<F2,E2,V2>& rhs)
    {
        _faces += rhs._faces;
        _edges += rhs._edges;
        _verts += rhs._verts;
        tSigned32 faceStart = faceSize();
        tSigned32 edgeStart = edgeSize();
        tSigned32 vertStart = vertSize();
        faceBuffer().resize(faceStart + rhs.faceSize());
        edgeBuffer().resize(edgeStart + rhs.edgeSize());
        vertBuffer().resize(vertStart + rhs.vertSize());
        tSigned32 i;
        for(i = 0; i != rhs.faceSize(); ++i)
        {
            if(rhs.faceEntry(i).isDead())
            {
                faceEntry(faceStart + i).markAsDead(_firstDeadFace);
                _firstDeadFace = faceStart + i;
            }
            else
            {
                faceEntry(faceStart + i).initFrom(rhs.faceEntry(i));
                faceEntry(faceStart + i).offset(edgeStart);
            }
        }
        for(i = 0; i != rhs.edgeSize(); ++i)
        {
            if(rhs.edgeEntry(i).isDead())
            {
                edgeEntry(edgeStart + i).markAsDead(_firstDeadEdge);
                _firstDeadEdge = edgeStart + i;
            }
            else
            {
                edgeEntry(edgeStart + i).initFrom(rhs.edgeEntry(i));
                edgeEntry(edgeStart + i).offset(faceStart, edgeStart, vertStart);
            }
        }
        for(i = 0; i != rhs.vertSize(); ++i)
        {
            if(rhs.vertEntry(i).isDead())
            {
                vertEntry(vertStart + i).markAsDead(_firstDeadVert);
                _firstDeadVert = vertStart + i;
            }
            else
            {
                vertEntry(vertStart + i).initFrom(rhs.vertEntry(i));
                vertEntry(vertStart + i).offset(edgeStart);
            }
        }
    }

    template <class tVertexPredicate> void
    split(tVertexPredicate vertexPredicate, tThis& splitTo)
    {
        assertD(splitTo.empty());
        assertD(!splitTo.hasDeadEntries());
        if(empty())
        {
            return;
        }
        std::vector<tSigned32> remaps(faceSize() + edgeSize() + vertSize(), -1);
        tSigned32* faceRemaps = &remaps[0];
        tSigned32* edgeRemaps = &remaps[faceSize()];
        tSigned32* vertRemaps = &remaps[faceSize() + edgeSize()];
        vertex v = beginVertex();
        do
        {
            if(!vertexPredicate(v))
            {
                continue;
            }
            vertRemaps[v.index()] = splitTo._verts++;
            edge e = firstEdge(v);
            do
            {
                edgeRemaps[e.index()] = splitTo._edges++;
                tSigned32 fIndex = e.face().index();
                if(faceRemaps[fIndex] == -1)
                {
                    faceRemaps[fIndex] = splitTo._faces++;
                }
            }
            while(e.toNextAroundVertex());
        }
        while((++v).valid());
        splitTo.faceBuffer().resize(splitTo._faces);
        splitTo.edgeBuffer().resize(splitTo._edges);
        splitTo.vertBuffer().resize(splitTo._verts);
        tSigned32 i;
        for(i = 0; i != faceSize(); ++i)
        {
            if(faceRemaps[i] != -1)
            {
                splitTo.faceEntry(faceRemaps[i]).initFrom(faceEntry(i));
                splitTo.faceEntry(faceRemaps[i]).remap(edgeRemaps);
                markFaceAsDead(i);
            }
        }
        for(i = 0; i != edgeSize(); ++i)
        {
            if(edgeRemaps[i] != -1)
            {
                assertD(vertRemaps[edgeAt(i).next().vertex().index()] != -1); // the two sides of the split must not be interconnected
                splitTo.edgeEntry(edgeRemaps[i]).initFrom(edgeEntry(i));
                splitTo.edgeEntry(edgeRemaps[i]).remap(faceRemaps, edgeRemaps, vertRemaps);
                markEdgeAsDead(i);
            }
        }
        for(i = 0; i != vertSize(); ++i)
        {
            if(vertRemaps[i] != -1)
            {
                splitTo.vertEntry(vertRemaps[i]).initFrom(vertEntry(i));
                splitTo.vertEntry(vertRemaps[i]).remap(edgeRemaps);
                markVertAsDead(i);
            }
        }
    }

    void swap(cVectorMesh<F,E,V>& rhs)
    {
        std::swap(_faces, rhs._faces);
        std::swap(_edges, rhs._edges);
        std::swap(_verts, rhs._verts);
        std::swap(_buffers, rhs._buffers);
        std::swap(_firstDeadFace, rhs._firstDeadFace);
        std::swap(_firstDeadEdge, rhs._firstDeadEdge);
        std::swap(_firstDeadVert, rhs._firstDeadVert);
    }

    void
    shrinkToFit()
    {
        ShrinkToFit(faceBuffer());
        ShrinkToFit(edgeBuffer());
        ShrinkToFit(vertBuffer());
    }

    void
    consolidate(
            std::vector<tSigned32>& faceRemaps,
            std::vector<tSigned32>& edgeRemaps,
            std::vector<tSigned32>& vertRemaps
            )
    {
    // build remap vectors

        faceRemaps.resize(0);
        faceRemaps.resize(faceBuffer().size(), -1);
        edgeRemaps.resize(0);
        edgeRemaps.resize(edgeBuffer().size(), -1);
        vertRemaps.resize(0);
        vertRemaps.resize(vertBuffer().size(), -1);

        if(empty())
        {
            return;
        }

        face f = beginFace();
        tSigned32 i;
        for(i = 0; i != _faces; ++i, ++f)
        {
            faceRemaps[f._i] = i;
        }
        assertD(!f.valid());
        edge e = beginEdge();
        for(i = 0; i != _edges; ++i)
        {
            edgeRemaps[e._i] = i;
            e.toNextInMesh();
        }
        assertD(!e.valid());
        vertex v = beginVertex();
        for(i = 0; i != _verts; ++i, ++v)
        {
            vertRemaps[v._i] = i;
        }
        assertD(!v.valid());

    // face and vert buffers are consolidated in place

        std::remove_if(faceBuffer().begin(), faceBuffer().end(), nVectorMesh_Internal::cIsDeadPredicate<F,E,V>());
        faceBuffer().resize(_faces);
        std::remove_if(vertBuffer().begin(), vertBuffer().end(), nVectorMesh_Internal::cIsDeadPredicate<F,E,V>());
        vertBuffer().resize(_verts);

    // edges are copied to new position in a new buffer (because these are not remapped in order)

        {
            std::vector<cEdge> newEdges(_edges);
            for(i = 0; i != SizeL(edgeBuffer()); ++i)
            {
                if(edgeRemaps[i] == -1)
                {
                    continue;
                }
                newEdges[edgeRemaps[i]] = edgeBuffer()[i];
            }
            newEdges.swap(edgeBuffer());
        }

    // reset first dead entry indices

        _firstDeadFace = -1;
        _firstDeadEdge = -1;
        _firstDeadVert = -1;

    // apply remaps internally

        for(i = 0; i != _faces; ++i)
        {
            faceBuffer()[i].remap(VectorBuffer(edgeRemaps));
        }
        for(i = 0; i != _edges; ++i)
        {
            edgeBuffer()[i].remap(VectorBuffer(faceRemaps), VectorBuffer(edgeRemaps), VectorBuffer(vertRemaps));
        }
        for(i = 0; i != _verts; ++i)
        {
            vertBuffer()[i].remap(VectorBuffer(edgeRemaps));
        }
    }
    void
    consolidate()
    {
        std::vector<tSigned32> faceRemaps(faceBuffer().size());
        std::vector<tSigned32> edgeRemaps(edgeBuffer().size());
        std::vector<tSigned32> vertRemaps(vertBuffer().size());
        consolidate(faceRemaps, edgeRemaps, vertRemaps);
    }

    void
    consolidateFaces(
            std::vector<tSigned32>& faceRemaps
            )
    {
    // build remap vector
        faceRemaps.resize(0);
        faceRemaps.resize(faceBuffer().size(), -1);

        if(empty())
        {
            return;
        }

        face f = beginFace();
        tSigned32 i;
        for(i = 0; i != _faces; ++i, ++f)
        {
            faceRemaps[f._i] = i;
        }
        assertD(!f.valid());
    // face buffer is consolidated in place
        std::remove_if(faceBuffer().begin(), faceBuffer().end(), nVectorMesh_Internal::cIsDeadPredicate<F,E,V>());
        faceBuffer().resize(_faces);
    // reset first dead entry
        _firstDeadFace = -1;
    // apply remaps internally
        edge e = beginEdge();
        do
        {
            e.entry().remapFace(VectorBuffer(faceRemaps));
        }
        while(e.toNextInMesh());
    }
    void
    consolidateFaces()
    {
        std::vector<tSigned32> faceRemaps(faceBuffer().size());
        consolidateFaces(faceRemaps);
    }

    void
    consolidateVertices(
            std::vector<tSigned32>& vertRemaps
            )
    {
    // build remap vector
        vertRemaps.resize(0);
        vertRemaps.resize(vertBuffer().size(), -1);

        if(empty())
        {
            return;
        }

        vertex v = beginVertex();
        tSigned32 i;
        for(i = 0; i != _verts; ++i, ++v)
        {
            vertRemaps[v._i] = i;
        }
        assertD(!v.valid());
    // vert buffer is consolidated in place
        std::remove_if(vertBuffer().begin(), vertBuffer().end(), nVectorMesh_Internal::cIsDeadPredicate<F,E,V>());
        vertBuffer().resize(_verts);
    // reset first dead entry
        _firstDeadVert = -1;
    // apply remaps internally
        edge e = beginEdge();
        do
        {
            e.entry().remapVert(VectorBuffer(vertRemaps));
        }
        while(e.toNextInMesh());
    }
    void
    consolidateVertices()
    {
        std::vector<tSigned32> vertRemaps(vertBuffer().size());
        consolidateVertices(vertRemaps);
    }

    tSigned32 faces() const
    {
        return _faces;
    }
    tSigned32 vertices() const
    {
        return _verts;
    }
    tSigned32 edges() const
    {
        return _edges;
    }

    bool
    hasDeadEntries() const
    {
        return _firstDeadFace != -1 || _firstDeadEdge != -1 || _firstDeadVert != -1;
    }

    bool empty() const
    {
        return _faces == 0;
    }
    face beginFace() const
    {
        face result;
        result._buffers = _buffers;
        result._i = 0;
        result.incrementPastDeadEntriesAndInvalidateIfAtEnd();
        return result;
    }
    static face endFace()
    {
        face result;
        result._i = -1;
        return result;
    }
    vertex beginVertex() const
    {
        vertex result;
        result._buffers = _buffers;
        result._i = 0;
        result.incrementPastDeadEntriesAndInvalidateIfAtEnd();
        return result;
    }
    static vertex endVertex()
    {
        vertex result;
        result._i = -1;
        return result;
    }
    static edge beginEdge(face f)
    {
        edge e;
        e._buffers = f._buffers;
        e._i = f.entry().firstEdge;
        return e;
    }
    static edge firstEdge(vertex v)
    {
        edge e;
        e._buffers = v._buffers;
        e._i = v.entry().firstEdge;
        return e;
    }
    static edge lastEdge(vertex v)
    {
        edge e;
        e._buffers = v._buffers;
        e._i = v.entry().lastEdge;
        return e;
    }
    edge beginEdge() const
    {
        face f = beginFace();
        if(f.valid())
        {
            return beginEdge(f);
        }
        edge result;
        result._i = -1;
        return result;
    }
    static edge endEdge()
    {
        edge result;
        result._i = -1;
        return result;
    }

    face faceAt(tSigned32 i) const
    {
        face result;
        result._buffers = _buffers;
        result._i = i;
        return result;
    }
    edge edgeAt(tSigned32 i) const
    {
        edge result;
        result._buffers = _buffers;
        result._i = i;
        return result;
    }
    vertex vertAt(tSigned32 i) const
    {
        vertex result;
        result._buffers = _buffers;
        result._i = i;
        return result;
    }

    F& faceDataAt(tSigned32 i) const
    {
        return const_cast<F&>(faceEntry(i)._userData);
    }
    E& edgeDataAt(tSigned32 i) const
    {
        return const_cast<E&>(edgeEntry(i)._userData);
    }
    V& vertDataAt(tSigned32 i) const
    {
        return const_cast<V&>(vertEntry(i)._userData);
    }

    bool
    ownsFace(face f) const
    {
        return f._buffers == _buffers;
    }
    bool
    ownsEdge(edge e) const
    {
        return e._buffers == _buffers;
    }
    bool
    ownsVertex(vertex v) const
    {
        return v._buffers == _buffers;
    }

    void eraseFace(face toErase)
    {
        assertD(_faces);

// disconnect if connected to the rest of the mesh

        //... optimise - vertex updating can be made more efficient
        //... this way, vertices will be constructed in breakEdge and subsequently deleted
        edge e = beginEdge(toErase);
        do
        {
            if(e.twin().valid())
            {
                breakEdge(e.twin());
            }
        }
        while(++e != beginEdge(toErase));

        e = beginEdge(toErase);
        do
        {
            assertD(e.vertex().entry().firstEdge == e._i);
            assertD(e.vertex().entry().lastEdge == e._i);
            markVertAsDead(e.entry().vert);
        }
        while(++e != beginEdge(toErase));

// delete edges

        deleteEdgeChain(toErase.entry().firstEdge);

// delete face

        markFaceAsDead(toErase._i);
    }

    void
    setBeginEdge(face f, edge e)
    {
        assertD(e.face() == f);
        f.entry().firstEdge = e._i;
    }

    // if vertices must be deleted, then the vertices connected to edge2 will be deleted
    // the vertices connected to edge1 are therefore guaranteed not to be deleted by this call
    void
    connectEdges(edge edge1, edge edge2)
    {
        assertD(!edge1.twin().valid());
        assertD(!edge2.twin().valid());

        tSigned32 e1 = edge1._i;
        tSigned32 e2 = edge2._i;

        edgeEntry(e1).twin = e2;
        edgeEntry(e2).twin = e1;

        updateVertexAfterEdgeConnect(e1);
        updateVertexAfterEdgeConnect_KeepVertexFromTwin(e2);
    }

    void
    connectEdges_LeaveVertsUninitialised(edge edge1, edge edge2)
    {
        assertD(edge1.vertex().entry().firstEdge == -1);
        assertD(edge2.vertex().entry().firstEdge == -1);
        assertD(!edge1.twin().valid());
        assertD(!edge2.twin().valid());

        tSigned32 e1 = edge1._i;
        tSigned32 e2 = edge2._i;

        edgeEntry(e1).twin = e2;
        edgeEntry(e2).twin = e1;
    }

    // if new vertices must be created then they will be assigned to e.twin()
    // therefore edge e is guaranteed to keep it's vertices
    void
    breakEdge(edge e)
    {
        tSigned32 e1 = e._i;
        tSigned32 e2 = edgeEntry(e1).twin;
        assertD(e2 != -1);

        updateVertexBeforeBreakEdge(e1, false);
        updateVertexBeforeBreakEdge(e2, true);

        edgeEntry(e1).twin = -1;
        edgeEntry(e2).twin = -1;
    }

    void
    splitEdge(edge e, const V& vertexData, const E& edgeData1, const E& edgeData2)
    {
        tSigned32 newVert = addVert(vertexData);

        insertEdgeAfter(e._i);
        e.next().entry().twin = e.entry().twin;

        e.next().entry().vert = newVert;
        vertEntry(newVert).firstEdge = e.entry().next;

        if(e.twin().valid())
        {
            insertEdgeAfter(e.entry().twin);
            e.twin().next().entry().twin = e._i;

            e.twin().next().entry().vert = newVert;

            e.twin().entry().twin = e.entry().next;
            e.entry().twin = e.twin().entry().next;

            vertEntry(newVert).lastEdge = -1;
            *e.twin() = edgeData2;
        }
        else
        {
            vertEntry(newVert).lastEdge = e.entry().next;
        }
        *e.next() = edgeData1;
    }
    void
    splitEdge(edge e, V vertexData = V(), E edgeData = E())
    {
        splitEdge(e, vertexData, edgeData, edgeData);
    }
    void unsplitEdge(edge e)
    {
        markVertAsDead(e.next().entry().vert);

        if(e.twin().valid())
        {
            assertD(e.next().twin().valid());
            assertD(e.next().twin() == e.twin().prev());
            tSigned32 eDiagonal = e.twin().prev()._i;
            removeEdgeAfter(e._i);
            removeEdgeAfter(eDiagonal);
            e.entry().twin = eDiagonal;
            edgeEntry(eDiagonal).twin = e._i;
        }
        else
        {
            assertD(!e.next().twin().valid());
            removeEdgeAfter(e._i);
        }
    }


    // _userData for created edges is default initialised
    // edge1 keeps the existing vertex, edge2 gets the new vertex
    void splitVertex(edge edge1, edge edge2)
    {
        tSigned32 e1 = edge1._i;
        tSigned32 v = edge1.entry().vert;
        assertD(vertEntry(v).lastEdge == -1); // only internal vertices are handled

        tSigned32 e2 = edge2._i;
        assertD(edge2.entry().vert == v);

        tSigned32 newVert = addVert(vertEntry(v)._userData);

        tSigned32 beforeE1 = addEdge(tEdgeData());
        edgeEntry(beforeE1).face = edgeEntry(e1).face;
        edgeEntry(beforeE1).vert = newVert;
        edgeEntry(edgeEntry(e1).prev).next = beforeE1;
        edgeEntry(beforeE1).prev = edgeEntry(e1).prev;
        edgeEntry(beforeE1).next = e1;
        edgeEntry(e1).prev = beforeE1;

        tSigned32 beforeE2 = addEdge(tEdgeData());
        edgeEntry(beforeE2).face = edgeEntry(e2).face;
        edgeEntry(beforeE2).vert = v;
        edgeEntry(edgeEntry(e2).prev).next = beforeE2;
        edgeEntry(beforeE2).prev = edgeEntry(e2).prev;
        edgeEntry(beforeE2).next = e2;
        edgeEntry(e2).prev = beforeE2;

        edgeEntry(beforeE1).twin = beforeE2;
        edgeEntry(beforeE2).twin = beforeE1;

        vertEntry(v).firstEdge = e1;
        vertEntry(newVert).firstEdge = e2;
        vertEntry(newVert).lastEdge = -1;

      // update vertex pointer for edges now belonging to newVert
        tSigned32 e = edgeEntry(edgeEntry(beforeE1).prev).twin;
        edgeEntry(e).vert = newVert;
        while(e != e2)
        {
            e = edgeEntry(edgeEntry(e).prev).twin;
            edgeEntry(e).vert = newVert;
        }
    }

    vertex addUnconnectedVertex(const V& userData)
    {
        vertex result;
        result._buffers = _buffers;
        result._i = addVert(userData);
        result.entry().firstEdge = -1;
        result.entry().lastEdge = -1;
        return result;
    }

    vertex addUnconnectedVertexAtEnd(const V& userData)
    {
        vertex result;
        result._buffers = _buffers;
        result._i = addVertAtEnd(userData);
        result.entry().firstEdge = -1;
        result.entry().lastEdge = -1;
        return result;
    }

    void
    initialiseEdgeVertexIfUninitialised(edge e)
    {
        if(e.vertex().entry().firstEdge != -1)
        {
          // already initialised
            return;
        }
        edge firstE = e;
        do
        {
            --e;
            e = e.twin();
        }
        while(e.valid() && e != firstE);
        if(e == firstE)
        {
          // internal vertex
            e.vertex().entry().firstEdge = e._i;
            e.vertex().entry().lastEdge = -1;
            return;
        }
        e = firstE;
        while(e.twin().valid())
        {
            e = e.twin().next();
        }
        assertD(e.vertex() == firstE.vertex());
        e.vertex().entry().firstEdge = e._i;
        e = firstE;
        while(e.prev().twin().valid())
        {
            e = e.prev().twin();
        }
        assertD(e.vertex() == firstE.vertex());
        e.vertex().entry().lastEdge = e._i;
    }
    void
    eraseUninitialisedVertices()
    {
        vertex v = beginVertex();
        while(v.valid())
        {
            vertex toProcess = v;
            ++v;
            if(toProcess.entry().firstEdge == -1)
            {
            // uninitialised
                markVertAsDead(toProcess._i);
            }
        }
    }
    void
    fixEdgesForAnyMultiplyAssignedVerts()
    {
        edge e;
        tSigned32 edgeInFaceCount = 0;
        e = beginEdge();
        while(e.valid())
        {
            ++edgeInFaceCount;
            e.toNextInMesh();
        }
        tSigned32 edgeAroundVertexCount = 0;
        vertex v = beginVertex();
        while(v.valid())
        {
            e = firstEdge(v);
            do
            {
                ++edgeAroundVertexCount;
            }
            while(e.toNextAroundVertex());
            ++v;
        }
        if(edgeInFaceCount == edgeAroundVertexCount)
        {
            return;
        }
        assertD(edgeInFaceCount > edgeAroundVertexCount);
      // some edges must have bad vertex assignments
        e = beginEdge();
        do
        {
            v = e.vertex();
            edge e2 = firstEdge(v);
            do
            {
                if(e2 == e)
                {
                    break;
                }
            }
            while(e2.toNextAroundVertex());
            if(e2.valid())
            {
              // this edge is ok
                continue;
            }

          // this edge needs a new vertex

            tSigned32 addedIndex = addVert(*v);
            
            // figure out whether this is an internal vertex, and if external find first and last edges
            edge firstAround = e;
            while(firstAround.twin().valid())
            {
                firstAround = firstAround.twin().next();
                if(firstAround == e)
                {
                    break;
                }
            }
            vertEntry(addedIndex).firstEdge = firstAround._i;
            if(firstAround.twin().valid())
            {
                // internal vertex
                vertEntry(addedIndex).lastEdge = -1;
            }
            else
            {
                // external vertex
                edge lastAround = e;
                while(lastAround.prev().twin().valid())
                {
                    lastAround = lastAround.prev().twin();
                }
                vertEntry(addedIndex).lastEdge = lastAround._i;
            }

            // update vertex indices for edges around the new vertex to point back to this vertex

            vertex newV;
            newV._buffers = _buffers;
            newV._i = addedIndex;

            edge eToUpdate = firstEdge(newV);
            do
            {
                eToUpdate.entry().vert = addedIndex;
            }
            while(eToUpdate.toNextAroundVertex());
        }
        while(e.toNextInMesh());
    }

    face
    addFaceAtEnd(
            const F& faceData,
            tSigned32 numberOfEdges,
            const E* edgeData,
            const V* vertexData
            )
    {
        tSigned32 newFace;
        newFace = addFaceAtEnd(faceData);
        return addFace_Common(faceData, numberOfEdges, edgeData, vertexData, newFace);
    }

    // the face's firstEdge will be constructed from the first entries in the data arrays
    face addFace(
        const F& faceData,
        tSigned32 numberOfEdges,
        const E* edgeData,
        const V* vertexData
        )
    {
        tSigned32 newFace;
        newFace = addFace(faceData);
        return addFace_Common(faceData, numberOfEdges, edgeData, vertexData, newFace);
    }


    face
    addFaceAtEnd_LeaveVertsUninitialised(
            const F& faceData,
            tSigned32 numberOfEdges,
            const E* edgeData,
            const vertex* vertices
            )
    {
        assertD(numberOfEdges > 1);

        tSigned32 newFace;
        newFace = addFaceAtEnd(faceData);

        cArray<tSigned32> newEdges(numberOfEdges);

// create edges and do most of the initialisation

        tSigned32 i;
        for(i = 0; i < numberOfEdges; i++)
        {
            assertD(vertices[i].entry().firstEdge == -1);
            newEdges[i] = addEdge(edgeData[i]);
            edgeEntry(newEdges[i]).vert = vertices[i]._i;
            edgeEntry(newEdges[i]).face = newFace;
            edgeEntry(newEdges[i]).twin = -1;
        }

// set edge next and prev pointers

        chainEdges(numberOfEdges, &newEdges.front());

// point face to edges

        faceEntry(newFace).firstEdge = newEdges.front();

        face result;
        result._buffers = _buffers;
        result._i = newFace;
        return result;
    }


    // the 'after' array enables splits to be performed with numberOfSplits not equal to edges around the face
    vertex
    splitFaceWithVertex(face f,
            tSigned32 numberOfSplits,
            const V& vertexData, 
            const E* edgeData, // points to numberOfSplits * 2 entries
            edge* after
            )
    {
        tSigned32 newVert = addVert(vertexData);

        cArray<tSigned32> leftEdge(numberOfSplits);
        cArray<tSigned32> rightEdge(numberOfSplits);

        tSigned32 i;

// create 'spoke' edges first

        for(i = 0; i < numberOfSplits; i++)
        {
            leftEdge[i] = addEdge(E());
            rightEdge[i] = addEdge(E());
        }

        // connect together

        edgeEntry(leftEdge[0]).twin = rightEdge[numberOfSplits - 1];
        edgeEntry(rightEdge[numberOfSplits - 1]).twin = leftEdge[0];

        for(i = 1; i < numberOfSplits; i++)
        {
            edgeEntry(leftEdge[i]).twin = rightEdge[i - 1];
            edgeEntry(rightEdge[i - 1]).twin = leftEdge[i];
        }

// create the new faces, also performing the bulk of initialisation

        for(i = 0; i < numberOfSplits; i++)
        {
            tSigned32 nextI = i + 1;
            if(nextI == numberOfSplits)
            {
                nextI = 0;
            }

            tSigned32 edgesAround = 1;
            {
                edge e = after[i].next();
                while(e != after[nextI])
                {
                    edgesAround++;
                    ++e;
                }
            }

            cArray<tSigned32> newEdges(edgesAround + 2);

            tSigned32 newFace = addFace(*f);

            newEdges[0] = leftEdge[i];
            edgeEntry(newEdges[0]).face = newFace;
            edgeEntry(newEdges[0]).vert = newVert;
            edgeEntry(newEdges[0])._userData = edgeData[i * 2 + 0];

            {
                tSigned32 e = after[i]._i;
                tSigned32 j;
                for(j = 1; j < edgesAround + 1; j++)
                {
                    newEdges[j] = e;
                    edgeEntry(newEdges[j]).face = newFace;
                    e = edgeEntry(e).next;
                }
                newEdges[j] = rightEdge[i];
                edgeEntry(newEdges[j]).face = newFace;
                edgeEntry(newEdges[j]).vert = edgeEntry(e).vert;
                edgeEntry(newEdges[j])._userData = edgeData[i * 2 + 1];
            }

            // update lastEdge for the vertex at the spoke as appropriate
            if(vertEntry(edgeEntry(newEdges[1]).vert).lastEdge == newEdges[1])
            {
                vertEntry(edgeEntry(newEdges[1]).vert).lastEdge = edgeEntry(leftEdge[i]).twin;
            }

            chainEdges(edgesAround + 2, &newEdges.front());
            faceEntry(newFace).firstEdge = newEdges[0];
        }

// set up vertex as an internal vertex

        vertEntry(newVert).firstEdge = leftEdge[0];
        vertEntry(newVert).lastEdge = -1;

        markFaceAsDead(f._i);

        vertex result;
        result._buffers = _buffers;
        result._i = newVert;
        return result;
    }

    void
    splitFace(
            edge after1, edge after2,
            const E &userdata12, const E &userdata21
            )
    {
        assertD(after1.face() == after2.face());
        edge edge12, edge21;
        edge before1, before2;
        edge i = beginEdge(after1.face());
        do
        {
            ++i;
        }
        while(i != after1 && i != after2);

        before1 = after1.prev();
        insertEdgeAfter(before1._i);
        edge12 = before1.next();
        before2 = after2.prev();
        insertEdgeAfter(before2._i);
        edge21 = before2.next();

        *edge12 = userdata12;
        *edge21 = userdata21;

        edge12.entry().next = after2._i;
        after2.entry().prev = edge12._i;

        edge21.entry().next = after1._i;
        after1.entry().prev = edge21._i;

        edge12.entry().twin = edge21._i;
        edge21.entry().twin = edge12._i;

        edge12.entry().vert = after1.entry().vert;
        edge21.entry().vert = after2.entry().vert;

        if(edge12.vertex().entry().lastEdge == after1._i)
        {
            edge12.vertex().entry().lastEdge = edge12._i;
        }
        if(edge21.vertex().entry().lastEdge == after2._i)
        {
            edge21.vertex().entry().lastEdge = edge21._i;
        }

        tSigned32 newFace = addFace(*(i.face()));
        faceEntry(newFace).firstEdge = i._i;
        {
            tSigned32 e = faceEntry(newFace).firstEdge;
            do
            {
                edgeEntry(e).face = newFace;
                e = edgeEntry(e).next;
            }
            while(e != faceEntry(newFace).firstEdge);
        }
    }

    void
    splitFace(edge after1, edge after2)
    {
        // bugfix: added these local variables because references passed in directly could be invalidated by vector reallocation
        E userdata12 = *after1;
        E userdata21 = *after2;
        splitFace(after1, after2, userdata12, userdata21);
    }

    // the face for the edge passed in will be reused,
    // so the resulting joined face attributes will come from that face
    face
    joinFaces(edge e)
    {
        face faceReused = e.face();

        assertD(faceReused != e.twin().face());
        // ** following code commented out to save code bloat, since it is not required
        // ** (simplifying meshes just by removing edges in the order they come is not as simple as it looks, because, for example, this can result in the creation of 'islands')
//        if(faceReused == e.twin().face())
//        {
//        // degenerate case where this is the last edge left after removing edges around a vertex
//
//            if(e.twin() != e.next())
//            {
//                --e;
//                assertD(e.twin() == e.next());
//            }
//            edge e2 = e.twin();
//            assertD(e.prev() != e2);
//            assertD(e.vertex() == e2.next().vertex()); // could construct situations where this is not true
//
//            // ensure face first edge is not one of the two that will be removed
//            faceEntry(faceReused).firstEdge = e.prev()._i;
//
//            vertex vCorner = e.vertex();
//            vertex vToDelete = e.next().vertex();
//
//            // in case of internal vertex ensure vertex first edge is not one of the two that will be removed
//            if(vertEntry(vCorner).firstEdge == e._i)
//            {
//                assertD(vertEntry(vCorner).lastEdge == -1);
//                vertEntry(vCorner).firstEdge = e.next().next()._i;
//            }
//
//            edge eBefore = e.prev();
//            edge eAfter = e.next().next();
//            edgeEntry(eBefore).next = eAfter._i;
//            edgeEntry(eAfter).prev = eBefore._i;
//
//            markEdgeAsDead(e._i);
//            markEdgeAsDead(e2._i);
//            markVertAsDead(vToDelete._i);
//
//            return faceReused;
//        }

        tSigned32 e1 = e._i;
        assertD(e.twin().valid());
        tSigned32 e2 = e.twin()._i;

        tSigned32 beforeLeft = edgeEntry(e1).prev;
        tSigned32 afterLeft = edgeEntry(e2).next;
        tSigned32 beforeRight = edgeEntry(e2).prev;
        tSigned32 afterRight = edgeEntry(e1).next;

        tSigned32 v1 = edgeEntry(e1).vert;
        assertD(vertEntry(v1).lastEdge == -1 || vertEntry(v1).firstEdge != e1);
        if(vertEntry(v1).lastEdge == e1)
        {
            vertEntry(v1).lastEdge = afterLeft;
        }
        if(vertEntry(v1).firstEdge == e1)
        {
            vertEntry(v1).firstEdge = afterLeft;
        }
        tSigned32 v2 = edgeEntry(e2).vert;
        assertD(vertEntry(v2).lastEdge == -1 || vertEntry(v2).firstEdge != e2);
        if(vertEntry(v2).lastEdge == e2)
        {
            vertEntry(v2).lastEdge = afterRight;
        }
        if(vertEntry(v2).firstEdge == e2)
        {
            vertEntry(v2).firstEdge = afterRight;
        }

        edgeEntry(beforeLeft).next = afterLeft;
        edgeEntry(afterLeft).prev = beforeLeft;
        edgeEntry(beforeRight).next = afterRight;
        edgeEntry(afterRight).prev = beforeRight;
        while(1)
        {
            edgeEntry(afterLeft).face = edgeEntry(beforeLeft).face;
            if(afterLeft == beforeRight)
            {
                break;
            }
            afterLeft = edgeEntry(afterLeft).next;
        }
        if(faceEntry(edgeEntry(e1).face).firstEdge == e1)
        {
            faceEntry(edgeEntry(e1).face).firstEdge = beforeLeft;
        }

        markFaceAsDead(edgeEntry(e2).face);
        markEdgeAsDead(e1);
        markEdgeAsDead(e2);

        return faceReused;
    }

    void collapseEdge(edge toCollapse)
    {
        tSigned32 e1 = toCollapse._i;

        // unlink e1 from chain

        tSigned32 after1 = edgeEntry(e1).next;
        edgeEntry(edgeEntry(e1).prev).next = after1;
        edgeEntry(after1).prev = edgeEntry(e1).prev;
        if(faceEntry(edgeEntry(e1).face).firstEdge == e1)
        {
            faceEntry(edgeEntry(e1).face).firstEdge = after1;
        }

        tSigned32 e2 = edgeEntry(e1).twin;

        tSigned32 v1 = edgeEntry(e1).vert;
        tSigned32 v2 = edgeEntry(edgeEntry(e1).next).vert;

        if(e2 == -1)
        {
            vertEntry(v1).firstEdge = vertEntry(v2).firstEdge;
            if(vertEntry(v1).lastEdge == e1)
            {
                vertEntry(v1).lastEdge = after1;
            }

            // update vertex pointers for edges now owned by v1
            tSigned32 e = vertEntry(v1).firstEdge;
            edgeEntry(e).vert = v1;
            while(e != after1)
            {
                e = edgeEntry(edgeEntry(e).prev).twin;
                edgeEntry(e).vert = v1;
            }

            markEdgeAsDead(e1);
            markVertAsDead(v2);
            return;
        }

        // unlink e2 from chain

        tSigned32 after2 = edgeEntry(e2).next;
        edgeEntry(edgeEntry(e2).prev).next = after2;
        edgeEntry(after2).prev = edgeEntry(e2).prev;
        if(faceEntry(edgeEntry(e2).face).firstEdge == e2)
        {
            faceEntry(edgeEntry(e2).face).firstEdge = after2;
        }

        if(vertEntry(v1).lastEdge != -1 && vertEntry(v2).lastEdge != -1)
        {
        // case where the collapse results in a break in the mesh
        // both vertices get reused

            std::swap(vertEntry(v1).firstEdge, vertEntry(v2).firstEdge);
            tSigned32 e;
            
            if(vertEntry(v1).lastEdge == e1)
            {
                vertEntry(v1).lastEdge = after1;
            }
            if(vertEntry(v2).lastEdge == e2)
            {
                vertEntry(v2).lastEdge = after2;
            }

            // update vertex pointers for edges now owned by v1

            e = vertEntry(v1).firstEdge;
            edgeEntry(e).vert = v1;
            while(e != after1)
            {
                e = edgeEntry(edgeEntry(e).prev).twin;
                edgeEntry(e).vert = v1;
            }

            // update vertex pointers for edges now owned by v2

            e = vertEntry(v2).firstEdge;
            edgeEntry(e).vert = v2;
            while(e != after2)
            {
                e = edgeEntry(edgeEntry(e).prev).twin;
                assertD(e != -1);
                assertD(e != vertEntry(v2).firstEdge);
                edgeEntry(e).vert = v2;
            }
        }
        else
        {
        // join vertices
        
            if(vertEntry(v2).lastEdge == -1)
            {
            // v1 may or may not be internal, and is used for new vertex
            // v2 is internal

                if(vertEntry(v1).lastEdge == e1)
                {
                    vertEntry(v1).lastEdge = after1;
                }

                // update pointers for edges now owned by v1
                tSigned32 e;
                e = after1;
                do
                {
                    edgeEntry(e).vert = v1;
                    e = edgeEntry(edgeEntry(e).twin).next;
                }
                while(e != after2);

                markVertAsDead(v2);
            }
            else
            {
            // v2 may or may not be internal, and is used for new vertex
            // v1 is internal
            
                if(vertEntry(v2).lastEdge == e2)
                {
                    vertEntry(v2).lastEdge = after2;
                }

                // update pointers for edges now owned by v2
                tSigned32 e;
                e = after2;
                do
                {
                    edgeEntry(e).vert = v2;
                    e = edgeEntry(edgeEntry(e).twin).next;
                }
                while(e != after1);

                markVertAsDead(v1);
            }
        }
        markEdgeAsDead(e1);
        markEdgeAsDead(e2);
    }

    void
    flipEdge(edge toFlip)
    {
        assertD(toFlip.twin().valid());
        assertD(toFlip.next().twin() != toFlip.twin().prev());
        assertD(toFlip.prev().twin() != toFlip.twin().next());

    // the pair of edges is rotated clockwise
        tSigned32 e1 = toFlip._i;
        tSigned32 e2 = edgeEntry(e1).twin;

        tSigned32 before1 = edgeEntry(e1).prev;
        tSigned32 after1 = edgeEntry(e1).next;
        tSigned32 before2 = edgeEntry(e2).prev;
        tSigned32 after2 = edgeEntry(e2).next;

      // update faces for the edges that get rotated past

        edgeEntry(edgeEntry(e1).next).face = edgeEntry(e2).face;
        edgeEntry(edgeEntry(e2).next).face = edgeEntry(e1).face;
        
      // redirect edge sequences

        edgeEntry(before2).next = after1;
        edgeEntry(after1).prev = before2;
        edgeEntry(before1).next = after2;
        edgeEntry(after2).prev = before1;

        edgeEntry(e1).prev = after2;
        edgeEntry(after2).next = e1;
        edgeEntry(e1).next = before1;
        edgeEntry(before1).prev = e1;

        edgeEntry(e2).prev = after1;
        edgeEntry(after1).next = e2;
        edgeEntry(e2).next = before2;
        edgeEntry(before2).prev = e2;

      // ensure each face still has a valid first edge

        faceEntry(edgeEntry(e1).face).firstEdge = e1;
        faceEntry(edgeEntry(e2).face).firstEdge = e2;

      // vertices for e1 and e2 before and after the change

        tSigned32 v1 = edgeEntry(e1).vert;
        tSigned32 vAfter1 = edgeEntry(before2).vert;
        tSigned32 v2 = edgeEntry(e2).vert;
        tSigned32 vAfter2 = edgeEntry(before1).vert;

      // update original vertices for removal of the edges

        if(vertEntry(v1).firstEdge == e1)
        {
            assertD(vertEntry(v1).lastEdge == -1);
            vertEntry(v1).firstEdge = after2;
        }
        else
        if(vertEntry(v1).lastEdge == e1)
        {
            vertEntry(v1).lastEdge = after2;
        }
        if(vertEntry(v2).firstEdge == e2)
        {
            assertD(vertEntry(v2).lastEdge == -1);
            vertEntry(v2).firstEdge = after1;
        }
        else
        if(vertEntry(v2).lastEdge == e2)
        {
            vertEntry(v2).lastEdge = after1;
        }

      // update new vertices for addition of the edges

        if(vertEntry(vAfter1).lastEdge == before2)
        {
            vertEntry(vAfter1).lastEdge = e1;
        }
        if(vertEntry(vAfter2).lastEdge == before1)
        {
            vertEntry(vAfter2).lastEdge = e2;
        }

      // update the rotated edges to point to their new vertices

        edgeEntry(e1).vert = vAfter1;
        edgeEntry(e2).vert = vAfter2;
    }

    face
    joinFaces(vertex vert)
    {
        assertD(vert.isInternalVertex());
        tSigned32 v = vert._i;

        tSigned32 firstSpoke = vertEntry(v).firstEdge;
        tSigned32 firstE = edgeEntry(firstSpoke).next;
        tSigned32 firstFace = edgeEntry(firstE).face; // this is the face that will be reused

      // ensure that firstEdge ptr from firstFace will remain valid
        faceEntry(firstFace).firstEdge = firstE;

        tSigned32 spoke = edgeEntry(edgeEntry(firstSpoke).prev).twin;
        while(1) // note that this supports internal vertices with just one edge
        {
            tSigned32 before = edgeEntry(edgeEntry(spoke).twin).prev;
            tSigned32 after = edgeEntry(spoke).next;

            // doesnt support a pathological situation where the same face connects to this vertex twice
            assertD(after != edgeEntry(before).twin);

          // connect edges before and after the spoke
            edgeEntry(before).next = after;
            edgeEntry(after).prev = before;

            tSigned32 spokeV = edgeEntry(after).vert;

          // update spokeV for the removal of the spoke edge

            if(vertEntry(spokeV).firstEdge == edgeEntry(spoke).twin)
            {
                assertD(vertEntry(spokeV).lastEdge == -1);
                vertEntry(spokeV).firstEdge = after;
            }
            else
            if(vertEntry(spokeV).lastEdge == edgeEntry(spoke).twin)
            {
                vertEntry(spokeV).lastEdge = after;
            }

            if(spoke == firstSpoke)
            {
                markEdgeAsDead(edgeEntry(spoke).twin);
                markEdgeAsDead(spoke);
                break;
            }

            // doesnt support a pathological situation where the same face connects to this vertex twice
            assertD(edgeEntry(spoke).face != firstFace);

          // assign edges around with the new face
            tSigned32 e = after;
            do
            {
                edgeEntry(e).face = firstFace;
                e = edgeEntry(e).next;
            }
            while(e != edgeEntry(spoke).prev);

            tSigned32 f = edgeEntry(spoke).face;

            markEdgeAsDead(edgeEntry(spoke).twin);
            markEdgeAsDead(spoke);
            spoke = edgeEntry(e).twin;

            markFaceAsDead(f);
        }

        markVertAsDead(v);

        face result;
        result._buffers = _buffers;
        result._i = firstFace;
        return result;
    }

    void
    normaliseInternalVertices()
    {
        vertex v = beginVertex();
        while(v.valid())
        {
            if(v.isInternalVertex())
            {
                normaliseInternalVertex(v);
            }
            ++v;
        }
    }
};

#endif

