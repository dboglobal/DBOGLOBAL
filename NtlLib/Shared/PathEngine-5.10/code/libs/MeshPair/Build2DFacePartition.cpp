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
#include "Build2DFacePartition.h"
#include "EdgesInto2DFace.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "common/TemplateMesh/External.h"
#include "common/TemplateMesh/ElementIndexing.h"
#include "common/Containers/SwapOut.h"

static tFace_3D
Face3DForNoEdgesCase(tFace f2D, tMesh_3D& mesh3D)
{
    tEdge e2D = tMesh::beginEdge(f2D);
    tSigned32 startElementIndex = e2D.vertex()->getElementOn3DIndex();
    cMeshElement<tMesh_3D> startEl = ElementAtIndex(mesh3D, startElementIndex);
    if(startEl.isFace())
    {
        return startEl.asFace();
    }
    cMeshElement<tMesh_3D> nextEl;
    if(VertexIsExternal(e2D))
    {
        tSigned32 spinCount = SpinCountAroundVertex(e2D);
        nextEl = TraverseFromVertex_Elements<tMesh_3D>(startEl.asVertex(), GetVertexPoint(e2D.next()), spinCount);
    }
    else
    {
        nextEl = NextElementAlongLine(startEl, GetEdgeLine(e2D));
    }
    if(nextEl.isFace())
    {
        return nextEl.asFace();
    }
    if(startEl.isVertex())
    {
        return nextEl.asEdgeFromVertex(startEl).face();
    }
    return startEl.asEdgeFromVertex(nextEl).twin().face();
}

static tFace_3D
Face3DForFace2D(tFace f2D, tMesh_3D& addTo)
{
    std::vector<tMesh_3D::tVertexData> verticesAround;
    tEdge e2D = tMesh::beginEdge(f2D);
    do
    {
        verticesAround.push_back(tMesh_3D::tVertexData(GetVertexPoint(e2D)));
    }
    while(e2D.toNextInFace());
    std::vector<tMesh_3D::tEdgeData> edgesAround(verticesAround.size());
    return addTo.addFace(tMesh_3D::tFaceData(), SizeL(edgesAround), &edgesAround.front(), &verticesAround.front());
}

static tMesh_TranslationBSP::tEdgeData
EdgeDataFor(
        const std::vector<tEdge_3D>& boundaryEdges,
        tMesh_3D& mesh3D,
        const tSigned32 edgeIndex
        )
{
    if(edgeIndex >= 0)
    {
        return boundaryEdges[edgeIndex];
    }
    return mesh3D.edgeAt(-1 - edgeIndex);
}

static void
NextAround(
        const std::vector<tEdge_3D>& boundaryEdges,
        const std::vector<tEdge_3D>& edgesBefore,
        const std::vector<tEdge_3D>& edgesAfter,
        const std::vector<tSigned32>& edgeBeforeLookup,
        tMesh_3D& mesh3D,
        tSigned32& edgeIndex
        )
{
    if(edgeIndex >= 0)
    {
  // boundary edge
        if(!edgesAfter[edgeIndex].valid())
        {
          // corner of a 2d face is inside a 3d mesh face
          // follows this corner around the boundary
            ++edgeIndex;
            if(edgeIndex == SizeL(boundaryEdges))
            {
                edgeIndex = 0;
            }
            return;
        }
      // follow an edge into the interior
        edgeIndex = -1 - edgesAfter[edgeIndex].index();
        return;
    }

  // interior edge

    tSigned32 e3DIndex = -1 - edgeIndex;
    tSigned32 edgeBeforeIndex = edgeBeforeLookup[e3DIndex];
    assertD(edgeBeforeIndex >= 0);
    if(edgeBeforeIndex >= SizeL(edgesBefore) || !edgesBefore[edgeBeforeIndex].valid() || edgesBefore[edgeBeforeIndex].index() != e3DIndex)
    {
      // follow around interior corner to another interior edge
        tEdge_3D e = mesh3D.edgeAt(e3DIndex);
        ++e;
        edgeIndex = -1 - e.index();
        return;
    }
    // traverse onto boundary
    edgeIndex = edgeBeforeIndex;
}

