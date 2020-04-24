//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GEOMETRY__CONVEX_POLYGON_INCLUDED
#define GEOMETRY__CONVEX_POLYGON_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "common/STL/vector.h"

class cAxisFraction;

class cConvexPoly
{
    tSigned32 _points;
    tPoint* _pointArray;

    bool edgeSeparates(tSigned32 edgeIndex, const cConvexPoly& rhs) const;
    bool checkStartAndEndForCornerCrossing(tSigned32 cornerIndex, const tPoint& lineStart, const tPoint& lineAxis) const;

public:

    static bool canConstructFrom(tSigned32 points, const tPoint* pointArray);

    // can be constructed empty to be used as the destination in combination methods
    // but an empty constructed instance is not valid for any other methods
    cConvexPoly()
    {
        _pointArray = 0;
    }
    cConvexPoly(tSigned32 points, tPoint* pointArray); // ownership is taken here for pointArray, which must be allocated with the array version of new
    cConvexPoly(const tPoint& centre, tSigned32 radius); // constructs an axis aligned square
    cConvexPoly(const tPoint& centre, tSigned32 radius, tSigned32 range); // constructs an axis aligned square, clipped to the specified range
    ~cConvexPoly();

    cConvexPoly(const cConvexPoly& rhs);
    cConvexPoly& operator=(const cConvexPoly& rhs);

    void swap(cConvexPoly& rhs);

    bool operator==(const cConvexPoly& rhs) const;

    void operator+=(const tPoint& offset);
    void operator-=(const tPoint& offset);
//    cConvexPoly operator+(const tPoint& offset) const
//    {
//        cConvexPoly result(*this);
//        result += offset;
//        return result;
//    }

    void normalise(cConvexPoly& result) const;
    void minkowskiExpand(const cConvexPoly& expandBy, cConvexPoly& result) const;
    void extrude(const tPoint& extrudeStart, const tPoint& extrudeEnd, cConvexPoly& result) const;
    void offset(const tPoint& offsetBy, cConvexPoly& result) const;

    tSigned32 getRangeUsed() const;
    void setRange(tSigned32 value);

  // vertex and edge queries

    tSigned32 sides() const
    {
        return _points;
    }
    const tPoint& vertex(tSigned32 index) const
    {
        return _pointArray[index];
    }
    const tPoint& vertex_WrappedAround(tSigned32 index) const
    {
        assertD(index >= 0);
        if(index >= _points)
        {
            index -= _points;
            assertD(index < _points);
        }
        return _pointArray[index];
    }
    tPoint getEdgeAxis(tSigned32 index) const
    {
        return vertex_WrappedAround(index + 1) - vertex(index);
    }
    tLine getBoundaryLine(tSigned32 index) const
    {
        return tLine(vertex(index), vertex_WrappedAround(index + 1));
    }
    tLine getBoundaryLine_Reversed(tSigned32 index) const
    {
        return tLine(vertex_WrappedAround(index + 1), vertex(index));
    }
    tSigned32 nextVertex(const tSigned32 vertex) const
    {
        if(vertex == sides() - 1)
        {
            return 0;
        }
        return vertex + 1;
    }
    tSigned32 prevVertex(const tSigned32 vertex) const
    {
        if(vertex == 0)
        {
            return sides() - 1;
        }
        return vertex - 1;
    }

  // point collision methods

    bool pointCollides2D(const tPoint& p) const;
    bool pointCollides2D(const tIntersection& p) const;
    bool pointCollides2D_Inverted(const tPoint& p) const;
    bool pointCollides2D_Inverted(const tIntersection& p) const;
    bool pointIsOnBoundary2D(const tIntersection& p) const;

  // line collision methods

    bool lineCollides2D(const tPoint& lineStart, const tPoint& lineAxis, tSigned32& collidingLineIndex) const;

    bool
    lineCollides2D(
            const tLine& l,
            const cAxisFraction& startDistance, const cAxisFraction& endDistance,
            tSigned32& collidingLineIndex
            ) const;

    bool enters_NotAtCorner(const cConvexPoly& rhs, tSigned32& lhsSection, tSigned32& rhsSection) const;
    bool exits_NotAtCorner(const cConvexPoly& rhs, tSigned32& lhsSection, tSigned32& rhsSection) const;

  // other stuff from cPlacedConvexShape

    bool overlaps2D(const cConvexPoly& rhs) const;
    bool overlaps2D(const cConvexPoly& rhs, tIntersection& pointInsideOrOnBoundaryOfBoth) const;

    tSigned32 getIndexForSilhouettePoint(eSide side, const tPoint& p) const;
    bool satisfiesMaxCircumference(tSigned32 maxCircumference) const;

    void getBounds(tPoint& minimum, tPoint& maximum) const;

  // stuff to support minkowski expansion

    tPoint expandEdgeVector(tSigned32 index) const
    {
        return vertex(index) - vertex(nextVertex(index));
    }

    void getStartEndExpand(const tPoint &edgeVector, tSigned32 &start, tSigned32 &end) const;

    tSigned32 endExpand(const tPoint& edgeVector) const
    {
        tSigned32 start, end;
        getStartEndExpand(edgeVector, start, end);
        return end;
    }
    //.... optimise
    tSigned32 startExpand(const tPoint& edgeVector) const
    {
        tSigned32 start, end;
        getStartEndExpand(edgeVector, start, end);
        return start;
    }

    tSigned32 getEdgesForCorner(const tPoint& previousEdge, const tPoint& nextEdge) const;
    tSigned32 getEdgesForCorner(
            const tPoint& previousAxis, const tPoint& nextAxis,
            tSigned32 spinCount
            ) const;
    tSigned32 getEdgesForCorner_DontConnectInline(
            const tPoint& previousAxis, const tPoint& nextAxis,
            tSigned32 spinCount
            ) const;
};

#endif
