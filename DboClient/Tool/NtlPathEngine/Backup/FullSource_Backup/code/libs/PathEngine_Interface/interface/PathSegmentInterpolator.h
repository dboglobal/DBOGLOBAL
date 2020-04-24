//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_INTERFACE__PATH_SEGMENT_INTERPOLATOR_INCLUDED
#define PATHENGINE_INTERFACE__PATH_SEGMENT_INTERPOLATOR_INCLUDED

#include "libs/Geometry/interface/tPoint.h"

class cPathSegmentInterpolator
{
    enum eInterpolationType
    {
        X_POS,
        X_NEG,
        Y_POS,
        Y_NEG,
    };

    tPoint _start, _end;
    tSigned32 _length;
    eInterpolationType _interpolationType;

    tSigned32 interpolatorValueAtPoint(const tPoint& p) const;
    tSigned32 otherAxisValueAtPoint(const tPoint& p) const;
    tPoint pointFor(tSigned32 interpolatorValue, tSigned32 otherAxisValue) const;

public:

    void initOrReinit(const tPoint& start, const tPoint& end);
    
    const tPoint& refStart() const;
    tSigned32 segmentLength() const;
    tSigned32 interpolatorMax() const;

    // interpolatorValue must be >= 0 and < interpolatorMax
    // point1 is chosen to negate any drift whenever currentPosition is not exactly on the original line
    void interpolate(const tPoint& currentPosition, tSigned32 interpolatorTarget, bool& exactlyOnLine, tPoint& point1, tPoint& point2) const;
};

#endif
