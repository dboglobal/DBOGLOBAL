//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ConvexPoly.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "platform_common/AddWithOverflow.h"
#include "common/STL/algorithm.h"

static bool
LineCollidesWithSegment(const tLine& l,
        const cAxisFraction& startDistance, const cAxisFraction& endDistance,
                        const tPoint* segmentPoints)
{
    tPoint segmentAxis = segmentPoints[1] - segmentPoints[0];
    if(segmentAxis.side(l.axis()) != SIDE_RIGHT)
    {
        return false;
    }

    if(l.side(segmentPoints[0]) != SIDE_RIGHT)
    {
        return false;
    }
    eSide sideForEnd = l.side(segmentPoints[1]);
    if(sideForEnd == SIDE_RIGHT)
    {
        return false;
    }
    if(sideForEnd == SIDE_CENTRE)
    {
        tPoint nextAxis = segmentPoints[2] - segmentPoints[1];
        if(nextAxis.side(l.axis()) != SIDE_RIGHT)
        {
            return false;
        }
    }

    tLine segmentLine(segmentPoints[0], segmentPoints[1]);
    tIntersection intersection(l, segmentLine);
    cAxisFraction intersectionDistance;
    intersection.getAxisFraction(intersectionDistance.n, intersectionDistance.d);
    return startDistance <= intersectionDistance && endDistance > intersectionDistance;
}

bool
cConvexPoly::edgeSeparates(tSigned32 edgeIndex, const cConvexPoly& rhs) const
{
    tPoint base = vertex(edgeIndex);
    tPoint axis = vertex_WrappedAround(edgeIndex + 1) - base;
    tSigned32 i;
    for(i = 0; i < rhs._points; ++i)
    {
        tPoint offset = rhs.vertex(i) - base;
        if(axis.side(offset) == SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}

bool
cConvexPoly::checkStartAndEndForCornerCrossing(
        tSigned32 cornerIndex,
        const tPoint& lineStart, const tPoint& lineAxis
        ) const
{
    tPoint lineEnd = lineStart + lineAxis;
    tPoint base = vertex(cornerIndex);
    tPoint axisAfter = vertex_WrappedAround(cornerIndex + 1) - base;
    if(axisAfter.side(lineEnd - base) != SIDE_RIGHT)
    {
        return false;
    }
    if(axisAfter.side(lineStart - base) == SIDE_RIGHT)
    {
        return false;
    }
    tPoint axisBefore = base - vertex(prevVertex(cornerIndex));
    if(axisBefore.side(lineEnd - base) != SIDE_RIGHT)
    {
        return false;
    }
    return true;
}

bool
cConvexPoly::canConstructFrom(tSigned32 points, const tPoint* pointArray)
{
    if(points < 3)
    {
        return false;
    }
    tSigned32 i;
    for(i = 0; i != points - 2; ++i)
    {
        tPoint base = pointArray[i];
        tPoint axis = pointArray[i + 1] - base;
        tSigned32 j;
        for(j = i + 2; j != points; ++j)
        {
            tPoint test = pointArray[j] - base;
            if(axis.side(test) != SIDE_RIGHT)
            {
                return false;
            }
        }
    }
    return true;
}

cConvexPoly::cConvexPoly(tSigned32 points, tPoint* pointArray)
{
    assertD(canConstructFrom(points, pointArray));
    _points = points;
    _pointArray = pointArray;
}

cConvexPoly::cConvexPoly(const tPoint& centre, tSigned32 radius)
{
    _points = 4;
    _pointArray = new tPoint[4];
    _pointArray[0] = centre + tPoint(-radius, -radius, radius);
    _pointArray[1] = centre + tPoint(-radius, radius, radius);
    _pointArray[2] = centre + tPoint(radius, radius, radius);
    _pointArray[3] = centre + tPoint(radius, -radius, radius);
}

cConvexPoly::cConvexPoly(const tPoint& centre, tSigned32 radius, tSigned32 range)
{
    assertD(radius >= -(range * 2) && radius <= (range * 2));
    assertD(centre.getRangeUsed() < range);
    assertD(centre.getRange() < range);
    bool overflow;
    tSigned32 minX, maxX, minY, maxY;
    overflow = SubtractWithOverflow(centre.getX(), radius, minX);
    if(overflow || minX < -range)
    {
        minX = -range;
    }
    overflow = AddWithOverflow(centre.getX(), radius, maxX);
    if(overflow || maxX > range)
    {
        maxX = range;
    }
    overflow = SubtractWithOverflow(centre.getY(), radius, minY);
    if(overflow || minY < -range)
    {
        minY = -range;
    }
    overflow = AddWithOverflow(centre.getY(), radius, maxY);
    if(overflow || maxY > range)
    {
        maxY = range;
    }
    _points = 4;
    _pointArray = new tPoint[4];
    _pointArray[0] = tPoint(minX, minY, range);
    _pointArray[1] = tPoint(minX, maxY, range);
    _pointArray[2] = tPoint(maxX, maxY, range);
    _pointArray[3] = tPoint(maxX, minY, range);
}

cConvexPoly::~cConvexPoly()
{
    delete [] _pointArray;
}

cConvexPoly::cConvexPoly(const cConvexPoly& rhs)
{
    if(rhs._pointArray == 0)
    {
        _pointArray = 0;
        return;
    }
    _points = rhs._points;
    _pointArray = new tPoint[_points];
    tSigned32 i;
    for(i = 0; i != _points; ++i)
    {
        _pointArray[i] = rhs._pointArray[i];
    }
}

cConvexPoly&
cConvexPoly::operator=(const cConvexPoly& rhs)
{
    if(this == &rhs)
    {
        return *this;
    }
    delete [] _pointArray;
    if(rhs._pointArray == 0)
    {
        _pointArray = 0;
        return *this;
    }
    _points = rhs._points;
    _pointArray = new tPoint[_points];
    tSigned32 i;
    for(i = 0; i != _points; ++i)
    {
        _pointArray[i] = rhs._pointArray[i];
    }
    return *this;
}

void
cConvexPoly::swap(cConvexPoly& rhs)
{
    std::swap(_points, rhs._points);
    std::swap(_pointArray, rhs._pointArray);
}

bool
cConvexPoly::operator==(const cConvexPoly& rhs) const
{
    if(sides() != rhs.sides())
    {
        return false;
    }
    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        if(_pointArray[i] != rhs._pointArray[i])
        {
            return false;
        }
    }
    return true;
}

void
cConvexPoly::operator+=(const tPoint& offset)
{
    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        _pointArray[i] += offset;
    }
}
void
cConvexPoly::operator-=(const tPoint& offset)
{
    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        _pointArray[i] -= offset;
    }
}

