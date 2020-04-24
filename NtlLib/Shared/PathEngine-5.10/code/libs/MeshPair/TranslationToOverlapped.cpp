//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/TranslationToOverlapped.h"
#include "Build2DFacePartition.h"
#include "FacesOverlapped.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/TemplateMesh/ElementIndexing.h"
#include "common/STL_Helpers.h"

template <class tEdge, class tEdge2> bool
EdgeSeparates(tEdge e, tEdge2 e2)
{
    tLine l = GetEdgeLine(e);
    do
    {
        if(l.side(GetVertexPoint(e2)) == SIDE_RIGHT)
        {
            return false;
        }
    }
    while(e2.toNextInFace());
    return true;
}

static bool
CheckFacesOverlap(tFace f1, tFace_3D f2)
{
    {
        tEdge e = tMesh::beginEdge(f1);
        do
        {
            if(EdgeSeparates(e, tMesh_3D::beginEdge(f2)))
            {
                return false;
            }
        }
        while(e.toNextInFace());
    }
    {
        tEdge_3D e = tMesh_3D::beginEdge(f2);
        do
        {
            if(EdgeSeparates(e, tMesh::beginEdge(f1)))
            {
                return false;
            }
        }
        while(e.toNextInFace());
    }
    return true;
}

void
cTranslationToOverlapped::processFace(tFace f, cFacesOverlapped& facesOverlapped)
{
// follow 2d edge lines through 3d mesh

    facesOverlapped.init();

    tEdge e = tMesh::beginEdge(f);
    cMeshElement<tMesh_3D> el;
    {
        tSigned32 elementIndex = e.vertex()->getElementOn3DIndex();
        el = ElementAtIndex(*_mesh3D, elementIndex);
        if(el.isFace())
        {
          // note that elementTraversed may be called also at end for this element,
          // but this doesn't matter for faces
            facesOverlapped.elementTraversed(el);
        }
    }
    do
    {
        tLine traverseLine = GetEdgeLine(e);

        if(VertexIsExternal(e))
        {
            tSigned32 spinCount = SpinCountAroundVertex(e);
            el = TraverseFromVertex_Elements<tMesh_3D>(el.asVertex(), traverseLine.end(), spinCount);
            facesOverlapped.elementTraversed(el);
        }

        while(!ElementContainsPoint(el, traverseLine.end()))
        {
            el = NextElementAlongLine(el, traverseLine);
            facesOverlapped.elementTraversed(el);
        }
    }
    while(e.toNextInFace());

    facesOverlapped.closeAndTakeResult(_facesOverlappedBy2D[f.index()]);
    assertD(!_facesOverlappedBy2D[f.index()].empty());

#ifdef ASSERTIONS_ON
    {
        tSigned32 i;
        for(i = 0; i != SizeL(_facesOverlappedBy2D[f.index()]); ++i)
        {
            tFace_3D f3D = _mesh3D->faceAt(_facesOverlappedBy2D[f.index()][i]);
            assertD(CheckFacesOverlap(f, f3D));
        }
    }
#endif
}

cTranslationToOverlapped::cTranslationToOverlapped(
            tMesh_3D& mesh3D, 
            tMesh& mesh2D,
            bool useBSPs
            ) :
    _mesh3D(&mesh3D),
    _mesh2D(&mesh2D),
