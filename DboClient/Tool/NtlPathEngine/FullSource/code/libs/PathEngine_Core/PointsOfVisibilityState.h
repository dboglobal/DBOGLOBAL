//**********************************************************************
//
// Copyright (c) 2007
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINTS_OF_VISIBILITY_STATE_INCLUDED
#define POINTS_OF_VISIBILITY_STATE_INCLUDED

#include "SilhouetteZone.h"
#include "common/interface/Assert.h"

enum eEndPointZone
{
    EPZ_ARRIVING,
    EPZ_DEPARTING,
};

class cPointsOfVisibilityState
{
    tUnsigned32 _i;

public:

    cPointsOfVisibilityState()
    {
    }

	cPointsOfVisibilityState(tSigned32 point, eSilhouetteZone approachZone)
    {
        assertD(point >= 0);
        assertD(point != 0x7fffffffL); // maximim value is reserved for marking invalid states
        assertD(approachZone == SZ_LEFTSILHOUETTE || approachZone == SZ_RIGHTSILHOUETTE);
        _i = point;
        _i *= 2;
        if(approachZone == SZ_LEFTSILHOUETTE)
        {
            ++_i;
        }
    }
	cPointsOfVisibilityState(tSigned32 point, eEndPointZone approachZone)
    {
        assertD(point >= 0);
        assertD(point != 0x7fffffffL); // maximim value is reserved for marking invalid states
        _i = point;
        _i *= 2;
        if(approachZone == EPZ_DEPARTING)
        {
            ++_i;
        }
    }
	cPointsOfVisibilityState(tSigned32 point, bool approachZone)
    {
        assertD(point >= 0);
        assertD(point != 0x7fffffffL); // maximim value is reserved for marking invalid states
        _i = point;
        _i *= 2;
        if(approachZone)
        {
            ++_i;
        }
    }

	bool
	operator==(const cPointsOfVisibilityState& rhs) const
	{
		return _i == rhs._i;
	}

    void
    setInvalid()
    {
        _i = 0xffffffffUL;
    }

	static cPointsOfVisibilityState
	invalidInstance()
	{
		cPointsOfVisibilityState result;
		result.setInvalid();
		return result;
	}

	static cPointsOfVisibilityState
	fromValue(tUnsigned32 value)
	{
		cPointsOfVisibilityState result;
		result._i = value;
		return result;
	}

    cPointsOfVisibilityState
    reversed() const
    {
        assertD(valid());
        cPointsOfVisibilityState result;
		result._i = _i;
        result._i ^= 1;
        return result;
    }

    bool
    valid() const
    {
        return _i != 0xffffffffUL;
    }
    tUnsigned32
    value() const
    {
        assertD(valid());
        return _i;
    }

    tSigned32
    pointIndex() const
    {
        return _i / 2;
    }
    eSilhouetteZone approachSilhouetteZone() const
    {
        return (_i & 1) ? SZ_LEFTSILHOUETTE : SZ_RIGHTSILHOUETTE;
    }
    eEndPointZone approachEndPointZone() const
    {
        return (_i & 1) ? EPZ_DEPARTING : EPZ_ARRIVING;
    }
    void
    decode(tSigned32& point, eSilhouetteZone& approachZone) const
    {
        point = _i / 2;
        approachZone = (_i & 1) ? SZ_LEFTSILHOUETTE : SZ_RIGHTSILHOUETTE;
    }
    void
    decode(tSigned32& point, eEndPointZone& approachZone) const
    {
        point = _i / 2;
        approachZone = (_i & 1) ? EPZ_DEPARTING : EPZ_ARRIVING;
    }

    bool
    operator<(const cPointsOfVisibilityState& rhs) const
    {
        return _i < rhs._i;
    }
};

#endif