void
cConvexPoly::normalise(cConvexPoly& result) const
{
    tSigned32 i, leftMost;
    leftMost = 0;
    tPoint leftMostV = vertex(0);
    for(i = 1; i < sides(); ++i)
    {
        tPoint v = vertex(i);
        if(v.getX() < leftMostV.getX()
         || (v.getX() == leftMostV.getX() && v.getY() < leftMostV.getY())
         )
        {
            leftMost = i;
            leftMostV = v;
        }
    }
    delete result._pointArray;
    result._points = _points;
    result._pointArray = new tPoint[sides()];
    tSigned32 j = leftMost;
    for(i = 0; i < sides(); ++i)
    {
        result._pointArray[i] = _pointArray[j];
        j = nextVertex(j);
    }
}

void
cConvexPoly::minkowskiExpand(
        const cConvexPoly& expandBy,
        cConvexPoly& result
        ) const
{
    delete result._pointArray;

  // figure out size of result first

    result._points = 0;
    tSigned32 prevEndExpand;
    {
        tPoint edgeVector = vertex_WrappedAround(sides()) - vertex(sides() - 1);
        prevEndExpand = expandBy.endExpand(edgeVector);
    }
    tSigned32 i;
    for(i = 0; i < sides(); i++)
    {
        tPoint edgeVector = vertex_WrappedAround(i + 1) - vertex(i);
        tSigned32 startExpand, endExpand;
        expandBy.getStartEndExpand(edgeVector, startExpand, endExpand);
        do
        {
            result._points++;
            prevEndExpand = expandBy.nextVertex(prevEndExpand);
        }
        while(prevEndExpand != expandBy.nextVertex(startExpand));
        prevEndExpand = endExpand;
    }

  // allocate result pointArray

    result._pointArray = new tPoint[result._points];

  // and repeat expansion algorithm, filling in points this time

    tSigned32 point = 0;
    {
        tPoint edgeVector = vertex_WrappedAround(sides()) - vertex(sides() - 1);
        prevEndExpand = expandBy.endExpand(edgeVector);
    }
    for(i = 0; i < sides(); i++)
    {
        tPoint edgeVector = vertex_WrappedAround(i + 1) - vertex(i);
        tSigned32 startExpand, endExpand;
        expandBy.getStartEndExpand(edgeVector, startExpand, endExpand);
        do
        {
            result._pointArray[point++] = vertex(i) - expandBy.vertex(prevEndExpand);
            prevEndExpand = expandBy.nextVertex(prevEndExpand);
        }
        while(prevEndExpand != expandBy.nextVertex(startExpand));
        prevEndExpand = endExpand;
    }

    assertD(point == result._points);
}


