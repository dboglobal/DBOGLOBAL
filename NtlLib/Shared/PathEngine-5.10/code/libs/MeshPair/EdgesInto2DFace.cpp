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
#include "EdgesInto2DFace.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"

cEdgesInto2DFace::eEvent
cEdgesInto2DFace::rotateWindowAndCheckEvent(
        const cMeshElement<tMesh_3D>& nextEl,
        tEdge_3D& firstEdge, tEdge_3D& lastEdge
        )
{
    eEvent result = NONE;
    // advance the window
    if(_elementWindow.size() != 3)
    {
        _elementWindow.push_back(nextEl);
        _containsCornerWindow.push_back(false);
        if(_elementWindow.size() != 3)
        {
            return result;
        }
    }
    else
    {
        _elementWindow[0] = _elementWindow[1];
        _elementWindow[1] = _elementWindow[2];
        _containsCornerWindow.setBit(0, _containsCornerWindow[1]);
        _containsCornerWindow.setBit(1, _containsCornerWindow[2]);
        _elementWindow.back() = nextEl;
        _containsCornerWindow.setLastBit(false);
    }
    if(_containsCornerWindow[1])
    {
        result = eEvent(result + CORNER);
    }
    if(_elementWindow[1].isEdge())
    {
        if(_elementWindow[0].isFace() && _elementWindow[2].isFace() && _elementWindow[0].asFace() != _elementWindow[2].asFace())
        {
          // simple edge crossing event
            firstEdge = _elementWindow[1].asEdgeInFace(_elementWindow[0].asFace());
            lastEdge = firstEdge;
            result = eEvent(result + EDGES);
        }
    }
    else if(_elementWindow[1].isVertex())
    {
        tVertex_3D v = _elementWindow[1].asVertex();
        if(_elementWindow[0].isFace())
        {
            firstEdge = EdgeInFaceForVertex<tMesh_3D>(_elementWindow[0].asFace(), v);
        }
        else
        {
            firstEdge = _elementWindow[0].asEdgeFromVertex(v).twin().next();
        }
        if(_elementWindow[2].isFace())
        {
            lastEdge = EdgeInFaceForVertex<tMesh_3D>(_elementWindow[2].asFace(), v);
        }
        else
        {
            lastEdge = _elementWindow[2].asEdgeFromVertex(v);
        }
        if(lastEdge != firstEdge)
        {
            lastEdge = lastEdge.prev().twin();
            result = eEvent(result + EDGES);
        }
    }
    return result;
}

cEdgesInto2DFace::cEdgesInto2DFace(tFace f, const cMeshElement<tMesh_3D>& startElement) :
 _elementWindow(3),
 _containsCornerWindow(3),
 _elementsAround(f, startElement)
{
    _elementWindow.resize(1);
    _elementWindow[0] = startElement;
    _containsCornerWindow.resize(1);
    _containsCornerWindow.setBit(0, true);
    _e = tMesh::beginEdge(f);
    _completed = false;
}

cEdgesInto2DFace::eEvent
cEdgesInto2DFace::nextEvent(tEdge_3D& firstEdge, tEdge_3D& lastEdge)
{
    cMeshElement<tMesh_3D> nextEl;
    eEvent nextEvent;
    do
    {
        cElementsAround2DFace::eEvent eventAround;
        eventAround = _elementsAround.nextEvent(nextEl);
        if(eventAround == cElementsAround2DFace::CORNER)
        {
            _containsCornerWindow.setLastBit(true);
            bool iterate = _e.toNextInFace();
            if(!iterate)
            {
            // reached final corner, wrap up
                if(_elementWindow.back().isFace())
                {
                    return eEvent(CORNER + COMPLETED);
                }
                eventAround = _elementsAround.nextEvent(nextEl);
                assertD(eventAround != CORNER);
                return eEvent(rotateWindowAndCheckEvent(nextEl, firstEdge, lastEdge) + COMPLETED);
            }
            nextEvent = NONE;
        }        
        else
        {
            nextEvent = rotateWindowAndCheckEvent(nextEl, firstEdge, lastEdge);
        }
    }
    while(nextEvent == NONE);
    return nextEvent;
}
