//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_WITH_TRACKED_TRANSFORM_INCLUDED
#define MESH_WITH_TRACKED_TRANSFORM_INCLUDED

#include "libs/Mesh_Common/interface/MeshWithTrackedTransform_Operations.h"
#include "libs/Mesh_Common/interface/iMeshTranslator.h"
#include "common/TemplateMesh/ElementIndexing.h"

// main class definition

template <class tMesh>
class cMeshWithTrackedTransform : public iMeshTranslator<tMesh>
{
public:

    typedef typename tMesh::tFaceData tFaceData;
    typedef typename tMesh::tEdgeData tEdgeData;
    typedef typename tMesh::tVertexData tVertexData;

    typedef typename tMesh::face face;
    typedef typename tMesh::edge edge;
    typedef typename tMesh::vertex vertex;

private:

    std::vector<tSigned32> _sources;
    std::vector<tSigned32> _results;

    tMesh* _originalMesh;
    tMesh* _transformedMesh;
    nTransformTree::cTree _tree;

    void clearSourcesAndResults()
    {
        _sources.resize(0);
        _results.resize(0);
    }
    void addSource(face f)
    {
        _sources.push_back(IndexForElement<tMesh>(f));
    }
    void addSource(edge e)
    {
        _sources.push_back(IndexForElement<tMesh>(e));
    }
    void addSource(vertex v)
    {
        _sources.push_back(IndexForElement<tMesh>(v));
    }
    void addResult(face f)
    {
        _results.push_back(IndexForElement<tMesh>(f));
    }
    void addResult(edge e)
    {
        _results.push_back(IndexForElement<tMesh>(e));
    }
    void addResult(vertex v)
    {
        _results.push_back(IndexForElement<tMesh>(v));
    }

public:

    // a reference to the original mesh will be retained
    // this must remain valid and unchanged during the entire lifetime of the constructed object
    cMeshWithTrackedTransform(tMesh* originalMesh) :
     _originalMesh(originalMesh),
     _transformedMesh(new tMesh),
     _tree(TopElement(*_originalMesh))
    {
        _transformedMesh->copy(*_originalMesh);
    }

    ~cMeshWithTrackedTransform()
    {
        delete _transformedMesh;
    }

    // this object cannot be used afterwards, except for calling the destructor
    tMesh* extractMesh()
    {
        tMesh* result = _transformedMesh;
        _transformedMesh = 0;
        return result;
    }
    // this object cannot be used afterwards, except for calling the destructor
    void extractMesh(tMesh& swapOutTo)
    {
        assertD(swapOutTo.empty());
        _transformedMesh->swap(swapOutTo);
    }

    cMeshElement<tMesh>
    translate(const cMeshElement<tMesh>& el, const iPointLike& p) const
    {
        tSigned32 startIndex = IndexForElement(el);
        tSigned32 endIndex = _tree.transformDown(startIndex, p);
        return ElementAtIndex(*_transformedMesh, endIndex);
    }
    cMeshElement<tMesh>
    translateBack(const cMeshElement<tMesh>& el, const iPointLike& p) const
    {
        tSigned32 startIndex = IndexForElement(el);
        tSigned32 endIndex = _tree.transformUp(startIndex, p);
        return ElementAtIndex(*_originalMesh, endIndex);
    }

// access to underlying mesh for when stuff cant be wrapped

    tMesh& mesh() {return *_transformedMesh;}
    const tMesh& mesh() const {return *_transformedMesh;}

// methods passed on to the mesh

    tSigned32 faces() const {return _transformedMesh->faces();}
    tSigned32 vertices() const {return _transformedMesh->vertices();}
    tSigned32 edges() const {return _transformedMesh->edges();}
    face beginFace() {return _transformedMesh->beginFace();}
    edge beginEdge() {return _transformedMesh->beginEdge();}
    vertex beginVertex() {return _transformedMesh->beginVertex();}
    face faceAt(tSigned32 index) {return _transformedMesh->faceAt(index);}
    edge edgeAt(tSigned32 index) {return _transformedMesh->edgeAt(index);}
    vertex vertAt(tSigned32 index) {return _transformedMesh->vertAt(index);}
    static edge beginEdge(face f) {return tMesh::beginEdge(f);}
    static edge firstEdge(vertex v) {return tMesh::firstEdge(v);}
    static edge lastEdge(vertex v) {return tMesh::lastEdge(v);}
    static edge endEdge() {return tMesh::endEdge();}

// intercepted methods

