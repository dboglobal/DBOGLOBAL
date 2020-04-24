//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"

class cQueryContext;
class cLocalisedConvexShape;
class cCornerProvider;

void
AddShapeCorners(cQueryContext& qc, const cLocalisedConvexShape& shape, cCornerProvider& addTo);

void
AddShapeCorners(
        cQueryContext& qc,
        const cLocalisedConvexShape& shape,
        const tPoint& boundsMin,
        const tPoint& boundsMax,
        cCornerProvider& addTo
        );
