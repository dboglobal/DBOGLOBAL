//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POSITION_FOR_POINT_IN_FACE_INCLUDED
#define POSITION_FOR_POINT_IN_FACE_INCLUDED

#include "libs/Mesh_Common/interface/Mesh.h"
#include "common/TemplateMesh/MeshElement.h"
#include "libs/Mesh_Common/interface/GeometryOverlay.h"
#include "libs/Geometry/interface/iPointLike.h"

template <class tMesh, class tPointLike> cMeshElement<tMesh>
ElementForPointInOverlappingFace(typename tMesh::face f, const tPointLike& p)
{
    typename tMesh::edge onEdge;
    onEdge.clear();
    typename tMesh::edge e = tMesh::beginEdge(f);
    do
    {
        eSide side = SideOfLine(GetEdgeLine(e), p);
        if(side == SIDE_LEFT)
        {
            cMeshElement<tMesh> result;
            result.setInvalid();
            return result;
        }
        if(side == SIDE_CENTRE)
        {
            if(onEdge.valid())
            {
            // exactly on two edges now
            // i.e. on a vertex between these edges,
            // or at the intersection of the extensions of the edges (in which case outside the face)
                if(onEdge == e.next())
                {
                    return onEdge.vertex();
                }
                if(e == onEdge.next())
                {
                    return e.vertex();
                }
                cMeshElement<tMesh> result;
                result.setInvalid();
                return result;
            }
            onEdge = e;
        }
    }
    while(++e != tMesh::beginEdge(f));

    if(onEdge.valid())
    {
    // exactly on just one edge
        return onEdge;
    }
    return f;
}


template <class tElement, class tPoint, class tOverlay> bool
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
                if(SideOfLine(overlay.getEdgeLine(e), point) != SIDE_RIGHT)
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
            if(e.face()->isExternal())
            {
                e = e.twin();
            }
            assertD(overlay.getEdgeAxis(e).side(overlay.getEdgeAxis(e.next())) == SIDE_RIGHT);
            assertD(overlay.getEdgeAxis(e.prev()).side(overlay.getEdgeAxis(e)) == SIDE_RIGHT);
            if(SideOfLine(overlay.getEdgeLine(e), point) != SIDE_CENTRE)
            {
                return false;
            }
            if(SideOfLine(overlay.getEdgeLine(e.next()), point) != SIDE_RIGHT)
            {
                return false;
            }
            if(SideOfLine(overlay.getEdgeLine(e.prev()), point) != SIDE_RIGHT)
            {
                return false;
            }
            return true;
        }
    case tElement::VERTEX:
        {
            typename tElement::tEdge e = tElement::tMesh::firstEdge(element.asVertex());
            if(e.face()->isExternal())
            {
                e = e.prev().twin();
            }
            assertD(overlay.getEdgeAxis(e.prev()).side(overlay.getEdgeAxis(e)) == SIDE_RIGHT);
            if(SideOfLine(overlay.getEdgeLine(e), point) != SIDE_CENTRE)
            {
                return false;
            }
            if(SideOfLine(overlay.getEdgeLine(e.prev()), point) != SIDE_CENTRE)
            {
                return false;
            }
            return true;
        }
    }
}
template <class tElement, class tPoint> bool
ElementContainsPoint(const tElement& element, const tPoint& point)
{
    cNullGeometryOverlay<typename tElement::tMesh> overlay;
    return ElementContainsPoint(element, point, overlay);
}

#endif