void
cConvexPoly::extrude(
        const tPoint& extrudeStart, const tPoint& extrudeEnd,
        cConvexPoly& result
        ) const
{
    delete result._pointArray;

    tPoint extrudeVector = extrudeEnd - extrudeStart;

  // get index for first extruded vertex

    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        if(getEdgeAxis(i).side(extrudeVector) == SIDE_RIGHT
         && getEdgeAxis(nextVertex(i)).side(extrudeVector) != SIDE_RIGHT
            )
        {
            break;
        }
    }
    assertD(i != sides());
    tSigned32 endI = i;

  // run the algorithm through a first time, to figure out the size of the result

    result._points = 0;
    i = nextVertex(i);
    result._points++;
    if(getEdgeAxis(i).side(extrudeVector) != SIDE_CENTRE)
    {
        result._points++;
    }
    do
    {
        i = nextVertex(i);
        result._points++;
    }
    while(getEdgeAxis(i).side(extrudeVector) == SIDE_LEFT);
    if(getEdgeAxis(i).side(extrudeVector) != SIDE_CENTRE)
    {
        result._points++;
    }
    while(i != endI)
    {
        i = nextVertex(i);
        result._points++;
    }

  // allocate result pointArray

    result._pointArray = new tPoint[result._points];

  // run through again, filling in points

    tSigned32 point = 0;
    i = endI;
    i = nextVertex(i);
    result._pointArray[point++] = extrudeStart + vertex(i);
    if(getEdgeAxis(i).side(extrudeVector) != SIDE_CENTRE)
    {
        result._pointArray[point++] = extrudeEnd + vertex(i);
    }
    do
    {
        i = nextVertex(i);
        result._pointArray[point++] = extrudeEnd + vertex(i);
    }
    while(getEdgeAxis(i).side(extrudeVector) == SIDE_LEFT);
    if(getEdgeAxis(i).side(extrudeVector) != SIDE_CENTRE)
    {
        result._pointArray[point++] = extrudeStart + vertex(i);
    }
    while(i != endI)
    {
        i = nextVertex(i);
        result._pointArray[point++] = extrudeStart + vertex(i);
    }

    assertD(point == result._points);
}

void
cConvexPoly::offset(const tPoint& offsetBy, cConvexPoly& result) const
{
    if(result._pointArray == 0 || result._points != _points)
    {
        delete result._pointArray;
        result._pointArray = new tPoint[_points];
        result._points = _points;
    }
    tSigned32 i;
    for(i = 0; i != _points; ++i)
    {
        result._pointArray[i] = _pointArray[i] + offsetBy;
    }
}

tSigned32
cConvexPoly::getRangeUsed() const
{
    tSigned32 result = _pointArray[0].getRangeUsed();
    tSigned32 i;
    for(i = 1; i != sides(); ++i)
    {
        if(_pointArray[i].getRangeUsed() > result)
        {
            result = _pointArray[i].getRangeUsed();
        }
    }
    return result;
}
void
cConvexPoly::setRange(tSigned32 value)
{
    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        _pointArray[i].setRange(value);
    }
}

