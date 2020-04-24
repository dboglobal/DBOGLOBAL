// **********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
// **********************************************************************

#include "platform_common/Header.h"
#include "ExternalShapeTraversal.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/ElementsTraversal.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"

//.... put this somewhere more sensible
template <class tElement, class tOverlay> bool
ElementContainsPoint(const tElement& element, const tPoint& point, const tOverlay& overlay)
{
    switch(element.type())
    {
    default:
        invalid();
    case tElement::FACE:
        {
            typename tElement::tFace f = element.asFace();
            assertD(!f->isExternal());
            typename tElement::tEdge e = tElement::tMesh::beginEdge(f);
            do
            {
                if(overlay.getEdgeLine(e).side(point) != SIDE_RIGHT)
                {
                    return false;
                }
            }
            while(e.toNextInFace());
            return true;
        }
    case tElement::EDGE:
        {
            typename tElement::tEdge e = element.asEdge();
            tLine l = overlay.getEdgeLine(e);
            if(l.direction(point) != DIRECTION_FORWARD)
            {
                return false;
            }
            return l.reversed().right90().side(point) == SIDE_LEFT;
        }
    case tElement::VERTEX:
        return overlay.getVertexPoint(tElement::tMesh::firstEdge(element.asVertex())) == point;
    }
}

void
cExternalShapeTraversal::followExternalEdges()
{
    assertD(_currentEl.isFace() && _currentEl.asFace()->isExternal());
    assertD(_previousEl.valid());

    cMeshElement<tMesh_3D_FaceTracked> prev, current, next;
    if(ElementContainsPoint(_previousEl, GetVertexPoint(_shapeEdge), _overlay))
    {
        prev = cMeshElement<tMesh_3D_FaceTracked>(_shapeEdge.prev());
        current = cMeshElement<tMesh_3D_FaceTracked>(_shapeEdge.vertex());
    }
    else
    {
        prev.clear();
        current = cMeshElement<tMesh_3D_FaceTracked>(_shapeEdge);
    }

    tEdge_3D_FaceTracked externalEdge;
    if(_previousEl.isEdge())
    {
        externalEdge = _previousEl.asEdgeInFace(_currentEl.asFace());
    }
    else
    {
        externalEdge = EdgeInFaceForVertex<tMesh_3D_FaceTracked>(_currentEl.asFace(), _previousEl.asVertex());
    }

    tEdge_3D_FaceTracked loopCheck = externalEdge;
    while(1)
    {
        tLine traverseLine = _overlay.getEdgeLine(externalEdge);
        while(!ElementContainsPoint(current, traverseLine.end()))
        {
            next = NextElementAlongLine_SpinThroughLeft(prev, current, traverseLine);
            assertD(!next.isFace() || !next.isExternalFace());
            if(next.isExternalEdge() || next.isExternalVertex())
            {
				_atVertex = next.isExternalVertex();

              // reentry

                assertD(!ElementContainsPoint(next, traverseLine.start()));
                if(ElementContainsPoint(next, traverseLine.end()))
                {
                    _previousEl = cMeshElement<tMesh_3D_FaceTracked>(externalEdge);
                    _currentEl = cMeshElement<tMesh_3D_FaceTracked>(externalEdge.next().vertex());
                }
                else
                {
                    _previousEl.clear();
                    _currentEl = cMeshElement<tMesh_3D_FaceTracked>(externalEdge);
                }

                if(next.isEdge())
                {
                    _shapeEdge = next.asEdge();
                    if(!_shapeEdge.face()->isExternal())
                    {
                        _shapeEdge = _shapeEdge.twin();
                    }
                    assertD(traverseLine.axis().side(GetEdgeAxis(_shapeEdge)) == SIDE_LEFT);
                }
                else
                {
                    _shapeEdge = ExternalEdgeForVertex(tMesh_3D_FaceTracked::firstEdge(next.asVertex()));
                }
                return;
            }
            prev = current;
            current = next;
        }
        ++externalEdge;
        assertD(externalEdge != loopCheck);
    }
}

void
cExternalShapeTraversal::meshModified(
        const cMeshElement<tMesh_3D_FaceTracked>& newPreviousEl,
        const cMeshElement<tMesh_3D_FaceTracked>& newCurrentEl)
{
    _previousEl = newPreviousEl;
    _currentEl = newCurrentEl;
}

cMeshElement<tMesh_3D_FaceTracked>
cExternalShapeTraversal::advance()
{
    if(_currentEl.isExternalFace())
    {
        assertD(!_atVertex);
        followExternalEdges();
    }
    else
    {
        tLine traverseLine = GetEdgeLine(_shapeEdge);
        cMeshElement<tMesh_3D_FaceTracked> nextEl = NextElementAlongLine_SpinThroughLeft(_previousEl, _currentEl, traverseLine, _overlay);
        _previousEl = _currentEl;
        _currentEl = nextEl;
        _atVertex = !_currentEl.isExternalFace() && ElementContainsPoint(_currentEl, traverseLine.end(), _overlay);
        if(_atVertex)
        {
            ++_shapeEdge;
        }
    }
    return _currentEl;
}

bool
cExternalShapeTraversal::atShapeVertex(tPoint& vertex) const
{
    if(_atVertex)
    {
        vertex = GetVertexPoint(_shapeEdge);
    }
    return _atVertex;
}

tLine
cExternalShapeTraversal::getCurrentLine() const
{
    return GetEdgeLine(_shapeEdge);
}
