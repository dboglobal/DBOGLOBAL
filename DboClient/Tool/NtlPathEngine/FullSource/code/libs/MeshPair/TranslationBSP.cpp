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
#include "interface/TranslationBSP.h"
#include "libs/MeshPair/interface/tMesh_TranslationBSP.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "common/Containers/SwapOut.h"

typedef tMesh_TranslationBSP tMesh;
typedef tFace_TranslationBSP tFace;
typedef tEdge_TranslationBSP tEdge;
typedef tVertex_TranslationBSP tVertex;

cNullGeometryOverlay_VertexIsIntersection<tMesh> nullOverlay;

static void
DoSwapOut(std::vector<tEdge>& potentialSplitters, tEdge toRemove)
{
    tSigned32 i = toRemove->_splitterIndex;
    assertD(potentialSplitters[i] == toRemove);
    if(SwapOut(potentialSplitters, i))
    {
      // update index for edge swapped down
        potentialSplitters[i]->_splitterIndex = i;
    }
    toRemove->_splitterIndex = -1; // no longer a potential splitter
}

static void
MovePotentialSplitter(std::vector<tEdge>& sourceVector, tEdge toMove, std::vector<tEdge>& targetVector)
{
    tSigned32 i = toMove->_splitterIndex;
    assertD(sourceVector[i] == toMove);
    if(SwapOut(sourceVector, i))
    {
      // update index for edge swapped down
        sourceVector[i]->_splitterIndex = i;
    }
    toMove->_splitterIndex = SizeL(targetVector);
    targetVector.push_back(toMove);
}

static bool
SplitterVectorIsValid(const std::vector<tEdge>& splitters)
{
    tSigned32 i;
    for(i = 0; i != SizeL(splitters); ++i)
    {
        if(!splitters[i].valid())
        {
            return false;
        }
        if(splitters[i]->_splitterIndex != i)
        {
            return false;
        }
    }
    return true;
}

static void
InitPotentialSplitters(tMesh& mesh, std::vector<tEdge>& potentialSplitters)
{
    tEdge e = mesh.beginEdge();
    do
    {
        assertD(e->_splitterIndex == -1);
        if(e.face()->_subSet == 0 && e.twin().face()->_subSet == 0 && SideOfEdge_SI(e) == SIDE_LEFT)
        {
            e->_splitterIndex = SizeL(potentialSplitters);
            potentialSplitters.push_back(e);
        }
    }
    while(e.toNextInMesh());
}

static void
PropagateSplit(
        tMesh& mesh,
        tVertex v,
        tEdge edgeForDirection,
        tSigned32 subSet,
        std::vector<tEdge>& potentialSplitters, bool checkTwin
        )
{
    tLine traverseLine = GetEdgeLine(edgeForDirection);
    while(1)
    {
        cMeshElement<tMesh> el(v);
        cMeshElement<tMesh> nextEl;
        nextEl = NextElementAlongLine(el, traverseLine, nullOverlay);
        if(nextEl.isFace())
        {
            if(nextEl.asFace()->_subSet != subSet)
            {
              // reached subset boundary
                return;
            }
            cMeshElement<tMesh> el3 = NextElementAlongLine(nextEl, traverseLine, nullOverlay);
            tVertex v2;
            if(el3.isVertex())
            {
                v2 = el3.asVertex();
            }
            else
            {
              // split this edge first
                tEdge e = el3.asEdge();
                mesh.splitEdge(e, tMesh::tVertexData(), e->_e3D, e->_e3D.twin());
                if(e->_splitterIndex != -1)
                {
                    e.next()->_splitterIndex = SizeL(potentialSplitters);
                    potentialSplitters.push_back(e.next());
                }
                else if(e.next().twin()->_splitterIndex != -1)
                {
                    e.twin()->_splitterIndex = SizeL(potentialSplitters);
                    potentialSplitters.push_back(e.twin());
                }
                v2 = e.next().vertex();
            }
          // go ahead and split the face
            
            tEdge after1 = EdgeInFaceForVertex<tMesh>(nextEl.asFace(), v);
            tEdge after2 = EdgeInFaceForVertex<tMesh>(nextEl.asFace(), v2);
            mesh.splitFace(after1, after2, tMesh::tEdgeData(edgeForDirection->_e3D), tMesh::tEdgeData(edgeForDirection.twin()->_e3D));

            v = v2;
        }
        else
        {
            tEdge e = nextEl.asEdgeFromVertex(v);
            if(e.face()->_subSet != subSet)
            {
              // reached subset boundary
                return;
            }
            assertD(e.twin().face()->_subSet == subSet);
            if(checkTwin)
            {
                if(e.twin()->_splitterIndex != -1)
                {
                    DoSwapOut(potentialSplitters, e.twin());
                }
            }
            else
            {
                if(e->_splitterIndex != -1)
                {
                    DoSwapOut(potentialSplitters, e);
                }
            }
            v = e.next().vertex();
        }
    }
}