    void
    consolidate(
            std::vector<tSigned32>& faceRemaps,
            std::vector<tSigned32>& edgeRemaps,
            std::vector<tSigned32>& vertRemaps
            )
    {
        tSigned32 topElementBeforeRemaps = TopElement(*_transformedMesh);
        _transformedMesh->consolidate(faceRemaps, edgeRemaps, vertRemaps);
        std::vector<tSigned32> elementRemaps(topElementBeforeRemaps + 1, -1);
        tSigned32 i;
        for(i = 0; i != SizeL(faceRemaps); ++i)
        {
            if(faceRemaps[i] != -1)
            {
                tSigned32 beforeRemap = ElementIndexForTypeAndSubIndex(bMeshElement::FACE, i);
                tSigned32 afterRemap = ElementIndexForTypeAndSubIndex(bMeshElement::FACE, faceRemaps[i]);
                elementRemaps[beforeRemap] = afterRemap;
            }
        }
        for(i = 0; i != SizeL(edgeRemaps); ++i)
        {
            if(edgeRemaps[i] != -1)
            {
                tSigned32 beforeRemap = ElementIndexForTypeAndSubIndex(bMeshElement::EDGE, i);
                tSigned32 afterRemap = ElementIndexForTypeAndSubIndex(bMeshElement::EDGE, edgeRemaps[i]);
                elementRemaps[beforeRemap] = afterRemap;
            }
        }
        for(i = 0; i != SizeL(vertRemaps); ++i)
        {
            if(vertRemaps[i] != -1)
            {
                tSigned32 beforeRemap = ElementIndexForTypeAndSubIndex(bMeshElement::VERTEX, i);
                tSigned32 afterRemap = ElementIndexForTypeAndSubIndex(bMeshElement::VERTEX, vertRemaps[i]);
                elementRemaps[beforeRemap] = afterRemap;
            }
        }
        _tree.remap(elementRemaps, TopElement(*_transformedMesh) + 1);
    }

    void
    consolidate()
    {
        std::vector<tSigned32> faceRemaps;
        std::vector<tSigned32> edgeRemaps;
        std::vector<tSigned32> vertRemaps;
        consolidate(faceRemaps, edgeRemaps, vertRemaps);
    }

    void splitEdge(edge e, tVertexData vertexData, tEdgeData edgeData);
    void splitFace(edge after1, edge after2, const tEdgeData& userdata12, const tEdgeData& userdata21);
    void
    splitFace(edge after1, edge after2)
    {
        tEdgeData userdata12 = *after1;
        tEdgeData userdata21 = *after2;
        splitFace(after1, after2, userdata12, userdata21);
    }

    vertex splitFaceWithVertex(face f,
                        tSigned32 numberOfSplits,
                        const tVertexData& vertexData, 
                        const tEdgeData* edgeData,
                        edge* after)
    {
        clearSourcesAndResults();
        addSource(f);
        typename tMesh::vertex v = (*_transformedMesh).splitFaceWithVertex(f, numberOfSplits, vertexData, edgeData, after);
        addResult(v);
        tSigned32 i;
        for(i = 0; i < numberOfSplits; i++)
        {
            addResult(after[i].prev());
            addResult(after[i].face());
        }
        for(i = 0; i < numberOfSplits; i++)
        {
            addResult(after[i].prev().twin());
        }

        std::vector<tPoint> spokes(numberOfSplits);
        for(i = 0; i < numberOfSplits; i++)
        {
            spokes[i] = GetVertexPoint(after[i]) - GetVertexPoint(v);
        }
        _tree.addOperation(new cSplitRadial(GetVertexPoint(v), spokes), _sources, _results);

        return v;
    }

    void flipEdge(edge e);
    face joinFaces(vertex v)
    {
        clearSourcesAndResults();

        std::vector<tPoint> spokes;
        tPoint vertexP = GetVertexPoint(v);

        addSource(v);

        edge e = tMesh::firstEdge(v);
        do
        {
            spokes.push_back(GetVertexPoint(e.next()) - vertexP);
            addSource(e);
            addSource(e.face());
        }
        while(e.toNextAroundVertex());

        e = tMesh::firstEdge(v);
        do
        {
            addSource(e.twin());
        }
        while(e.toNextAroundVertex());

        face f = (*_transformedMesh).joinFaces(v);

        addResult(f);

        _tree.addOperation(new cJoinRadial(vertexP, spokes), _sources, _results);

        return f;
    }
    face joinFaces(edge e)
    {
        clearSourcesAndResults();

        tLine l = GetEdgeLine(e);

        addSource(e.twin().face());
        addSource(e);
        addSource(e.face());

        addSource(e.twin());

        face f = (*_transformedMesh).joinFaces(e);

        addResult(f);

        _tree.addOperation(new cJoin3Way(l), _sources, _results);    

        return f;
    }
    void unsplitEdge(edge e);

