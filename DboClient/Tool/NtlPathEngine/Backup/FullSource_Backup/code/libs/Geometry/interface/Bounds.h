//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"

// returns false in the case where the region boundaries coincide
bool
BoundsOverlap(
        const tPoint& min1,
        const tPoint& max1,
        const tPoint& min2,
        const tPoint& max2
        );

// returns true in the case where the region boundaries coincide
bool
BoundsOverlap_Inclusive(
        const tPoint& min1,
        const tPoint& max1,
        const tPoint& min2,
        const tPoint& max2
        );
