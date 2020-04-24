//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_COMMON_ELEMENTS_TRAVERSAL_DEFINED
#define MESH_COMMON_ELEMENTS_TRAVERSAL_DEFINED

#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"

template <class tElement, class tOverlay> tElement
NextElementAlongLine(const tElement& el, const tLine& line, const tOverlay& overlay)
{
    typedef typename tElement::tMesh::edge tEdge;
    typedef typename tElement::tMesh::vertex tVertex;

    if(el.type() == tElement::EDGE)
    {
        tEdge e = el.asEdge();
        eDirection direction = overlay.getEdgeLine(e).direction(line);
        switch(direction)
        {
        default:
            invalid();
        case DIRECTION_FORWARD:
            return tElement(e.next().vertex());
        case DIRECTION_RIGHT:
            return tElement(e.face());
        case DIRECTION_BACK:
            return tElement(e.vertex());
        case DIRECTION_LEFT:
            return tElement(e.twin().face());
        }
    }    

    if(el.type() == tElement::VERTEX)
    {
        tEdge e = tElement::tMesh::firstEdge(el.asVertex());
        assertD(e.valid());
        assertD(!VertexIsExternal(e) || ExternalVertexIsInLine(ExternalEdgeForVertex(e), overlay));
        while(1)
        {
            eDirection direction = overlay.getEdgeLine(e).direction(line);
            if(direction == DIRECTION_FORWARD)
            {
                return tElement(e);
            }
            if(direction == DIRECTION_RIGHT
             && overlay.getEdgeLine(e.prev()).side(line) == SIDE_RIGHT)
            {
                return tElement(e.face());
            }
            bool iterate = e.toNextAroundVertex();
            assertD(iterate);
        }
    }    

    tEdge e = tElement::tMesh::beginEdge(el.asFace());
    tEdge firstFacing, lastFacing;
    FacingRange_EntryUnknown(line, e, firstFacing, lastFacing, overlay);
    bool crossesVertex;
    tEdge exitEdge = ExitEdge_CrossesVertex(line, firstFacing, lastFacing, crossesVertex, overlay);
    if(crossesVertex)
    {
        return tElement(exitEdge.vertex());
    }
    return tElement(exitEdge);
}
template <class tElement> tElement
NextElementAlongLine(const tElement& el, const tLine& line)
{
    cNullGeometryOverlay<typename tElement::tMesh> overlay;
    return NextElementAlongLine(el, line, overlay);
}

// prev only needs to be set if el is an external and non-inline vertex
template <class tElement, class tOverlay> tElement
NextElementAlongLine_SpinThroughLeft(const tElement& prev, const tElement& el, const tLine& line, const tOverlay& overlay)
{
    if(!el.isVertex() || !prev.valid())
    {
        return NextElementAlongLine(el, line, overlay);
    }
    typename tElement::tVertex v = el.asVertex();
     typename tElement::tEdge e;
    tPoint axis = line.axis();
    eSide sidePrev;
    if(prev.type() == tElement::FACE)
    {
        e = EdgeInFaceForVertex<typename tElement::tMesh>(prev.asFace(), el.asVertex());
        sidePrev = overlay.getEdgeAxis(e).side(axis);
        goto startInFace;
    }
    e = prev.asEdge();
    if(e.vertex() != v)
    {
        e = e.twin();
        assertD(e.vertex() == v);
    }
    sidePrev = overlay.getEdgeAxis(e).side(axis);
    while(1)
    {
      // check edge
        if(sidePrev == SIDE_CENTRE && overlay.getEdgeAxis(e).direction_KnownToBeInline(axis) == DIRECTION_FORWARD)
        {
            return tElement(e);
        }
startInFace:
      // check face
        typename tElement::tEdge eNext = e.prev().twin();
        eSide sideNext = overlay.getEdgeAxis(eNext).side(axis);
        if(e.face()->isExternal())
        {
            return tElement(e.face());
        }
        else
        {
            if(sidePrev == SIDE_RIGHT && sideNext == SIDE_LEFT)
            {
                return tElement(e.face());
            }
        }
        e = eNext;
        sidePrev = sideNext;
    }
}

template <class tElement> tElement
NextElementAlongLine_SpinThroughLeft(const tElement& prev, const tElement& el, const tLine& line)
{
    cNullGeometryOverlay<typename tElement::tMesh> overlay;
    return NextElementAlongLine_SpinThroughLeft(prev, el, line, overlay);
}

template <class tElement, class tOverlay> tElement
NextElementAlongLine_SpinDoesntMatter(const tElement& el, const tLine& line, const tOverlay& overlay)
{
    if(!el.isVertex() || !VertexIsExternal(tElement::tMesh::firstEdge(el.asVertex())))
    {
        return NextElementAlongLine(el, line, overlay);
    }
    typename tElement::tEdge e = tElement::tMesh::firstEdge(el.asVertex());
    e = ExternalEdgeForVertex(e).prev().twin();
    do
    {
        eDirection direction = overlay.getEdgeLine(e).direction(line);
        if(direction == DIRECTION_FORWARD)
        {
            return tElement(e);
        }
        if(direction == DIRECTION_RIGHT
            && overlay.getEdgeLine(e.prev()).side(line) == SIDE_RIGHT)
        {
            return tElement(e.face());
        }
        bool iterate = e.toNextAroundVertex();
        assertD(iterate);
    }
    while(!e.face()->isExternal());
    return tElement(e.face());
}

template <class tElement> tElement
NextElementAlongLine_SpinDoesntMatter(const tElement& el, const tLine& line)
{
    cNullGeometryOverlay<typename tElement::tMesh> overlay;
    return NextElementAlongLine_SpinDoesntMatter(el, line, overlay);
}

#endif
