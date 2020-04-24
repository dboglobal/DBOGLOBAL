//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/PathSegmentInterpolator.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/STL/algorithm.h"

tSigned32
cPathSegmentInterpolator::interpolatorValueAtPoint(const tPoint& p) const
{
    switch(_interpolationType)
    {
    default:
        invalid();
    case X_POS:
        return p.getX() - _start.getX();
    case X_NEG:
        return _start.getX() - p.getX();
    case Y_POS:
        return p.getY() - _start.getY();
    case Y_NEG:
        return _start.getY() - p.getY();
    }
}
tSigned32
cPathSegmentInterpolator::otherAxisValueAtPoint(const tPoint& p) const
{
    if(_interpolationType == X_POS || _interpolationType == X_NEG)
    {
        return p.getY();
    }
    return p.getX();
}
tPoint
cPathSegmentInterpolator::pointFor(tSigned32 interpolatorValue, tSigned32 otherAxisValue) const
{
    tSigned32 effectiveRange;
    effectiveRange = _start.getRange();
    if(_end.getRange() > effectiveRange)
    {
        effectiveRange = _end.getRange();
    }
    switch(_interpolationType)
    {
    default:
        invalid();
    case X_POS:
        return tPoint(_start.getX() + interpolatorValue, otherAxisValue, effectiveRange);
    case X_NEG:
        return tPoint(_start.getX() - interpolatorValue, otherAxisValue, effectiveRange);
    case Y_POS:
        return tPoint(otherAxisValue, _start.getY() + interpolatorValue, effectiveRange);
    case Y_NEG:
        return tPoint(otherAxisValue, _start.getY() - interpolatorValue, effectiveRange);
    }
}

void
cPathSegmentInterpolator::initOrReinit(const tPoint& start, const tPoint& end)
{
    _start = start;
    _end = end;

    tLine line(_start, _end);
    tPoint axis = line.axis();
    _length = Length(axis);

    tSigned32 absoluteX = axis.getX();
    if(absoluteX < 0)
    {
        absoluteX = -absoluteX;
    }
    tSigned32 absoluteY = axis.getY();
    if(absoluteY < 0)
    {
        absoluteY = -absoluteY;
    }
    
    assertD(absoluteX || absoluteY);
    if(absoluteX >= absoluteY)
    {
        if(axis.getX() > 0)
        {
            _interpolationType = X_POS;
        }
        else
        {
            _interpolationType = X_NEG;
        }
    }
    else
    {
        if(axis.getY() > 0)
        {
            _interpolationType = Y_POS;
        }
        else
        {
            _interpolationType = Y_NEG;
        }
    }
}

const tPoint&
cPathSegmentInterpolator::refStart() const
{
    return _start;
}
tSigned32
cPathSegmentInterpolator::segmentLength() const
{
    return _length;
}
tSigned32
cPathSegmentInterpolator::interpolatorMax() const
{
    return interpolatorValueAtPoint(_end);
}

void
cPathSegmentInterpolator::interpolate(
        const tPoint& currentPosition,
        tSigned32 interpolatorTarget,
        bool& exactlyOnLine, tPoint& point1, tPoint& point2
        ) const
{
    assertD(interpolatorTarget >= 0);
    assertD(interpolatorTarget < interpolatorMax());
    
    tSigned32 interpolatorBase = interpolatorValueAtPoint(currentPosition);
    assertD(interpolatorBase >= 0);
    assertD(interpolatorBase < interpolatorMax());
    assertD(interpolatorTarget >= interpolatorBase);
    if(interpolatorTarget == interpolatorBase)
    {
        exactlyOnLine = true;
        point1 = currentPosition;
        return;
    }

    tSigned32 otherAxisBase = otherAxisValueAtPoint(currentPosition);
    tSigned32 otherAxisToEnd = otherAxisValueAtPoint(_end) - otherAxisBase;

    tSigned32 interpolaterToEnd = interpolatorMax() - interpolatorBase;
    tSigned32 interpolaterToTarget = interpolatorTarget - interpolatorBase;

    tPoint::tDistance otherAxisToTarget_Multiplied;
    tMultiplier::multiply(otherAxisToEnd, interpolaterToTarget, otherAxisToTarget_Multiplied);
    tSigned32 otherAxisToTarget;
    tMultiplier::divide(otherAxisToTarget_Multiplied, interpolaterToEnd, otherAxisToTarget);

    point1 = pointFor(interpolatorTarget, otherAxisBase + otherAxisToTarget);

    tLine l(currentPosition, _end);
    eSide side1 = l.side(point1);
    if(side1 == SIDE_CENTRE)
    {
        exactlyOnLine = true;
        return;
    }

    exactlyOnLine = false;
    point2 = pointFor(interpolatorTarget, otherAxisBase + otherAxisToTarget + 1);
    eSide side2 = l.side(point2);
    assertD(side2 != SIDE_CENTRE);
    if(side2 == side1)
    {
        point2 = pointFor(interpolatorTarget, otherAxisBase + otherAxisToTarget - 1);
        side2 = l.side(point2);
        assertD(side2 == OtherSide(side1));
    }
    
    eSide preferredSide = l.side(_start); // prefer to make point1 approximate in direction to negate drift for current position
    if(preferredSide == SIDE_CENTRE)
    { 
        preferredSide = SIDE_LEFT; // in the absence of drift this is chosen arbitrarily to make the resulting points deterministic
    }

    if(side1 != preferredSide)
    {
        std::swap(point1, point2);    
    }
}
