//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/EdgesTraversal.h"

cEdgesTraversal::cEdgesTraversal(tFace startFace)
{
    init_InFace(startFace);
}
cEdgesTraversal::cEdgesTraversal(tEdge entryEdge)
{
    init_Entry(entryEdge);
}

void
cEdgesTraversal::init_InFace(tFace startFace)
{
    assertD(empty());
    _edges.push_back(tMesh::beginEdge(startFace));
}

void
cEdgesTraversal::init_Entry(tEdge entryEdge)
{
    assertD(empty());
    _edges.push_back(entryEdge.twin());
    _edges.push_back(entryEdge);
}

void
cEdgesTraversal::reverse()
{
    assertD(!empty());
    std::vector<tEdge> reversedEdges;
    if(exits())
    {
        reversedEdges.push_back(_edges.back());
        reversedEdges.push_back(_edges.back().twin());
    }
    else
    {
        reversedEdges.push_back(tMesh::beginEdge(lastFaceCrossed()));
    }

    tSigned32 i = edgesCrossed();
    while(i--)
    {
        reversedEdges.push_back(edgeCrossed(i));
    }

    if(enters())
    {
        reversedEdges.push_back(_edges[1].twin());
    }

    _edges.swap(reversedEdges);
//    assertD(isValid());
}

bool
cEdgesTraversal::isValid() const
{
    if(_edges.empty())
    {
        return false;
    }
    if(!_edges[0].valid())
    {
        return false;
    }
    tFace f = _edges[0].face();
    tSigned32 i;
    for(i = 1; i < SizeL(_edges); i++)
    {
        if(!_edges[i].valid())
        {
            return false;
        }
        if(_edges[i].twin().face() != f)
        {
            return false;
        }
        f = _edges[i].face();
    }
    for(i = 0; i != facesCrossed(); ++i)
    {
        if(faceCrossed(i)->isExternal())
        {
            return false;
        }
    }
    return true;
}


void
cEdgesTraversal::pushEdgeCrossing(tEdge crossed)
{
    assertD(!_edges.empty());
    assertD(!crossed.face()->isExternal());
    assertD(!crossed.twin().face()->isExternal());
    assertD(crossed.face() == _edges.back().face());
    _edges.push_back(crossed.twin());
}

void
cEdgesTraversal::pushExit(tEdge exitEdge)
{
    assertD(!_edges.empty());
    assertD(!exitEdge.face()->isExternal());
    assertD(exitEdge.face() == _edges.back().face());
    _edges.push_back(exitEdge.twin());
}

void
cEdgesTraversal::popEdgeCrossing()
{
    assertD(edgesCrossed());
    assertD(!exits());
    _edges.pop_back();
}

void
cEdgesTraversal::popExit()
{
    assertD(!empty());
    assertD(exits());
    _edges.pop_back();
}

tFace
cEdgesTraversal::faceCrossed(tSigned32 index) const
{
    if(enters())
    {
        index++;
    }
    return _edges[index].face();
}

tSigned32
cEdgesTraversal::indexForFace(tFace f) const
{
    tSigned32 i;
    for(i = 0; i < facesCrossed(); i++)
    {
        if(faceCrossed(i) == f)
        {
            return i;
        }
    }
    return -1;
}

tSigned32
cEdgesTraversal::edgesCrossed() const
{
	return facesCrossed() - 1;
}
tEdge
cEdgesTraversal::edgeCrossed(tSigned32 index) const
{
    if(enters())
    {
        index++;
    }
    index++;
	return _edges[index].twin();
}
tEdge
cEdgesTraversal::lastEdgeCrossed() const
{
    return edgeCrossed(edgesCrossed() - 1);
}

bool
cEdgesTraversal::enters() const
{
    return _edges.front().face()->isExternal();
}
bool
cEdgesTraversal::exits() const
{
    return _edges.back().face()->isExternal();
}

tFace
cEdgesTraversal::externalFaceForEntry() const
{
    return entryEdge().twin().face();
}
tFace
cEdgesTraversal::externalFaceForExit() const
{
    return exitEdge().twin().face();
}