bool
cConvexPoly::pointCollides2D(const tPoint& p) const
{
    tSigned32 i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(vertex(i), vertex_WrappedAround(i + 1));
        if(edgeLine.side(p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}
bool
cConvexPoly::pointCollides2D(const tIntersection& p) const
{
    tSigned32 i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(vertex(i), vertex_WrappedAround(i + 1));
        if(edgeLine.side(p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}
bool
cConvexPoly::pointCollides2D_Inverted(const tPoint& p) const
{
    tSigned32 i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(vertex(i), vertex_WrappedAround(i + 1));
        if(edgeLine.side(p) == SIDE_LEFT)
        {
            return true;
        }
    }
    return false;
}
bool
cConvexPoly::pointCollides2D_Inverted(const tIntersection& p) const
{
    tSigned32 i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(vertex(i), vertex_WrappedAround(i + 1));
        if(edgeLine.side(p) == SIDE_LEFT)
        {
            return true;
        }
    }
    return false;
}
bool
cConvexPoly::pointIsOnBoundary2D(const tIntersection& p) const
{
    tSigned32 i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(vertex(i), vertex_WrappedAround(i + 1));
        if(edgeLine.side(p) != SIDE_CENTRE)
        {
            continue;
        }
        tLine prevEdgeLine(vertex(prevVertex(i)), vertex(i));
        if(prevEdgeLine.side(p) == SIDE_LEFT)
        {
            return false;
        }
        tLine nextEdgeLine(vertex_WrappedAround(i + 1), vertex_WrappedAround(i + 2));
        if(nextEdgeLine.side(p) == SIDE_LEFT)
        {
            return false;
        }
        return true;
    }
    return false;
}

bool
cConvexPoly::lineCollides2D(const tPoint& lineStart, const tPoint& lineAxis, tSigned32& collidingLineIndex) const
{
    tSigned32 i;
    eSide side = lineAxis.side(_pointArray[0] - lineStart);
    if(side == SIDE_LEFT)
    {
        i = _points - 1;
        do
        {
            side = lineAxis.side(_pointArray[i] - lineStart);
        }
        while(side == SIDE_LEFT && --i);
        if(!i)
        {
            // all points are on left
            return false;
        }
        collidingLineIndex = i;
        if(side == SIDE_CENTRE)
        {
            return checkStartAndEndForCornerCrossing(i, lineStart, lineAxis);
        }
        assertD(side == SIDE_RIGHT);
        tLine segmentLine(_pointArray[i], vertex_WrappedAround(i + 1));
        return segmentLine.side(lineStart) != SIDE_RIGHT && segmentLine.side(lineStart + lineAxis) == SIDE_RIGHT;
    }

    if(side == SIDE_CENTRE)
    {
      // first point checked is side centre
        eSide nextSide = lineAxis.side(_pointArray[1] - lineStart);
        if(nextSide != SIDE_RIGHT)
        {
            if(nextSide == SIDE_CENTRE)
            {
                return false;
            }
            collidingLineIndex = 0;
            return checkStartAndEndForCornerCrossing(0, lineStart, lineAxis);
        }
    }

    i = 1;
    do
    {
        side = lineAxis.side(_pointArray[i] - lineStart);
    }
    while(side == SIDE_RIGHT && ++i != _points);
    if(i == _points)
    {
        // all points are on right
        return false;
    }
    if(side == SIDE_CENTRE)
    {
        collidingLineIndex = i;
        return checkStartAndEndForCornerCrossing(i, lineStart, lineAxis);
    }
    assertD(side == SIDE_LEFT);
    tLine segmentLine(_pointArray[i - 1], _pointArray[i]);
    collidingLineIndex = i - 1;
    return segmentLine.side(lineStart) != SIDE_RIGHT && segmentLine.side(lineStart + lineAxis) == SIDE_RIGHT;
}

bool
cConvexPoly::lineCollides2D(
        const tLine& l,
        const cAxisFraction& startDistance, const cAxisFraction& endDistance,
        tSigned32& collidingLineIndex
        ) const
{
    tSigned32 i;
    for(i = 0; i < _points; i++)
    {
        if(LineCollidesWithSegment(l, startDistance, endDistance, &_pointArray[i]))
        {
            collidingLineIndex = i;
            return true;
        }
    }
    return false;
}

bool
cConvexPoly::enters_NotAtCorner(const cConvexPoly& rhs, tSigned32& lhsSection, tSigned32& rhsSection) const
{
    for(lhsSection = 0; lhsSection < _points; lhsSection++)
    {
        tLine lhsSectionLine(_pointArray[lhsSection], vertex_WrappedAround(lhsSection + 1));
        for(rhsSection = 0; rhsSection < rhs._points; rhsSection++)
        {
            tLine rhsSectionLine(rhs._pointArray[rhsSection], rhs.vertex_WrappedAround(rhsSection + 1));
            if(rhsSectionLine.side(lhsSectionLine.start()) == SIDE_LEFT
             && rhsSectionLine.side(lhsSectionLine.end()) == SIDE_RIGHT
             && lhsSectionLine.side(rhsSectionLine.start()) == SIDE_RIGHT
             && lhsSectionLine.side(rhsSectionLine.end()) == SIDE_LEFT)
            {
                return true;
            }
        }
    }
    return false;
}
bool
cConvexPoly::exits_NotAtCorner(const cConvexPoly& rhs, tSigned32& lhsSection, tSigned32& rhsSection) const
{
    for(lhsSection = 0; lhsSection < _points; lhsSection++)
    {
        tLine lhsSectionLine(_pointArray[lhsSection], vertex_WrappedAround(lhsSection + 1));
        for(rhsSection = 0; rhsSection < rhs._points; rhsSection++)
        {
            tLine rhsSectionLine(rhs._pointArray[rhsSection], rhs.vertex_WrappedAround(rhsSection + 1));
            if(rhsSectionLine.side(lhsSectionLine.start()) == SIDE_RIGHT
             && rhsSectionLine.side(lhsSectionLine.end()) == SIDE_LEFT
             && lhsSectionLine.side(rhsSectionLine.start()) == SIDE_LEFT
             && lhsSectionLine.side(rhsSectionLine.end()) == SIDE_RIGHT)
            {
                return true;
            }
        }
    }
    return false;
}

bool
cConvexPoly::overlaps2D(const cConvexPoly& rhs) const
{
    tSigned32 i;
    for(i = 0; i < _points; ++i)
    {
        if(edgeSeparates(i, rhs))
        {
            return false;
        }
    }
    for(i = 0; i < rhs._points; ++i)
    {
        if(rhs.edgeSeparates(i, *this))
        {
            return false;
        }
    }
    return true;
}

bool
cConvexPoly::overlaps2D(const cConvexPoly& rhs, tIntersection& pointInsideOrOnBoundaryOfBoth) const
{
    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        tSigned32 collidingLineIndex;
        if(!rhs.lineCollides2D(
                _pointArray[i],
                vertex_WrappedAround(i + 1) - _pointArray[i],
                collidingLineIndex
                )
            )
        {
            continue;
        }
        pointInsideOrOnBoundaryOfBoth = tIntersection(getBoundaryLine(i), rhs.getBoundaryLine(collidingLineIndex));
        return true;
    }

  // boundaries do not cross
  // check for 2 cases where one shape contains the other

    if(pointCollides2D(rhs.vertex(0)))
    {
        pointInsideOrOnBoundaryOfBoth = rhs.vertex(0);
        return true;
    }
    const cConvexPoly& lhs = *this;
    if(rhs.pointCollides2D(lhs.vertex(0)))
    {
        pointInsideOrOnBoundaryOfBoth = lhs.vertex(0);
        return true;
    }

  // and one pathological case where the shape boundaries are identical

    if(rhs.sides() != sides())
    {
      // boundaries can't be identical
        return false;
    }

    for(i = 0; i != sides(); ++i)
    {
        if(rhs.vertex(0) == lhs.vertex(i))
        {
            break;
        }
    }
    if(i == sides())
    {
      // no match for rhs[0]
        return false;
    }

    ++i;
    if(i == sides())
    {
        i = 0;
    }
    tSigned32 j;
    for(j = 1; j != rhs.sides(); ++j)
    {
        if(lhs.vertex(i) != rhs.vertex(j))
        {
            return false;
        }
        ++i;
        if(i == sides())
        {
            i = 0;
        }
    }

    pointInsideOrOnBoundaryOfBoth = lhs.vertex(0);
    return true;
}

