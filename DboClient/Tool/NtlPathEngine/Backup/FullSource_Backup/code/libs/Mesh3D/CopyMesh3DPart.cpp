//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CopyMesh3DPart.h"
#include "common/TemplateMesh/Helper.h"
#include "common/Containers/CollapsedVectorVector.h"
#include "common/STL/algorithm.h"

static tEdge_3D
FindEdgeInFaceWithVertex(tFace_3D f, const tMesh_3D::tVertexData& vd)
{
    tEdge_3D e = tMesh_3D::beginEdge(f);
    while(*e.vertex() != vd)
    {
        bool iterate = e.toNextInFace();
        assertD(iterate); // fails if no edge found with the given vertex data
    }
    return e;
}

static void
CopyFaceAndConnect(
        tMesh_3D& copyFrom, tSigned32 index, tMesh_3D& copyTo,
        const std::vector<tSigned32>& faceRemap
        )
{
    tFace_3D f = copyFrom.faceAt(index);
    tSigned32 edges = CountEdgesAround(tMesh_3D::beginEdge(f));

    // optimise - move allocations outside loop in calling code
    std::vector<tMesh_3D::tEdgeData> edgeData(edges);
    std::vector<tMesh_3D::tVertexData> vertexData(edges);

    edgeData.resize(0);
    vertexData.resize(0);
    tEdge_3D e = tMesh_3D::beginEdge(f);
    do
    {
        edgeData.push_back(*e);
        vertexData.push_back(*e.vertex());
    }
    while(e.toNextInFace());
    tFace_3D copiedF = copyTo.addFaceAtEnd(
            *f,
            edges,
            &edgeData.front(),
            &vertexData.front()
            );

    e = tMesh_3D::beginEdge(f);
    tEdge_3D copiedE = tMesh_3D::beginEdge(copiedF);
    do
    {
        tFace_3D twinF = e.twin().face();
        if(!twinF->isExternal())
        {
            tSigned32 twinIndex = twinF.index();
            tSigned32 remappedTwinIndex = faceRemap[twinIndex];
            if(remappedTwinIndex != -1)
            {
                tFace_3D connectToF = copyTo.faceAt(remappedTwinIndex);
                tEdge_3D connectToE = FindEdgeInFaceWithVertex(connectToF, *copiedE.next().vertex());
                copyTo.connectEdges(copiedE, connectToE);
            }
        }
        copiedE.toNextInFace();
    }
    while(e.toNextInFace());

}

void
CopyMesh3DPart(
        tMesh_3D& mesh3D,
        const cCollapsedVectorVector<tSigned32>& subsetsLookup,
        tSigned32 subsetIndex,
        tMesh_3D& mesh3DResult,
        std::vector<tSigned32>& faceRemap
        )
{
    assertD(mesh3DResult.empty());
//    assertD(SizeL(faceRemap) == mesh3D.faces()); // only needs to be big enough to fit internal faces
    assertD(SizeL(faceRemap) == subsetsLookup.size());
    tSigned32 i;

    std::fill(faceRemap.begin(), faceRemap.end(), -1);

// loop over all faces
// for faces in subset:
//  enter into remap,
//  copy to part
//  and connect edges where the twins face has already been copied

    for(i = 0; i != subsetsLookup.size(); ++i)
    {
        for(tSigned32 j = 0; j != subsetsLookup.subVectorSize(i); j += 2)
        {
            if(subsetsLookup[i][j] == subsetIndex)
            {
                faceRemap[i] = mesh3DResult.faces();
                assertD(faceRemap[i] == subsetsLookup[i][j + 1]);
                CopyFaceAndConnect(mesh3D, i, mesh3DResult, faceRemap);
                break;
            }
        }
    }
}

void
AddFaceToCopiedMesh3DPart(
        tMesh_3D& mesh3D,
        tSigned32 fIndex,
        tMesh_3D& mesh3DResult,
        std::vector<tSigned32>& faceRemap
        )
{
    assertD(faceRemap[fIndex] == -1);
    faceRemap[fIndex] = mesh3DResult.faces();
    CopyFaceAndConnect(mesh3D, fIndex, mesh3DResult, faceRemap);
}

cMeshElement<tMesh_3D>
TranslateToCopiedMesh3DPart(
        const cMeshElement<tMesh_3D>& toTranslate,
        const std::vector<tSigned32>& faceRemap,
        tMesh_3D& originalMesh,
        tMesh_3D& part
        )
{
    switch(toTranslate.type())
    {
    default:
        invalid();
    case bMeshElement::FACE:
        return part.faceAt(faceRemap[toTranslate.asFace().index()]);
    case bMeshElement::EDGE:
        {
            tEdge_3D e = toTranslate.asEdge();
            tSigned32 remappedFIndex = faceRemap[e.face().index()];
            if(remappedFIndex == -1)
            {
                e = e.twin();
                remappedFIndex = faceRemap[e.face().index()];
                assertD(remappedFIndex != -1); // element is not on copied portion of mesh
            }
            tFace_3D remappedF = part.faceAt(remappedFIndex);
            return FindEdgeInFaceWithVertex(remappedF, *e.vertex());
        }
    case bMeshElement::VERTEX:
        {
            tVertex_3D v = toTranslate.asVertex();
            tEdge_3D e = tMesh_3D::firstEdge(v);
            tSigned32 remappedFIndex;
          // iterate through faces around the vertex, to find one that got copied
            while(1)
            {
                remappedFIndex = faceRemap[e.face().index()];
                if(remappedFIndex != -1)
                {
                    break;
                }
                bool iterate = e.toNextAroundVertex();
                assertD(iterate); // vertex is not on copied portion of mesh
            }
            tFace_3D remappedF = part.faceAt(remappedFIndex);
            return FindEdgeInFaceWithVertex(remappedF, *e.vertex()).vertex();
        }
    }
}
