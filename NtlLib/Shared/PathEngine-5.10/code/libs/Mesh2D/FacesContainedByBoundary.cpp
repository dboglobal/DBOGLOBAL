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
#include "FacesContainedByBoundary.h"
#include "libs/Mesh2D/interface/tMesh.h"

static tSigned32
GetIndexForEdgeEncountered(
        std::vector<tEdge>& boundary,
        tEdge e
        )
{
    tSigned32 i;
    for(i = 0; i != SizeL(boundary); ++i)
    {
        if(boundary[i] == e)
        {
            return i;
        }
    }
    invalid();
    return 0;
}

void
cFacesContainedByBoundary::processInsideEdges(
        tEdge eBefore,
        tEdge eAfter,
        std::vector<tEdge>& boundary
        )
{
    tEdge e = eBefore.next();
    assertD(e.vertex() == eAfter.vertex());
    while(e != eAfter)
    {
        edgeCrossed(boundary, e);
        _result.push_back(e.face());
        e = e.twin().next();
    }
}

void
cFacesContainedByBoundary::processInsideEdges(
        tEdge boundaryEdge1, tEdge boundaryEdge2,
        tEdge& firstE, tEdge& previousE,
        std::vector<tEdge>& boundary
        )
{
    tEdge e = boundaryEdge1.twin().prev();
    while(e != boundaryEdge2)
    {
        if(!firstE.valid())
        {
            firstE = e.twin();
        }
        else
        {
            if(previousE == e)
            {
            // special case for situation where a loop contains a single edge
                firstE.clear();
                previousE.clear();
                return;
            }
            else
            {
                processInsideEdges(previousE, e.twin(), boundary);
            }
        }
        previousE = e.twin();
        --e;
    }
}

void
cFacesContainedByBoundary::processLoop(const std::vector<tEdge>& boundary, tSigned32 beginI)
{
    std::vector<tEdge> insideBoundary;
    assertD(SizeL(boundary) >= beginI + 3);
    tEdge firstE, previousE;
    firstE.clear();
    previousE.clear();
    tSigned32 i;
    for(i = beginI; i + 1 != boundary.size(); ++i)
    {
        processInsideEdges(boundary[i], boundary[i + 1], firstE, previousE, insideBoundary);
        assertD(_edgeEncountered[boundary[i].index()] == true);
        _edgeEncountered.setBit(boundary[i].index(),  false);
    }
    processInsideEdges(boundary[i], boundary[beginI], firstE, previousE, insideBoundary);
    assertD(_edgeEncountered[boundary[i].index()] == true);
    _edgeEncountered.setBit(boundary[i].index(), false);

    assertD(previousE.valid() == firstE.valid());
    if(firstE.valid() && insideBoundary.empty())
    {
      // need to explicitly add the contained face in this case
        _result.push_back(firstE.face());
    }
    if(previousE.valid())
    {
        processInsideEdges(previousE, firstE, insideBoundary);
    }
    if(!insideBoundary.empty())
    {
        processLoop(insideBoundary, 0);
    }
}

void
cFacesContainedByBoundary::edgeCrossed(std::vector<tEdge>& boundary, tEdge e)
{
    assertD(!_edgeEncountered[e.index()]); // assumes boundary shape is convex

    if(!boundary.empty() && e == boundary.back().twin())
    {
    // crossed back immediately over an edge
        assertD(_edgeEncountered[e.twin().index()]);
        _edgeEncountered.setBit(e.twin().index(), false);
        boundary.pop_back();
        return;
    }

    if(_edgeEncountered[e.twin().index()])
    {
    // crossed back over an edge after looping around other edges
        tSigned32 beginI = GetIndexForEdgeEncountered(boundary, e.twin());
        processLoop(boundary, beginI + 1);
        _edgeEncountered.setBit(e.twin().index(), false);
        boundary.resize(beginI);
        return;
    }

    _edgeEncountered.setBit(e.index(), true);
    boundary.push_back(e);
}

cFacesContainedByBoundary::cFacesContainedByBoundary(tSigned32 edgesInMesh) :
 _edgeEncountered(edgesInMesh, false)
{
}

//bool
//cFacesContainedByBoundary::empty() const
//{
//    return _result.empty();
//}
//
//void
//cFacesContainedByBoundary::clear()
//{
//    tSigned32 i;
//    for(i = 0; i != _topLevelBoundary.size(); ++i)
//    {
//        assertD(_edgeEncountered[_topLevelBoundary[i].index()] == true);
//        _edgeEncountered[_topLevelBoundary[i].index()] = false;
//    }
//    _topLevelBoundary.clear();
//    _result.clear();
//}

void
cFacesContainedByBoundary::edgeCrossed(tEdge e)
{
    edgeCrossed(_topLevelBoundary, e);
}

void
cFacesContainedByBoundary::closeAndTakeResult(std::vector<tFace>& swapOutTo)
{
    assertD(swapOutTo.empty());
    if(!_topLevelBoundary.empty())
    {
        processLoop(_topLevelBoundary, 0);
        _topLevelBoundary.clear();
    }
    _result.swap(swapOutTo);

#ifdef ASSERTIONS_ON
    tSigned32 i;
    for(i = 0; i != SizeL(_edgeEncountered); ++i)
    {
        assertD(_edgeEncountered[i] == false);
    }
#endif
}
