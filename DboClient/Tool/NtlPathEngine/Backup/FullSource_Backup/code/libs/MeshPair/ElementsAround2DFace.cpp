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
#include "ElementsAround2DFace.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"

void
cElementsAround2DFace::setNextEvent()
{
    if(ElementContainsPoint(_el, _traverseLine.end()))
    {
        _nextEvent = CORNER;
    }
    else
    {
        _nextEvent = ELEMENT;
        _nextEl = NextElementAlongLine(_el, _traverseLine);
    }
}
void
cElementsAround2DFace::setNextEvent_EdgeStart()
{
    if(VertexIsExternal(_e))
    {
        _nextEvent = ELEMENT;
        tSigned32 spinCount = SpinCountAroundVertex(_e);
        _nextEl = TraverseFromVertex_Elements<tMesh_3D>(_el.asVertex(), _traverseLine.end(), spinCount);
        return;
    }
    setNextEvent();
}

cElementsAround2DFace::cElementsAround2DFace(tFace f, const cMeshElement<tMesh_3D>& startElement)
{
    _e = tMesh::beginEdge(f);
    _traverseLine = GetEdgeLine(_e);
    _el = startElement;
    setNextEvent_EdgeStart();
}

cElementsAround2DFace::eEvent
cElementsAround2DFace::nextEvent(cMeshElement<tMesh_3D>& element)
{
    if(_nextEvent == ELEMENT)
    {
        element = _nextEl;
        _el = _nextEl;
        setNextEvent();
        return ELEMENT;
    }
    ++_e;
    _traverseLine = GetEdgeLine(_e);
    setNextEvent_EdgeStart();
    return CORNER;
}