    // on position translation, both sides of the broken edge translate to e
    void breakEdge(edge e);

    // on reverse translation, both edges translate to e1
    void connectEdges(edge e1, edge e2)
    {
        clearSourcesAndResults();
        addSource(e1);
        addSource(e2);
        (*_transformedMesh).connectEdges(e1, e2);
        addResult(e1);
        addResult(e2);
        _tree.addOperation(new cFixedTwoToOne(), _sources, _results);    
    }
};

// ------------------------------------ transform operations

template <class tMesh> void
cMeshWithTrackedTransform<tMesh>::splitEdge(edge e, tVertexData vertexData, tEdgeData edgeData)
{
    clearSourcesAndResults();

    addSource(e);
    if(e.twin().valid())
    {
        addSource(e.twin());
    }

    (*_transformedMesh).splitEdge(e, vertexData, edgeData);
    
    addResult(e);
    addResult(e.next().vertex());
    addResult(e.next());

    if(e.twin().valid())
    {
        addResult(e.twin().prev());
        addResult(e.twin());
    }
    
    tLine separatingLine = GetEdgeLine(e.next()).left90();
    _tree.addOperation(new cSplit3Way(separatingLine), _sources, _results);
}

template <class tMesh> void
cMeshWithTrackedTransform<tMesh>::splitFace(edge after1, edge after2, const tEdgeData& userdata12, const tEdgeData& userdata21)
{
//    assertD(FaceIsConvex(after1)); // not quite correct because it is ok for one of the corners to be inline
#ifdef ASSERTIONS_ON
    {
        tLine l(GetVertexPoint(after1), GetVertexPoint(after2));
        typename tMesh::edge e;
        e = after1.next();
        do
        {
            assertD(l.side(GetVertexPoint(e)) == SIDE_LEFT);
        }
        while(++e != after2);
        ++e;
        do
        {
            assertD(l.side(GetVertexPoint(e)) == SIDE_RIGHT);
        }
        while(++e != after1);
    }
#endif
    clearSourcesAndResults();

    addSource(after1.face());

    (*_transformedMesh).splitFace(after1, after2, userdata12, userdata21);

    addResult(after1.face());
    addResult(after2.prev());
    addResult(after2.face());

    addResult(after1.prev());

    _tree.addOperation(new cSplit3Way(GetEdgeLine(after2.prev())), _sources, _results);
}

template <class tMesh> void
cMeshWithTrackedTransform<tMesh>::flipEdge(edge e)
{
    clearSourcesAndResults();

    addSource(e.twin().face());
    addSource(e);
    addSource(e.face());

    addSource(e.twin());

    tLine lineBefore = GetEdgeLine(e);

    (*_transformedMesh).flipEdge(e);

    addResult(e.twin().face());
    addResult(e);
    addResult(e.face());

    addResult(e.twin());

    _tree.addOperation(new cFlip3Way(lineBefore, GetEdgeLine(e)), _sources, _results);
}

template <class tMesh> void
cMeshWithTrackedTransform<tMesh>::unsplitEdge(edge e)
{
    clearSourcesAndResults();

    tLine l = GetEdgeLine(e).reversed().right90();

    addSource(e);
    addSource(e.next().vertex());
    addSource(e.next());

    addSource(e.twin());
    addSource(e.twin().prev());

    (*_transformedMesh).unsplitEdge(e);

    addResult(e);

    addResult(e.twin());

    _tree.addOperation(new cJoin3Way(l), _sources, _results);    
}

// this operation is implemented this way
// so that translating across a cut portal always goes to the desired cut edge
template <class tMesh> void
cMeshWithTrackedTransform<tMesh>::breakEdge(edge e)
{
    clearSourcesAndResults();

    edge twin = e.twin();

    addSource(e);
    addSource(twin);

    (*_transformedMesh).breakEdge(e);

    addResult(e);
    addResult(twin);

    _tree.addOperation(new cFixedTwoToOne(), _sources, _results);    
}

#endif