void
cTranslationBSP::performSplit(tMesh& mesh, std::vector<tEdge>& potentialSplitters, tSigned32 splitterToUse, tSigned32& nextSubSetIndex)
{
    tEdge splitter = potentialSplitters[splitterToUse];
    DoSwapOut(potentialSplitters, splitter);

// traverse out from splitter in both directions until boundary of current subset is reached

    tSigned32 subSet = splitter.face()->_subSet;
    assertD(subSet >= 0);
    assertD(subSet == splitter.twin().face()->_subSet);

    assertF(SplitterVectorIsValid(potentialSplitters));

    PropagateSplit(mesh, splitter.vertex(), splitter.twin(), subSet, potentialSplitters, true);
    PropagateSplit(mesh, splitter.twin().vertex(), splitter, subSet, potentialSplitters, false);

    assertF(SplitterVectorIsValid(potentialSplitters));

    std::vector<tEdge> potentialSplittersInFront;
    std::vector<tFace> open(1, splitter.face());
    do
    {
        tFace f = open.back();
        open.pop_back();
        tEdge e = tMesh::beginEdge(f);
        do
        {
            if(e->_splitterIndex != -1)
            {
                if(e.twin().face()->_subSet != subSet)
                {
                    // the face across this edge has already been processed by the fill
                    assertD(e.twin().face()->_subSet == nextSubSetIndex);
                    continue;
                }
                MovePotentialSplitter(potentialSplitters, e, potentialSplittersInFront);
                open.push_back(e.twin().face());
            }
            else if(e.twin()->_splitterIndex != -1)
            {
                if(e.twin().face()->_subSet != subSet)
                {
                    // the face across this edge has already been processed by the fill
                    assertD(e.twin().face()->_subSet == nextSubSetIndex);
                    continue;
                }
                MovePotentialSplitter(potentialSplitters, e.twin(), potentialSplittersInFront);
                open.push_back(e.twin().face());
            }
        }
        while(e.toNextInFace());
        f->_subSet = nextSubSetIndex;
    }
    while(!open.empty());

    assertF(SplitterVectorIsValid(potentialSplitters));
    assertF(SplitterVectorIsValid(potentialSplittersInFront));

    nextSubSetIndex++;

    tSigned32 thisSubTreeIndex = SizeL(_splitters);
    _splitters.push_back(splitter->_e3D);
    _subTreeIndices.push_back(0);
    _subTreeIndices.push_back(0);

    if(potentialSplitters.empty())
    {
      // leaf node
        _subTreeIndices[thisSubTreeIndex * 2 + 0] = -1 - splitter.twin().face()->_originalIndex;
    }
    else
    {
      // split recursively
        _subTreeIndices[thisSubTreeIndex * 2 + 0] = SizeL(_splitters);
        performSplit(mesh, potentialSplitters, nextSubSetIndex);
    }

    if(potentialSplittersInFront.empty())
    {
      // leaf node
        _subTreeIndices[thisSubTreeIndex * 2 + 1] = -1 - splitter.face()->_originalIndex;
    }
    else
    {
      // split recursively
        _subTreeIndices[thisSubTreeIndex * 2 + 1] = SizeL(_splitters);
        performSplit(mesh, potentialSplittersInFront, nextSubSetIndex);
    }
}

void
cTranslationBSP::performSplit(tMesh& mesh, std::vector<tEdge>& potentialSplitters, tSigned32& nextSubSetIndex)
{
    //.... randomise choice of splitter
    performSplit(mesh, potentialSplitters, 0, nextSubSetIndex);
}

cTranslationBSP::cTranslationBSP(tMesh& mesh)
{
    std::vector<tEdge> potentialSplitters;
    InitPotentialSplitters(mesh, potentialSplitters);

    if(potentialSplitters.empty())
    {
    // handle special case where no partitioning is required

        assertD(mesh.faces() == 2);
        tFace f = mesh.beginFace();
        if(f->_subSet == -1)
        {
            ++f;
        }
        assertD(f->_subSet == 0);
        _leafIndexForUnpartitionedCase = f->_originalIndex;
        return;
    }

    _leafIndexForUnpartitionedCase = -1;

    tSigned32 nextSubSetIndex = 1;
    performSplit(mesh, potentialSplitters, nextSubSetIndex);
}