tSigned32
cConvexPoly::getIndexForSilhouettePoint(eSide side, const tPoint& p) const
{
    assertD(!pointCollides2D(p));
    if(side == SIDE_LEFT)
    {
    // find the last edge that p is not on the right of
        tSigned32 i = 0;
        while(tLine(_pointArray[i], vertex_WrappedAround(i + 1)).side(p) == SIDE_RIGHT)
        {
            ++i;
            assertD(i < sides());
        }
        do
        {
            ++i;
            if(i == sides())
            {
                i = 0;
            }
        }
        while(tLine(_pointArray[i], vertex_WrappedAround(i + 1)).side(p) != SIDE_RIGHT);
        // return the index of the point after this last edge
        return i;
    }

    assertD(side == SIDE_RIGHT);
// find the last edge that p is on the right of
    tSigned32 i = 0;
    while(tLine(_pointArray[i], vertex_WrappedAround(i + 1)).side(p) != SIDE_RIGHT)
    {
        ++i;
        assertD(i < sides());
    }
    do
    {
        ++i;
        if(i == sides())
        {
            i = 0;
        }
    }
    while(tLine(_pointArray[i], vertex_WrappedAround(i + 1)).side(p) == SIDE_RIGHT);
    // return the index of the point after this last edge
    return i;
}