void
Build2DFacePartition(
        tFace f2D,
        tMesh_3D& mesh3D,
        std::vector<tSigned32>& buffer,
        tMesh_3D& tempMesh,
        tMesh_TranslationBSP& result
        )
{
    assertD(tempMesh.empty());
    assertD(result.empty());
    assertD(buffer.size() == mesh3D.edges());
    assertD(!mesh3D.hasDeadEntries());
    std::vector<tSigned32>& edgeBeforeLookup = buffer;

    tFace_3D f3D = Face3DForFace2D(f2D, tempMesh);
    AddExternalFace(tempMesh, tMesh_3D::beginEdge(f3D));

// generate the set of edges into the face
// store this in vectors organised per boundary edge

    // these edges are in mesh3D
    std::vector<tEdge_3D> edgesBefore;
    std::vector<tEdge_3D> edgesAfter;

    // these edges are in tempMesh
    std::vector<tEdge_3D> boundaryEdges;

    {
        cMeshElement<tMesh_3D> startElement;
        {
            tSigned32 startElementIndex = tMesh::beginEdge(f2D).vertex()->getElementOn3DIndex();
            startElement = ElementAtIndex(mesh3D, startElementIndex);
        }
        cEdgesInto2DFace edgesIn(f2D, startElement);
        tEdge_3D e3D = tMesh_3D::beginEdge(f3D);
        edgesBefore.push_back(tMesh_3D::endEdge());
        cEdgesInto2DFace::eEvent event;
        do
        {
            boundaryEdges.push_back(e3D);
            tEdge_3D firstEdgeIn, lastEdgeIn;
            event = edgesIn.nextEvent(firstEdgeIn, lastEdgeIn);
            if(event & cEdgesInto2DFace::EDGES)
            {
                edgesAfter.push_back(firstEdgeIn);
                edgesBefore.push_back(lastEdgeIn.twin());
            }
            else
            {
                edgesAfter.push_back(tMesh_3D::endEdge());
                edgesBefore.push_back(tMesh_3D::endEdge());
            }
            if(event & cEdgesInto2DFace::CORNER)
            {
                ++e3D;
            }
        }
        while((event & cEdgesInto2DFace::COMPLETED) == 0);
    }

    edgesBefore.front() = edgesBefore.back();
    edgesBefore.pop_back();

// post edge indices for edgesBefore into edgeBeforeLookup
    
    tSigned32 i;
    for(i = 0; i != SizeL(edgesBefore); ++i)
    {
        if(edgesBefore[i].valid())
        {
            edgeBeforeLookup[edgesBefore[i].index()] = i;
        }
    }

// generate the faces inside the boundary with a flood fill operation

    tEdge_TranslationBSP edgeToBuildExternalFaceBehind;

    std::vector<tEdge_TranslationBSP> open;

  // (first face is generated outside the fill loop)

    std::vector<tMesh_TranslationBSP::tEdgeData> edgeData;
    tSigned32 edgeIndex = 0;
    do
    {
        edgeData.push_back(EdgeDataFor(boundaryEdges, mesh3D, edgeIndex));
        NextAround(boundaryEdges, edgesBefore, edgesAfter, edgeBeforeLookup, mesh3D, edgeIndex);
    }
    while(edgeIndex != 0);

    std::vector<tMesh_TranslationBSP::tVertexData> vertexData(edgeData.size());
    tFace_TranslationBSP lastAdded = result.addFace(tMesh_TranslationBSP::tFaceData(), SizeL(edgeData), &edgeData.front(), &vertexData.front());
    lastAdded->_subSet = 0;

    {
        tEdge_TranslationBSP e = tMesh_TranslationBSP::beginEdge(lastAdded);
        edgeToBuildExternalFaceBehind = e;
        do
        {
            if(mesh3D.ownsEdge(e->_e3D))
            {
                open.push_back(e);
            }
        }
        while(e.toNextInFace());
    }

    if(open.empty())
    {
        lastAdded->_originalIndex = Face3DForNoEdgesCase(f2D, mesh3D).index();
    }
    else
    {
        lastAdded->_originalIndex = open.back()->_e3D.face().index();
    }

    while(!open.empty())
    {
        tEdge_TranslationBSP ePopped = open.back();
        open.pop_back();

        std::vector<tMesh_TranslationBSP::tEdgeData> edgeData;
        tSigned32 startEdgeIndex = -1 - ePopped->_e3D.twin().index();
        tSigned32 edgeIndex = startEdgeIndex;
        do
        {
            edgeData.push_back(EdgeDataFor(boundaryEdges, mesh3D, edgeIndex));
            NextAround(boundaryEdges, edgesBefore, edgesAfter, edgeBeforeLookup, mesh3D, edgeIndex);
        }
        while(edgeIndex != startEdgeIndex);

        std::vector<tMesh_TranslationBSP::tVertexData> vertexData(edgeData.size());
        tFace_TranslationBSP added = result.addFaceAtEnd(tMesh_TranslationBSP::tFaceData(), SizeL(edgeData), &edgeData.front(), &vertexData.front());
        added->_subSet = 0;

        {
            tEdge_TranslationBSP e = tMesh_TranslationBSP::beginEdge(added);
            added->_originalIndex = e->_e3D.face().index();
            result.connectEdges(e, ePopped);
            ++e;
            do
            {
                if(mesh3D.ownsEdge(e->_e3D))
                {
                    tSigned32 i;
                    for(i = 0; i != SizeL(open); ++i)
                    {
                        if(open[i]->_e3D == e->_e3D.twin())
                        {
                            break;
                        }
                    }
                    if(i == SizeL(open))
                    {
                        open.push_back(e);
                    }
                    else
                    {
                        result.connectEdges(e, open[i]);
                        SwapOut(open, i);
                    }
                }
            }
            while(e.toNextInFace());
        }
    }

// add an external face

    {
        std::vector<tMesh_TranslationBSP::tEdgeData> edgeData;
        std::vector<tEdge_TranslationBSP> insideEdges;

        tEdge_TranslationBSP e = edgeToBuildExternalFaceBehind;
        do
        {
            assertD(!e.twin().valid());
            insideEdges.push_back(e);
            assertD(!mesh3D.ownsEdge(e->_e3D));
            edgeData.push_back(e->_e3D.twin());
            e = e.prevExternal();
        }
        while(e != edgeToBuildExternalFaceBehind);

        std::vector<tMesh_TranslationBSP::tVertexData> vertexData(edgeData.size());
        tFace_TranslationBSP added = result.addFaceAtEnd(tMesh_TranslationBSP::tFaceData(), SizeL(edgeData), &edgeData.front(),&vertexData.front());
        added->_subSet = -1;

        e = tMesh_TranslationBSP::beginEdge(added);
        tSigned32 i = 0;
        do
        {
            result.connectEdges(insideEdges[i], e);
            i++;
        }
        while(e.toNextInFace());
    }
}
