//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ShapeCollisionPreprocess.h"
#include "interface/LocalisedConvexShape.h"
#include "libs/Instrumentation/interface/Profiler.h"

cShapeCollisionPreprocess::cShapeCollisionPreprocess()
{
    _expansion2D = 0;
}
cShapeCollisionPreprocess::~cShapeCollisionPreprocess()
{
    delete _expansion2D;
}

bool
cShapeCollisionPreprocess::isValidFor(const cInternalPosition& baseShapePosition) const
{
    return _preprocessedPosition == baseShapePosition;
}

void
cShapeCollisionPreprocess::initOrReinit(
        cQueryContext& qc,
        const cConvexPoly& placedBaseShape,
        const cInternalPosition& position
        )
{
    PROFILER_SCOPE("cShapeCollisionPreprocess::reinit");
    _preprocessedPosition = position;
    delete _expansion2D;
    _expansion2D = new cLocalisedConvexShape(qc, placedBaseShape, position);
}

void
cShapeCollisionPreprocess::initOrReinit(
        cQueryContext& qc,
        const cConvexPoly& placedBaseShape,
        const cInternalPosition& position,
        const cConvexPoly& expandShape
        )
{
    PROFILER_SCOPE("cShapeCollisionPreprocess::reinit");
    _preprocessedPosition = position;
    delete _expansion2D;
    cConvexPoly combinedShape;
    placedBaseShape.minkowskiExpand(expandShape, combinedShape);
    _expansion2D = new cLocalisedConvexShape(qc, combinedShape, position);
}