bool
cConvexPoly::satisfiesMaxCircumference(tSigned32 maxCircumference) const
{
    tSigned32 circumference = 0;
    tSigned32 i;
    for(i = 0; i != sides(); ++i)
    {
        tPoint v = vertex_WrappedAround(i + 1) - _pointArray[i];
        if(v.getRangeUsed() > maxCircumference)
        {
            return false;
        }
        circumference += Length(v);
        if(circumference > maxCircumference)
        {
            return false;
        }
    }
    return true;
}

void
cConvexPoly::getBounds(tPoint& minimum, tPoint& maximum) const
{
    tSigned32 i;
    minimum = _pointArray[0];
    maximum = minimum;
    for(i = 1; i != sides(); ++i)
    {
        minimum.updateAsMinimumBound(_pointArray[i]);
        maximum.updateAsMaximumBound(_pointArray[i]);
    }
}

void
cConvexPoly::getStartEndExpand(const tPoint& edgeVector, tSigned32 &start, tSigned32 &end) const
{
    tSigned32 i, nextI;
    for(i = 0; i < sides(); i++)
    {
        nextI = nextVertex(i);
        if(expandEdgeVector(i).direction(edgeVector) == DIRECTION_FORWARD)
        {
            start = i;
            end = nextI;
            return;
        }
        if(expandEdgeVector(i).side(edgeVector) == SIDE_RIGHT && 
            expandEdgeVector(nextI).side(edgeVector) == SIDE_LEFT)
        {
            start = nextI;
            end = nextI;
            return;
        }
    }
    invalid();
}
tSigned32
cConvexPoly::getEdgesForCorner(const tPoint& previousEdge, const tPoint& nextEdge) const
{
    eDirection dir;
    tSigned32 previousStart;
    tSigned32 previousEnd;
    tSigned32 nextStart;
    tSigned32 nextEnd;
    tSigned32 edges;

    dir = previousEdge.direction(nextEdge);
    assertD(dir != DIRECTION_BACK);

    getStartEndExpand(previousEdge, previousStart, previousEnd);
    getStartEndExpand(nextEdge, nextStart, nextEnd);

    edges = 0;

    if(dir == DIRECTION_LEFT || dir == DIRECTION_FORWARD)
    {
    // concave corner
    // (or edges are in line)
    // number of interpolated edges will be 0 or negative
        while(nextStart != previousEnd)
        {
            nextStart = nextVertex(nextStart);
            edges--;
        }
    }
    else
    {
    // convex corner
        while(nextStart != previousEnd)
        {
            previousEnd = nextVertex(previousEnd);
            edges++;
        }
    }
    return edges;
}

tSigned32
cConvexPoly::getEdgesForCorner_DontConnectInline(
            const tPoint& previousAxis, const tPoint& nextAxis,
            tSigned32 spinCount
            ) const
{
    assertD(spinCount >= 0);

    tSigned32 previousEnd = startExpand(previousAxis);
    tSigned32 nextStart = startExpand(nextAxis);

    tSigned32 edges;
    edges = 0;

    if(previousAxis.side(nextAxis) != SIDE_RIGHT)
    {
    // concave corner
    // (or in line)
    // number of interpolated edges will be 0 or negative
        while(nextStart != previousEnd)
        {
            nextStart = nextVertex(nextStart);
            edges--;
        }
    }
    else
    {
    // convex corner
    // number of interpolated edges will be 0 or positive
        while(nextStart != previousEnd)
        {
            previousEnd = nextVertex(previousEnd);
            edges++;
        }
    }

    edges += sides() * (spinCount / 2);

    return edges;
}
