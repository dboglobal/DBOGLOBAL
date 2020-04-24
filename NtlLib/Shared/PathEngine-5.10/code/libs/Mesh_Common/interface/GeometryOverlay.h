//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GEOMETRY_OVERLAY_INCLUDED
#define GEOMETRY_OVERLAY_INCLUDED

#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "common/STL/vector.h"

template <class tMesh>
class cNullGeometryOverlay
{
public:
    tLine getEdgeLine(typename tMesh::edge e) const
    {
        return GetEdgeLine(e);
    }
    tPoint getEdgeAxis(typename tMesh::edge e) const
    {
        return GetEdgeAxis(e);
    }
    tPoint getVertexPoint(typename tMesh::edge e) const
    {
        return GetVertexPoint(e);
    }
};

template <class tMesh>
class cNullGeometryOverlay_VertexIsIntersection
{
public:
    tLine getEdgeLine(typename tMesh::edge e) const
    {
        return GetEdgeLine(e);
    }
    tPoint getEdgeAxis(typename tMesh::edge e) const
    {
        return GetEdgeAxis(e);
    }
    tIntersection getVertexPoint(typename tMesh::edge e) const
    {
        return GetVertexPoint(e);
    }
};

template <class tMesh>
class cGeometryOverlay
{
    std::vector<typename tMesh::edge> _edges;
    std::vector<tLine> _lines;

public:

    void pushBack(typename tMesh::edge e, tLine l)
    {
        _edges.push_back(e);
        _lines.push_back(l);
    }

    tLine getEdgeLine(typename tMesh::edge e) const
    {
        tSigned32 i;
        for(i = 0; i != SizeL(_edges); ++i)
        {
            if(e == _edges[i])
            {
                return _lines[i];
            }
        }
        return GetEdgeLine(e);
    }
    tPoint getEdgeAxis(typename tMesh::edge e) const
    {
        tSigned32 i;
        for(i = 0; i != SizeL(_edges); ++i)
        {
            if(e == _edges[i])
            {
                return _lines[i].axis();
            }
        }
        return GetEdgeAxis(e);
    }
    tIntersection getVertexPoint(typename tMesh::edge e) const
    {
        tPoint axisE = getEdgeAxis(e);
        typename tMesh::edge eIntersect = e;
        do
        {
            eIntersect = eIntersect.twin().next();
            assertD(eIntersect != e);
        }
        while(axisE.side(getEdgeAxis(eIntersect)) == SIDE_CENTRE);
        return tIntersection(getEdgeLine(e), getEdgeLine(eIntersect));
    }
};

#endif