//..... is anything actually using translation of external faces?
//    _facesOverlappedBy2D(index2D.internalFaces()),
//    _facesOverlappedBy3D(index3D.internalFaces())
    _facesOverlappedBy2D(mesh2D.faces()),
    _facesOverlappedBy3D(mesh3D.faces())
{
    tSigned32 internalFacesOn2D = CountInternalFaces(*_mesh2D);
    assertD(!mesh3D.hasDeadEntries());
    assertD(!mesh2D.hasDeadEntries());
    cFacesOverlapped facesOverlapped(mesh3D.faces());

// generate table of faces overlapped by mesh

    tFace f = mesh2D.beginFace();
    do
    {
        if(f->isExternal())
        {
            tSigned32 vertex3D = tMesh::beginEdge(f).vertex()->getTargetVertexIndex();
            tEdge_3D e = tMesh_3D::firstEdge(_mesh3D->vertAt(vertex3D));
            e = ExternalEdgeForVertex(e);
            _facesOverlappedBy2D[f.index()].push_back(e.face().index());
        }
        else
        {
            processFace(f, facesOverlapped);
        }
    }
    while((++f).valid());

// reverse this table for mesh_3d

    tSigned32 i;
    for(i = 0; i < SizeL(_facesOverlappedBy2D); i++)
    {
        tSigned32 j;
        for(j = 0; j < SizeL(_facesOverlappedBy2D[i]); j++)
        {
            tSigned32 overlapped = _facesOverlappedBy2D[i][j];
            assertD(overlapped >= 0 && overlapped < SizeL(_facesOverlappedBy3D));
            _facesOverlappedBy3D[overlapped].push_back(i);
        }
    }

// build bsps if requested

    if(!useBSPs)
    {
        return;
    }

    _bsps.resize(internalFacesOn2D);
    std::vector<tSigned32> buffer(_mesh3D->edges(), 0);
    for(i = 0; i != internalFacesOn2D; ++i)
    {
        tMesh_3D tempMesh;
        tMesh_TranslationBSP partition;
        Build2DFacePartition(_mesh2D->faceAt(i), *_mesh3D, buffer, tempMesh, partition);
        assertD(partition.faces() - 1 == numberOfFacesOverlapped(_mesh2D->faceAt(i)));
        _bsps[i] = new cTranslationBSP(partition);
    }
}

cTranslationToOverlapped::~cTranslationToOverlapped()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_bsps); ++i)
    {
        delete _bsps[i];
    }
}

template <class tMesh> typename tMesh::edge
FindMatchingEdge(typename tMesh::face f, const tLine& edgeLine, const tLine& entryLine)
{
    typename tMesh::edge e = tMesh::beginEdge(f);
    do
    {
        if(edgeLine.side(GetVertexPoint(e)) != SIDE_CENTRE)
        {
            continue;
        }
        if(edgeLine.axis().direction(GetEdgeAxis(e)) != DIRECTION_FORWARD)
        {
            continue;
        }
        if(SideOfLine_SI(entryLine, e.vertex()) != SIDE_RIGHT || SideOfLine_SI(entryLine, e.next().vertex()) != SIDE_LEFT)
        {
            continue;
        }
        return e;
    }
    while(e.toNextInFace());
    return tMesh::endEdge();
}

tEdge
cTranslationToOverlapped::translateEntry(tEdge_3D entryEdge, const tLine& entryLine) const
{
    assertD(entryEdge.twin().face()->isExternal());
    tSigned32 fIndex = entryEdge.face().index();
    tSigned32 i;
    for(i = 0; i != _facesOverlappedBy3D[fIndex].size(); ++i)
    {
        tFace f2D = _mesh2D->faceAt(_facesOverlappedBy3D[fIndex][i]);
        tEdge result = FindMatchingEdge<tMesh>(f2D, GetEdgeLine(entryEdge), entryLine);
        if(result.valid())
        {
            assertD(result.twin().face()->isExternal());
            return result;
        }
    }
    invalid();
    return tEdge();
}
tEdge_3D
cTranslationToOverlapped::translateEntry(tEdge entryEdge, const tLine& entryLine) const
{
    assertD(entryEdge.twin().face()->isExternal());
    tSigned32 fIndex = entryEdge.face().index();
    tSigned32 i;
    for(i = 0; i != _facesOverlappedBy2D[fIndex].size(); ++i)
    {
        tFace_3D f3D = _mesh3D->faceAt(_facesOverlappedBy2D[fIndex][i]);
        tEdge_3D result = FindMatchingEdge<tMesh_3D>(f3D, GetEdgeLine(entryEdge), entryLine);
        if(result.valid())
        {
            assertD(result.twin().face()->isExternal());
            return result;
        }
    }
    invalid();
    return tEdge_3D();
}

tSigned32
cTranslationToOverlapped::numberOfFacesOverlapped(tFace f) const
{
    return SizeL(_facesOverlappedBy2D[f.index()]);
}
tSigned32
cTranslationToOverlapped::numberOfFacesOverlapped(tSigned32 f2DIndex) const
{
    return SizeL(_facesOverlappedBy2D[f2DIndex]);
}
tSigned32
cTranslationToOverlapped::iterateInSetOfFacesOverlapped(tSigned32 f2DIndex, tSigned32 i) const
{
    return _facesOverlappedBy2D[f2DIndex][i];
}
