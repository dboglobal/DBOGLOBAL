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
#include "FacesOverlapped.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/STL_Helpers.h"

static tFace_3D
EffectiveFace(const cMeshElement<tMesh_3D>& el1, const cMeshElement<tMesh_3D>& el2)
{
    assertD(!el1.isPositionallyEquivalent(el2));

    if(el2.isFace())
    {
        return el2.asFace();
    }

  // if vertex or edge follow a face then that is the effective face
    if(el1.isFace())
    {
        return el1.asFace();
    }

    if(el2.isEdge())
    {
        tEdge_3D e = el2.asEdgeFromVertex(el1.asVertex());
        return e.face();
    }
    tEdge_3D e = el1.asEdgeFromVertex(el2.asVertex()).twin();
    return e.face();
}

static void
GetBoundaryEdgesAtCrossing(
        const cMeshElement<tMesh_3D>& el0,
        const cMeshElement<tMesh_3D>& el1,
        const cMeshElement<tMesh_3D>& el2,
        tEdge_3D& afterBoundary, tEdge_3D& beforeNextBoundary)
{
    if(el1.isEdge())
    {
        assertD(el0.isFace() && el2.isFace());
        tEdge_3D e = el1.asEdgeInFace(el0.asFace());
        afterBoundary = e;
        beforeNextBoundary = e.twin();
        return;
    }
    tVertex_3D v = el1.asVertex();
    if(el0.isEdge())
    {
        afterBoundary = el0.asEdgeFromVertex(v).twin().next();
    }
    else
    {
        afterBoundary = EdgeInFaceForVertex<tMesh_3D>(el0.asFace(), v);
    }
    if(el2.isEdge())
    {
        beforeNextBoundary = el2.asEdgeFromVertex(v).prev();
    }
    else
    {
        beforeNextBoundary = EdgeInFaceForVertex<tMesh_3D>(el2.asFace(), v).prev();
    }
}

void
cFacesOverlapped::addFace(tFace_3D f)
{
    tSigned32 i = f.index();
    assertD(!_overlappedFlagBuffer[i]);
    _overlapped.push_back(i);
    _overlappedFlagBuffer.setBit(i, true);
    _openBuffer.push_back(f);
    _processedFlagBuffer.setBit(i, true);
}
void
cFacesOverlapped::addFaceIfNotFlagged(tFace_3D f)
{
    tSigned32 i = f.index();
    if(!_overlappedFlagBuffer[i])
    {
        _overlapped.push_back(i);
        _overlappedFlagBuffer.setBit(i, true);
        _openBuffer.push_back(f);
        _processedFlagBuffer.setBit(i, true);
    }
}

void
cFacesOverlapped::markFacesOutsideBoundary(tEdge_3D before, tEdge_3D after)
{
    assertD(before.face() == after.face());
    ++before;
    while(before != after)
    {
        tFace_3D connected = before.twin().face();
        _processedFlagBuffer.setBit(connected.index(), true);
        ++before;
    }
}

cFacesOverlapped::cFacesOverlapped(tSigned32 facesOnMesh) :
 _processedFlagBuffer(facesOnMesh),
 _overlappedFlagBuffer(facesOnMesh),
 _buffer(2)
{
}

void
cFacesOverlapped::init()
{
    _processedFlagBuffer.fill(false);
    _overlappedFlagBuffer.fill(false);
    assertD(_overlapped.empty());
    assertD(_openBuffer.empty());
    _buffer.clear();
}

void
cFacesOverlapped::elementTraversed(const cMeshElement<tMesh_3D>& element)
{
    if(!_buffer.empty() && element.isPositionallyEquivalent(_buffer.back()))
    {
        assertD(!element.isVertex());
        return;
    }
    if(_buffer.size() != 2)
    {
        if(_buffer.size() == 1)
        {
            _buffer.push_back(element);
            _currentFace = EffectiveFace(_buffer[0], _buffer[1]);
            addFace(_currentFace);
            return;
        }
        assertD(_buffer.empty());
        _startElement = element;
        _afterFirstBoundary.clear();
        _buffer.push_back(element);
        return;
    }
    tFace_3D nextFace = EffectiveFace(_buffer[1], element);
    if(nextFace != _currentFace)
    {
        tEdge_3D afterBoundary;
        tEdge_3D beforeNextBoundary;
        GetBoundaryEdgesAtCrossing(_buffer[0], _buffer[1], element, afterBoundary, beforeNextBoundary);
        if(_afterFirstBoundary.valid())
        {
            markFacesOutsideBoundary(_beforeBoundary, afterBoundary);
            _beforeBoundary = beforeNextBoundary;
        }
        else
        {
            _afterFirstBoundary = afterBoundary;
            _beforeBoundary = beforeNextBoundary;
        }
        _currentFace = nextFace;
        addFaceIfNotFlagged(_currentFace);
    }
    _buffer[0] = _buffer[1];
    _buffer[1] = element;
}

void
cFacesOverlapped::closeAndTakeResult(std::vector<tSigned32>& swapResultTo)
{
    assertD(swapResultTo.empty());
    elementTraversed(_startElement);

    if(!_afterFirstBoundary.valid())
    {
      // the boundary doesn't exit the startface
        if(_overlapped.empty())
        {
            assertD(_buffer.size() == 1);
            assertD(_buffer[0].isFace());
            _overlapped.push_back(_buffer[0].asFace().index());
        }
        _openBuffer.clear();
    }
    else
    {
      // close the boundary
        markFacesOutsideBoundary(_beforeBoundary, _afterFirstBoundary);

      // do the fill
        while(!_openBuffer.empty())
        {
            tFace_3D f = _openBuffer.back();
            _openBuffer.pop_back();
            tEdge_3D e = tMesh_3D::beginEdge(f);
            do
            {
                tFace_3D connected = e.twin().face();
                if(!_processedFlagBuffer[connected.index()])
                {
                    assertD(!_overlappedFlagBuffer[connected.index()]);
                    _overlapped.push_back(connected.index());
                    _openBuffer.push_back(connected);
                    _processedFlagBuffer.setBit(connected.index(), true);
                }
            }
            while(e.toNextInFace());
        }
    }
    swapResultTo.swap(_overlapped);
}

